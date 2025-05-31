/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdbool.h>
#include <stdint.h>

#include "crclib.h"
#include "util.h"

#define P07LENGTH_POS  8u
#define P07LENGTH_LEN  4u
#define P07COUNTER_POS 12u
#define P07COUNTER_LEN 4u
#define P07DATAID_POS  16u
#define P07DATAID_LEN  4u
#define P07CRC_POS     0u
#define P07CRC_LEN     8u

#define P07HEADER_LEN  (P07CRC_LEN + P07LENGTH_LEN + P07COUNTER_LEN + P07DATAID_LEN)

uint64_t compute_p07_crc(uint8_t *data_ptr, uint32_t length, uint32_t offset)
{
    uint64_t crc;

    // bytes before crc bytes
    uint32_t crc_offset = (uint32_t)(offset + P07CRC_POS);
    crc                 = Crc_CalculateCRC64(data_ptr, crc_offset, CRC64_INITIAL_VALUE, true);

    // bytes after crc bytes, if any
    if (offset + P07CRC_POS + P07CRC_LEN < length) {
        uint32_t second_part_offset = offset + P07CRC_POS + P07CRC_LEN;
        uint8_t *second_part_ptr    = data_ptr + second_part_offset;
        uint32_t second_part_len    = length - (uint32_t)(offset + P07CRC_POS + P07CRC_LEN);
        crc                         = Crc_CalculateCRC64(second_part_ptr, second_part_len, crc, false);
    }
    return crc;
}

// clang-format off
PyDoc_STRVAR(e2e_p07_protect_doc,
             "e2e_p07_protect(data: bytearray, length: int, data_id: int, *, offset: int = 0, increment_counter: bool = True) -> None \n"
             "Calculate CRC inplace according to AUTOSAR E2E Profile 7. \n"
             "\n"
             ":param bytearray data: \n"
             "    Mutable `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_.\n"
             ":param int length: \n"
             "    Number of data bytes which are considered for CRC calculation. `length` must fulfill \n"
             "    the following condition: ``20 <= length <= len(data)`` \n"
             ":param int data_id: \n"
             "    A unique identifier which is used to protect against masquerading. The `data_id` is a 32bit unsigned integer. \n"
             ":param int offset: \n"
             "    Byte offset of the E2E header. \n"
             ":param bool increment_counter: \n"
             "    If `True` the counter will be incremented before calculating the CRC. \n");
// clang-format on
static PyObject *py_e2e_p07_protect(PyObject *module, PyObject *args, PyObject *kwargs)
{
    Py_buffer     data;
    unsigned long length;
    unsigned long data_id;
    unsigned long offset    = 0;
    int           increment = true;

    static char  *kwlist[]  = {"data", "length", "data_id", "offset", "increment_counter", NULL};

    if (!PyArg_ParseTupleAndKeywords(args,
                                     kwargs,
                                     "y*kk|$kp:e2e_p07_protect",
                                     kwlist,
                                     &data,
                                     &length,
                                     &data_id,
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
    if (data.len < P07HEADER_LEN) {
        PyErr_SetString(PyExc_ValueError,
                        "The length of bytearray \"data\" must be greater than "
                        "or equal to 20.");
        goto error;
    }
    if (length < P07HEADER_LEN || length > data.len) {
        PyErr_SetString(PyExc_ValueError,
                        "Parameter \"length\" must fulfill the following "
                        "condition: 20 <= length <= len(data).");
        goto error;
    }
    if (offset > data.len - P07HEADER_LEN) {
        PyErr_SetString(PyExc_ValueError, "Argument \"offset\" invalid.");
        goto error;
    }

    uint8_t *data_ptr = (uint8_t *)data.buf;

    // write length
    uint32_to_bigendian(data_ptr + offset + P07LENGTH_POS, length);

    // increment counter
    if (increment) {
        uint32_t counter = bigendian_to_uint32(data_ptr + offset + P07COUNTER_POS);
        counter += 1;
        uint32_to_bigendian(data_ptr + offset + P07COUNTER_POS, counter);
    }

    // write data_id
    uint32_to_bigendian(data_ptr + offset + P07DATAID_POS, data_id);

    // calculate CRC
    uint64_t crc = compute_p07_crc(data_ptr, length, offset);
    uint64_to_bigendian(data_ptr + offset + P07CRC_POS, crc);

    PyBuffer_Release(&data);

    Py_RETURN_NONE;

error:
    PyBuffer_Release(&data);
    return NULL;
}

// clang-format off
PyDoc_STRVAR(e2e_p07_check_doc,
             "e2e_p07_check(data: bytes, length: int, data_id: int, *, offset: int = 0) -> bool \n"
             "Return ``True`` if CRC is correct according to AUTOSAR E2E Profile 7. \n"
             "\n"
             ":param data: \n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_. \n"
             ":param length: \n"
             "    Data byte count over which the CRC must be calculated. `length` must fulfill \n"
             "    the following condition: ``20 <= length <= len(data)`` \n"
             ":param int data_id: \n"
             "    A unique identifier which is used to protect against masquerading. The `data_id` is a 32bit unsigned integer. \n"
             ":param int offset: \n"
             "    Byte offset of the E2E header. \n"
             ":return:\n"
             "    `True` if CRC is valid, otherwise return `False`");
// clang-format on
static PyObject *py_e2e_p07_check(PyObject *module, PyObject *args, PyObject *kwargs)
{
    Py_buffer     data;
    unsigned long length;
    unsigned long data_id;
    unsigned long offset   = 0;

    static char  *kwlist[] = {"data", "length", "data_id", "offset", NULL};

    if (!PyArg_ParseTupleAndKeywords(args,
                                     kwargs,
                                     "y*kk|$k:e2e_p07_check",
                                     kwlist,
                                     &data,
                                     &length,
                                     &data_id,
                                     &offset)) {
        return NULL;
    }
    if (data.len < P07HEADER_LEN) {
        PyErr_SetString(PyExc_ValueError,
                        "The length of bytearray \"data\" must be greater or equal to 20.");
        goto error;
    }
    if (length < P07HEADER_LEN || length > data.len) {
        PyErr_SetString(PyExc_ValueError,
                        "Parameter \"length\" must fulfill the following "
                        "condition: 20 <= length <= len(data).");
        goto error;
    }
    if (offset > data.len - P07HEADER_LEN) {
        PyErr_SetString(PyExc_ValueError, "Argument \"offset\" invalid.");
        goto error;
    }

    uint8_t *data_ptr       = (uint8_t *)data.buf;

    // read length
    uint32_t length_actual  = bigendian_to_uint32(data_ptr + offset + P07LENGTH_POS);

    // read data_id
    uint32_t data_id_actual = bigendian_to_uint32(data_ptr + offset + P07DATAID_POS);

    // read crc
    uint64_t crc_actual     = bigendian_to_uint64(data_ptr + offset + P07CRC_POS);

    // calculate CRC
    uint64_t crc            = compute_p07_crc(data_ptr, length, offset);

    PyBuffer_Release(&data);

    if ((length_actual == length) && (data_id_actual == data_id) && (crc_actual == crc)) {
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
    {"e2e_p07_protect", (PyCFunction)py_e2e_p07_protect, METH_VARARGS | METH_KEYWORDS, e2e_p07_protect_doc},
    {"e2e_p07_check",   (PyCFunction)py_e2e_p07_check,   METH_VARARGS | METH_KEYWORDS, e2e_p07_check_doc},
    {NULL} // sentinel
};
// clang-format on

// Module execution function for multi-phase initialization
static int p07_exec(PyObject *module)
{
    if (PyModule_AddFunctions(module, methods) < 0) {
        return -1;
    }
    return 0;
}

// Array of slot definitions for multi-phase initialization
static PyModuleDef_Slot p07_slots[] = {{Py_mod_exec, (void *)p07_exec},
#ifdef Py_GIL_DISABLED
                                       {Py_mod_gil, Py_MOD_GIL_NOT_USED},
#endif
                                       {0, NULL}};

// Module definition
static struct PyModuleDef p07_module = {
    PyModuleDef_HEAD_INIT,
    .m_name    = "e2e.p07",
    .m_doc     = "",
    .m_size    = 0,
    .m_methods = NULL,
    .m_slots   = p07_slots,
};

// Module init function
PyMODINIT_FUNC PyInit_p07(void) { return PyModuleDef_Init(&p07_module); }
