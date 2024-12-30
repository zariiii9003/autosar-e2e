/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdbool.h>
#include <stdint.h>

#include "crclib.h"
#include "util.h"

#define P04LENGTH_POS  0
#define P04LENGTH_LEN  2
#define P04COUNTER_POS 2
#define P04COUNTER_LEN 2
#define P04DATAID_POS  4
#define P04DATAID_LEN  4
#define P04CRC_POS     8
#define P04CRC_LEN     4
#define P04HEADER_LEN  (P04LENGTH_LEN+P04COUNTER_LEN+P04DATAID_LEN+P04CRC_LEN)

uint32_t compute_p04_crc(uint8_t *data_ptr,
                         uint16_t length,
                         uint16_t offset)
{
    uint32_t crc;

    // bytes before crc bytes
    uint32_t crc_offset = (uint32_t)(offset + P04CRC_POS);
    crc = Crc_CalculateCRC32P4(data_ptr, crc_offset, CRC32P4_INITIAL_VALUE, true);

    // bytes after crc bytes, if any
    if (offset + P04HEADER_LEN < length){
        uint8_t * second_part_ptr = data_ptr + offset + P04HEADER_LEN;
        uint32_t second_part_length = length - offset - P04HEADER_LEN;
        crc = Crc_CalculateCRC32P4(second_part_ptr, second_part_length, crc, false);
    }
    return crc;
}

PyDoc_STRVAR(e2e_p04_protect_doc,
             "e2e_p04_protect(data: bytearray, length: int, data_id: int, *, offset: int = 0, increment_counter: bool = True) -> None \n"
             "Calculate CRC inplace according to AUTOSAR E2E Profile 4. \n"
             "\n"
             ":param bytearray data: \n"
             "    Mutable `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_.\n"
             ":param int length: \n"
             "    Number of data bytes which are considered for CRC calculation. `length` must fulfill \n"
             "    the following condition: ``12 <= length <= len(data)`` \n"
             ":param int data_id: \n"
             "    A unique identifier which is used to protect against masquerading. The `data_id` is a 32bit unsigned integer. \n"
             ":param int offset: \n"
             "    Byte offset of the E2E header. \n"
             ":param bool increment_counter: \n"
             "    If `True` the counter will be incremented before calculating the CRC. \n");

static PyObject *
py_e2e_p04_protect(PyObject *module,
                   PyObject *args,
                   PyObject *kwargs)
{
    Py_buffer data;
    unsigned short length;
    unsigned long data_id;
    unsigned short offset = 0;
    int increment = true;

    static char *kwlist[] = {
        "data",
        "length",
        "data_id",
        "offset",
        "increment_counter",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*Hk|$Hp:e2e_p04_protect",
                                     kwlist, &data, &length, &data_id, &offset, &increment))
    {
        return NULL;
    }
    if (data.readonly)
    {
        PyErr_SetString(PyExc_ValueError, "\"data\" must be mutable. Use a bytearray or any object that implements the buffer protocol.");
        goto error;
    }
    if (data.len < P04HEADER_LEN)
    {
        PyErr_SetString(PyExc_ValueError, "The length of bytearray \"data\" must be greater than or equal to 12.");
        goto error;
    }
    if (length < P04HEADER_LEN || length > data.len)
    {
        PyErr_SetString(PyExc_ValueError, "Parameter \"length\" must fulfill the following condition: 12 <= length <= len(data).");
        goto error;
    }
    if (offset > data.len - P04HEADER_LEN)
    {
        PyErr_SetString(PyExc_ValueError, "Argument \"offset\" invalid.");
        goto error;
    }

    uint8_t *data_ptr = (uint8_t *)data.buf;

    // write length
    uint16_to_bigendian(data_ptr + offset + P04LENGTH_POS, length);

    // increment counter
    if (increment) {
        uint16_t counter = bigendian_to_uint16(data_ptr + offset + P04COUNTER_POS);
        counter++;
        uint16_to_bigendian(data_ptr + offset + P04COUNTER_POS, counter);
    }

    // write data_id
    uint32_to_bigendian(data_ptr + offset + P04DATAID_POS, data_id);

    // calculate CRC
    uint32_t crc = compute_p04_crc(data_ptr, length, offset);
    uint32_to_bigendian(data_ptr + offset + P04CRC_POS, crc);

    PyBuffer_Release(&data);

    Py_RETURN_NONE;

error:
    PyBuffer_Release(&data);
    return NULL;
}

PyDoc_STRVAR(e2e_p04_check_doc,
             "e2e_p04_check(data: bytes, length: int, data_id: int, *, offset: int = 0) -> bool \n"
             "Return ``True`` if CRC is correct according to AUTOSAR E2E Profile 4. \n"
             "\n"
             ":param data: \n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_. \n"
             ":param length: \n"
             "    Data byte count over which the CRC must be calculated. `length` must fulfill \n"
             "    the following condition: ``12 <= length <= len(data)`` \n"
             ":param int data_id: \n"
             "    A unique identifier which is used to protect against masquerading. The `data_id` is a 32bit unsigned integer. \n"
             ":param int offset: \n"
             "    Byte offset of the E2E header. \n"
             ":return:\n"
             "    `True` if CRC is valid, otherwise return `False`");

static PyObject *
py_e2e_p04_check(PyObject *module,
                 PyObject *args,
                 PyObject *kwargs)
{
    Py_buffer data;
    unsigned short length;
    unsigned long data_id;
    unsigned short offset = 0;

    static char *kwlist[] = {
        "data",
        "length",
        "data_id",
        "offset",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*Hk|$H:e2e_p04_check",
                                     kwlist, &data, &length, &data_id, &offset))
    {
        return NULL;
    }
    if (data.len < P04HEADER_LEN)
    {
        PyErr_SetString(PyExc_ValueError, "The length of bytearray \"data\" must be greater or equal to 12.");
        goto error;
    }
    if (length < P04HEADER_LEN || length > data.len)
    {
        PyErr_SetString(PyExc_ValueError, "Parameter \"length\" must fulfill the following condition: 12 <= length <= len(data).");
        goto error;
    }
    if (offset > data.len - P04HEADER_LEN)
    {
        PyErr_SetString(PyExc_ValueError, "Argument \"offset\" invalid.");
        goto error;
    }

    uint8_t *data_ptr = (uint8_t *)data.buf;

    // read length
    uint16_t length_actual = bigendian_to_uint16(data_ptr + offset + P04LENGTH_POS);

    // read data_id
    uint32_t data_id_actual = bigendian_to_uint32(data_ptr + offset + P04DATAID_POS);

    // read crc
    uint32_t crc_actual = bigendian_to_uint32(data_ptr + offset + P04CRC_POS);

    // calculate CRC
    uint32_t crc = compute_p04_crc(data_ptr, length, offset);

    PyBuffer_Release(&data);

    if ((length_actual == length) && (data_id_actual == data_id) && (crc_actual == crc))
    {
        Py_RETURN_TRUE;
    }
    else
    {
        Py_RETURN_FALSE;
    }

error:
    PyBuffer_Release(&data);
    return NULL;
}

static struct PyMethodDef methods[] = {
    {"e2e_p04_protect",
     (PyCFunction)py_e2e_p04_protect,
     METH_VARARGS | METH_KEYWORDS,
     e2e_p04_protect_doc},
    {"e2e_p04_check",
     (PyCFunction)py_e2e_p04_check,
     METH_VARARGS | METH_KEYWORDS,
     e2e_p04_check_doc},
    {NULL} // sentinel
};

static PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "e2e.p04",
    .m_doc = "",
    .m_size = -1,
    .m_methods = methods};

PyMODINIT_FUNC PyInit_p04(void)
{

    PyObject *module_p;
    module_p = PyModule_Create(&module);

    if (module_p == NULL)
        return (NULL);

    return (module_p);
}
