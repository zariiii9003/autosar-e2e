/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdbool.h>
#include <stdint.h>

#include "crclib.h"
#include "util.h"

#define P05LENGTH_POS 0
#define P05LENGTH_LEN 0
#define P05COUNTER_POS 2
#define P05COUNTER_LEN 1
#define P05DATAID_POS 0
#define P05DATAID_LEN 0
#define P05CRC_POS 0
#define P05CRC_LEN 2
#define P05HEADER_LEN (P05CRC_LEN+P05COUNTER_LEN)

uint16_t compute_p05_crc(uint8_t *data_ptr, 
                         uint16_t length, 
                         uint16_t data_id, 
                         uint16_t offset)
{
    // 'length' does not contain CRC
    uint16_t crc;
    uint8_t data_id_lo_byte = (uint8_t)data_id;
    uint8_t data_id_hi_byte = (uint8_t)(data_id >> 8);
    if (offset > 0)
    {
        crc = Crc_CalculateCRC16(data_ptr, offset, CRC16_INITIAL_VALUE, true);
        crc = Crc_CalculateCRC16(&data_ptr[offset + P05COUNTER_POS], length - offset, crc, false);
    }
    else
    {
        crc = Crc_CalculateCRC16(&data_ptr[P05COUNTER_POS], length, CRC16_INITIAL_VALUE, true);
    }
    crc = Crc_CalculateCRC16(&data_id_lo_byte, 1, crc, false);
    crc = Crc_CalculateCRC16(&data_id_hi_byte, 1, crc, false);

    return crc;
}

PyDoc_STRVAR(e2e_p05_protect_doc,
             "e2e_p05_protect(data: bytearray, length: int, data_id: int, *, offset: int = 0, increment_counter: bool = True) -> None \n"
             "Calculate CRC inplace according to AUTOSAR E2E Profile 5. \n"
             "\n"
             ":param bytearray data: \n"
             "    Mutable `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_.\n"
             ":param int length: \n"
             "    Number of data bytes which are considered for CRC calculation. `length` must fulfill \n"
             "    the following condition: ``1 <= length <= len(data) - 2`` \n"
             ":param int data_id: \n"
             "    A unique identifier which is used to protect against masquerading. The `data_id` is a 16bit unsigned integer. \n"
             ":param int offset: \n"
             "    Byte offset of the E2E header. \n"
             ":param bool increment_counter: \n"
             "    If `True` the counter will be incremented before calculating the CRC. \n");

static PyObject *
py_e2e_p05_protect(PyObject *module,
                   PyObject *args,
                   PyObject *kwargs)
{
    Py_buffer data;
    unsigned short length;
    unsigned short data_id;
    unsigned short offset = 0;
    int increment = true;

    static char *kwlist[] = {
        "data",
        "length",
        "data_id",
        "offset",
        "increment_counter",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*HH|$Hp:e2e_p05_protect",
                                     kwlist, &data, &length, &data_id, &offset, &increment))
    {
        return NULL;
    }
    if (data.readonly)
    {
        PyErr_SetString(PyExc_ValueError, "\"data\" must be mutable. Use a bytearray or any object that implements the buffer protocol.");
        goto error;
    }
    if (data.len <= P05HEADER_LEN)
    {
        PyErr_SetString(PyExc_ValueError, "The length of bytearray \"data\" must be greater than 3.");
        goto error;
    }
    if (length < P05COUNTER_LEN || length > data.len - P05CRC_LEN)
    {
        PyErr_SetString(PyExc_ValueError, "Parameter \"length\" must fulfill the following condition: 1 <= length <= len(data) - 2.");
        goto error;
    }
    if (offset > data.len - P05HEADER_LEN)
    {
        PyErr_SetString(PyExc_ValueError, "Argument \"offset\" invalid.");
        goto error;
    }

    uint8_t *data_ptr = (uint8_t *)data.buf;

    // increment counter
    if (increment) {
        data_ptr[offset + P05COUNTER_POS]++;
    }
    // calculate CRC
    uint16_t crc = compute_p05_crc(data_ptr, length, data_id, offset);
    uint16_to_littleendian(data_ptr + offset + P05CRC_POS, crc);

    PyBuffer_Release(&data);

    Py_RETURN_NONE;

error:
    PyBuffer_Release(&data);
    return NULL;
}

PyDoc_STRVAR(e2e_p05_check_doc,
             "e2e_p05_check(data: bytes, length: int, data_id: int, *, offset: int = 0) -> bool \n"
             "Return ``True`` if CRC is correct according to AUTOSAR E2E Profile 5. \n"
             "\n"
             ":param data: \n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_. \n"
             ":param length: \n"
             "    Data byte count over which the CRC must be calculated. `length` must fulfill \n"
             "    the following condition: ``1 <= length <= len(data) - 2`` \n"
             ":param int data_id: \n"
             "    A unique identifier which is used to protect against masquerading. The `data_id` is a 16bit unsigned integer. \n"
             ":param int offset: \n"
             "    Byte offset of the E2E header. \n"
             ":return:\n"
             "    `True` if CRC is valid, otherwise return `False`");

static PyObject *
py_e2e_p05_check(PyObject *module,
                 PyObject *args,
                 PyObject *kwargs)
{
    Py_buffer data;
    unsigned short length;
    unsigned short data_id;
    unsigned short offset = 0;

    static char *kwlist[] = {
        "data",
        "length",
        "data_id",
        "offset",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*HH|$H:e2e_p05_check",
                                     kwlist, &data, &length, &data_id, &offset))
    {
        return NULL;
    }

    if (data.len <= P05HEADER_LEN)
    {
        PyErr_SetString(PyExc_ValueError, "The length of bytearray \"data\" must be greater than 3.");
        goto error;
    }
    if (length < P05COUNTER_LEN || length > data.len - P05CRC_LEN)
    {
        PyErr_SetString(PyExc_ValueError, "Parameter \"length\" must fulfill the following condition: 1 <= length <= len(data) - 2.");
        goto error;
    }
    if (offset > data.len - P05HEADER_LEN)
    {
        PyErr_SetString(PyExc_ValueError, "Argument \"offset\" invalid.");
        goto error;
    }

    uint8_t *data_ptr = (uint8_t *)data.buf;

    // calculate CRC
    uint16_t crc = compute_p05_crc(data_ptr, length, data_id, offset);
    uint16_t crc_actual = littleendian_to_uint16(data_ptr + offset + P05CRC_POS);

    PyBuffer_Release(&data);

    if (crc == crc_actual)
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
    {"e2e_p05_protect",
     (PyCFunction)py_e2e_p05_protect,
     METH_VARARGS | METH_KEYWORDS,
     e2e_p05_protect_doc},
    {"e2e_p05_check",
     (PyCFunction)py_e2e_p05_check,
     METH_VARARGS | METH_KEYWORDS,
     e2e_p05_check_doc},
    {NULL} // sentinel
};

static PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "e2e.p05",
    .m_doc = "",
    .m_size = -1,
    .m_methods = methods};

PyMODINIT_FUNC PyInit_p05(void)
{

    PyObject *module_p;
    module_p = PyModule_Create(&module);

    if (module_p == NULL)
        return (NULL);

    return (module_p);
}
