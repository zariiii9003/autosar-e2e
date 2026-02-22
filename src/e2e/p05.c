/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdbool.h>
#include <stdint.h>

#include "crclib.h"
#include "util.h"

#define P05LENGTH_POS    0u
#define P05LENGTH_LEN    0u
#define P05COUNTER_POS   2u
#define P05COUNTER_LEN   1u
#define P05DATAID_POS    0u
#define P05DATAID_LEN    0u
#define P05CRC_POS       0u
#define P05CRC_LEN       2u
#define P05CALCULATE_CRC Crc_CalculateCRC16

#define P05HEADER_LEN    (P05CRC_LEN + P05COUNTER_LEN)

uint16_t compute_p05_crc(uint8_t *data_ptr, uint16_t length, uint16_t data_id, uint16_t offset)
{
    uint16_t crc;
    uint8_t  data_id_lo_byte = (uint8_t)data_id;
    uint8_t  data_id_hi_byte = (uint8_t)(data_id >> 8);
    if (offset > 0) {
        crc = P05CALCULATE_CRC(data_ptr, offset, CRC16_INITIAL_VALUE, true);
        crc = P05CALCULATE_CRC(&data_ptr[offset + P05COUNTER_POS],
                               length - (offset + P05COUNTER_POS),
                               crc,
                               false);
    }
    else {
        crc = P05CALCULATE_CRC(&data_ptr[P05COUNTER_POS],
                               length - P05COUNTER_POS,
                               CRC16_INITIAL_VALUE,
                               true);
    }
    crc = P05CALCULATE_CRC(&data_id_lo_byte, 1, crc, false);
    crc = P05CALCULATE_CRC(&data_id_hi_byte, 1, crc, false);

    return crc;
}

// clang-format off
PyDoc_STRVAR(e2e_p05_protect_doc,
             "e2e_p05_protect(data: bytearray, data_id: int, *, length: int = 0, offset: int = 0, increment_counter: bool = True) -> None \n"
             "Calculate CRC inplace according to AUTOSAR E2E Profile 5. \n"
             "\n"
             ":param bytearray data: \n"
             "    Mutable `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_.\n"
             ":param int data_id: \n"
             "    A unique identifier which is used to protect against masquerading. The `data_id` is a 16bit unsigned integer. \n"
             ":param int length: \n"
             "    Number of bytes to consider for CRC calculation. \n"
             "    If ``length == 0``, the full buffer length (``len(data)``) is used. Otherwise, ``3 <= length <= len(data)`` must hold."
             ":param int offset: \n"
             "    Byte offset of the E2E header. \n"
             ":param bool increment_counter: \n"
             "    If `True` the counter will be incremented before calculating the CRC. \n");
// clang-format on
static PyObject *py_e2e_p05_protect(PyObject *module, PyObject *args, PyObject *kwargs)
{
    Py_buffer      data;
    unsigned short data_id;
    unsigned short length    = 0u;
    unsigned short offset    = 0u;
    int            increment = true;

    static char   *kwlist[]  = {"data", "data_id", "length", "offset", "increment_counter", NULL};

    if (!PyArg_ParseTupleAndKeywords(args,
                                     kwargs,
                                     "y*H|$HHp:e2e_p05_protect",
                                     kwlist,
                                     &data,
                                     &data_id,
                                     &length,
                                     &offset,
                                     &increment)) {
        return NULL;
    }
    if (data.readonly) {
        PyErr_SetString(PyExc_ValueError,
                        "\"data\" must be mutable. Use a bytearray or any "
                        "object that implements the buffer protocol.");
        goto error;
    }
    if (data.len < P05HEADER_LEN) {
        PyErr_SetString(PyExc_ValueError, "The length of bytearray \"data\" must be greater than 2.");
        goto error;
    }
    if (length == 0u) {
        length = data.len;
    }
    else if (length < P05HEADER_LEN || length > data.len) {
        PyErr_SetString(PyExc_ValueError,
                        "Parameter \"length\" must fulfill the following "
                        "condition: 3 <= length <= len(data).");
        goto error;
    }
    if (offset > length - P05HEADER_LEN) {
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

// clang-format off
PyDoc_STRVAR(e2e_p05_check_doc,
             "e2e_p05_check(data: bytes, data_id: int, *, length: int = 0, offset: int = 0) -> bool \n"
             "Return ``True`` if CRC is correct according to AUTOSAR E2E Profile 5. \n"
             "\n"
             ":param data: \n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_. \n"
             ":param int data_id: \n"
             "    A unique identifier which is used to protect against masquerading. The `data_id` is a 16bit unsigned integer. \n"
             ":param int length: \n"
             "    Number of bytes to consider for CRC calculation. \n"
             "    If ``length == 0``, the full buffer length (``len(data)``) is used. Otherwise, ``3 <= length <= len(data)`` must hold."
             ":param int offset: \n"
             "    Byte offset of the E2E header. \n"
             ":return:\n"
             "    `True` if CRC is valid, otherwise return `False`");
// clang-format on
static PyObject *py_e2e_p05_check(PyObject *module, PyObject *args, PyObject *kwargs)
{
    Py_buffer      data;
    unsigned short data_id;
    unsigned short length   = 0u;
    unsigned short offset   = 0u;

    static char   *kwlist[] = {"data", "data_id", "length", "offset", NULL};

    if (!PyArg_ParseTupleAndKeywords(args,
                                     kwargs,
                                     "y*H|$HH:e2e_p05_check",
                                     kwlist,
                                     &data,
                                     &data_id,
                                     &length,
                                     &offset)) {
        return NULL;
    }

    if (data.len < P05HEADER_LEN) {
        PyErr_SetString(PyExc_ValueError, "The length of bytearray \"data\" must be greater than 2.");
        goto error;
    }
    if (length == 0u) {
        length = data.len;
    }
    else if (length < P05HEADER_LEN || length > data.len) {
        PyErr_SetString(PyExc_ValueError,
                        "Parameter \"length\" must fulfill the following "
                        "condition: 3 <= length <= len(data).");
        goto error;
    }
    if (offset > length - P05HEADER_LEN) {
        PyErr_SetString(PyExc_ValueError, "Argument \"offset\" invalid.");
        goto error;
    }

    uint8_t *data_ptr   = (uint8_t *)data.buf;

    // calculate CRC
    uint16_t crc        = compute_p05_crc(data_ptr, length, data_id, offset);
    uint16_t crc_actual = littleendian_to_uint16(data_ptr + offset + P05CRC_POS);

    PyBuffer_Release(&data);

    if (crc == crc_actual) {
        Py_RETURN_TRUE;
    }
    else {
        Py_RETURN_FALSE;
    }

error:
    PyBuffer_Release(&data);
    return NULL;
}

// Method definitions
// clang-format off
static struct PyMethodDef methods[] = {
    {"e2e_p05_protect", (PyCFunction)py_e2e_p05_protect, METH_VARARGS | METH_KEYWORDS, e2e_p05_protect_doc},
    {"e2e_p05_check",   (PyCFunction)py_e2e_p05_check,   METH_VARARGS | METH_KEYWORDS, e2e_p05_check_doc},
    {NULL} // sentinel
};
// clang-format on

// Module execution function for multi-phase initialization
static int p05_exec(PyObject *module)
{
    if (PyModule_AddFunctions(module, methods) < 0) {
        return -1;
    }
    return 0;
}

// Array of slot definitions for multi-phase initialization
static PyModuleDef_Slot p05_slots[] = {{Py_mod_exec, (void *)p05_exec},
#ifdef Py_GIL_DISABLED
                                       {Py_mod_gil, Py_MOD_GIL_NOT_USED},
#endif
                                       {0, NULL}};

// Module definition
static struct PyModuleDef p05_module = {PyModuleDef_HEAD_INIT,
                                        .m_name    = "e2e.p05",
                                        .m_doc     = "",
                                        .m_size    = 0,
                                        .m_methods = NULL,
                                        .m_slots   = p05_slots};

// Module initialization function
PyMODINIT_FUNC            PyInit_p05(void) { return PyModuleDef_Init(&p05_module); }
