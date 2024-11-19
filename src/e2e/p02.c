/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdbool.h>
#include <stdint.h>

#include "crclib.h"

PyDoc_STRVAR(e2e_p02_protect_doc,
             "e2e_p02_protect(data: bytearray, length: int, data_id_list: bytes, *, increment_counter: bool = True) -> None \n"
             "Calculate CRC inplace according to AUTOSAR E2E Profile 2. \n"
             "\n"
             ":param bytearray data: \n"
             "    Mutable `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    starting with the CRC byte. This CRC byte will be updated inplace. \n"
             ":param int length: \n"
             "    Number of data bytes which are considered for CRC calculation. `length` must fulfill \n"
             "    the following condition: ``1 <= length <= len(data) - 1`` \n"
             ":param bytes data_id_list: \n"
             "    A `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    of length 16 which is used to protect against masquerading. \n"
             ":param bool increment_counter: \n"
             "    If `True` the counter in byte 1 will be incremented before calculating the CRC. \n");

static PyObject *
py_e2e_p02_protect(PyObject *module,
                   PyObject *args,
                   PyObject *kwargs)
{
    Py_buffer data;
    unsigned long length;
    Py_buffer data_id_list;
    int increment = true;

    static char *kwlist[] = {
        "data",
        "length",
        "data_id_list",
        "increment_counter",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*ky*|$p:e2e_p02_protect",
                                     kwlist, &data, &length, &data_id_list, &increment))
    {
        return NULL;
    }
    if (data.readonly)
    {
        PyErr_SetString(PyExc_ValueError, "\"data\" must be mutable. Use a bytearray or any object that implements the buffer protocol.");
        goto error;
    }
    if (data.len <= 2)
    {
        PyErr_SetString(PyExc_ValueError, "The length of bytearray \"data\" must be greater than 2.");
        goto error;
    }
    if (length < 1 || length > data.len - 1)
    {
        PyErr_SetString(PyExc_ValueError, "Parameter \"length\" must fulfill the following condition: 1 <= length <= len(data) - 1.");
        goto error;
    }
    if (data_id_list.len != 16)
    {
        PyErr_SetString(PyExc_ValueError, "Argument \"data_id_list\" must be a bytes object with length 16.");
        goto error;
    }

    uint8_t *data_ptr = (uint8_t *)data.buf;
    uint8_t *data_id_ptr = (uint8_t *)data_id_list.buf;

    // increment counter
    uint8_t counter = data_ptr[1] & 0x0Fu;
    if (increment) {
        counter = (counter + 1) % 16u;
        data_ptr[1] = (data_ptr[1] & 0xF0u) | counter;
    }

    // calculate CRC
    uint8_t crc = Crc_CalculateCRC8H2F(data_ptr + 1, (uint32_t)length, CRC8H2F_INITIAL_VALUE, true);
    crc = Crc_CalculateCRC8H2F(data_id_ptr + counter, 1u, crc, false);
    data_ptr[0] = crc;

    PyBuffer_Release(&data);
    PyBuffer_Release(&data_id_list);

    Py_RETURN_NONE;

error:
    PyBuffer_Release(&data);
    PyBuffer_Release(&data_id_list);
    return NULL;
}

PyDoc_STRVAR(e2e_p02_check_doc,
             "e2e_p02_check(data: bytes, length: int, data_id_list: bytes) -> bool \n"
             "Return ``True`` if CRC is correct according to AUTOSAR E2E Profile 2. \n"
             "\n"
             ":param data: \n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    starting with the CRC byte. \n"
             ":param length: \n"
             "    Data byte count over which the CRC must be calculated. `length` must fulfill \n"
             "    the following condition: ``1 <= length <= len(data) - 1`` \n"
             ":param data_id_list: \n"
             "    A `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    of length 16 which is used to protect against masquerading. \n"
             ":return:\n"
             "    `True` if CRC is valid, otherwise return `False`");

static PyObject *
py_e2e_p02_check(PyObject *module,
                 PyObject *args,
                 PyObject *kwargs)
{
    Py_buffer data;
    unsigned long length;
    Py_buffer data_id_list;

    static char *kwlist[] = {
        "data",
        "length",
        "data_id_list",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*ky*:py_e2e_p02_protect",
                                     kwlist, &data, &length, &data_id_list))
    {
        return NULL;
    }

    if (data.len < 2)
    {
        PyErr_SetString(PyExc_ValueError, "The length of bytearray \"data\" must be greater than 1.");
        goto error;
    }
    if (length < 1 || length > data.len - 1)
    {
        PyErr_SetString(PyExc_ValueError, "Parameter \"length\" must fulfill the following condition: 1 <= length < len(data).");
        goto error;
    }
    if (data_id_list.len != 16)
    {
        PyErr_SetString(PyExc_ValueError, "Argument \"data_id_list\" must be a bytes object with length 16.");
        goto error;
    }

    uint8_t *data_ptr = (uint8_t *)data.buf;
    uint8_t *data_id_ptr = (uint8_t *)data_id_list.buf;

    // get counter
    uint8_t counter = data_ptr[1] & 0x0Fu;

    // calculate CRC
    uint8_t crc = Crc_CalculateCRC8H2F(data_ptr + 1, (uint32_t)length, CRC8H2F_INITIAL_VALUE, true);
    crc = Crc_CalculateCRC8H2F(data_id_ptr + counter, 1u, crc, false);

    PyBuffer_Release(&data);
    PyBuffer_Release(&data_id_list);

    if (data_ptr[0] == crc)
    {
        Py_RETURN_TRUE;
    }
    else
    {
        Py_RETURN_FALSE;
    }

error:
    PyBuffer_Release(&data);
    PyBuffer_Release(&data_id_list);
    return NULL;
}

static struct PyMethodDef methods[] = {
    {"e2e_p02_protect",
     (PyCFunction)py_e2e_p02_protect,
     METH_VARARGS | METH_KEYWORDS,
     e2e_p02_protect_doc},
    {"e2e_p02_check",
     (PyCFunction)py_e2e_p02_check,
     METH_VARARGS | METH_KEYWORDS,
     e2e_p02_check_doc},
    {NULL} // sentinel
};

static PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "e2e.p02",
    .m_doc = "",
    .m_size = -1,
    .m_methods = methods};

PyMODINIT_FUNC PyInit_p02(void)
{

    PyObject *module_p;
    module_p = PyModule_Create(&module);

    if (module_p == NULL)
        return (NULL);

    return (module_p);
}
