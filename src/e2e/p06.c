/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "crclib.h"
#include "util.h"

#define P06LENGTH_POS    2
#define P06LENGTH_LEN    2
#define P06COUNTER_POS   4
#define P06COUNTER_LEN   1
#define P06DATAID_POS    0
#define P06DATAID_LEN    0
#define P06CRC_POS       0
#define P06CRC_LEN       2
#define P06CALCULATE_CRC Crc_CalculateCRC16

#define P06HEADER_LEN    (P06CRC_LEN + P06LENGTH_LEN + P06COUNTER_LEN)

uint16_t compute_p06_crc(uint8_t *data_ptr, uint16_t length, uint16_t data_id, uint16_t offset)
{
    uint16_t crc;
    uint8_t  data_id_lo_byte = (uint8_t)data_id;
    uint8_t  data_id_hi_byte = (uint8_t)(data_id >> 8);
    if (offset > 0) {
        crc = P06CALCULATE_CRC(data_ptr, offset, CRC16_INITIAL_VALUE, true);
        crc = P06CALCULATE_CRC(&data_ptr[offset + P06LENGTH_POS],
                               length - offset - P06LENGTH_POS,
                               crc,
                               false);
    }
    else {
        crc = P06CALCULATE_CRC(&data_ptr[P06LENGTH_POS],
                               length - P06LENGTH_POS,
                               CRC16_INITIAL_VALUE,
                               true);
    }
    crc = P06CALCULATE_CRC(&data_id_hi_byte, 1, crc, false);
    crc = P06CALCULATE_CRC(&data_id_lo_byte, 1, crc, false);

    return crc;
}

// clang-format off
PyDoc_STRVAR(e2e_p06_protect_doc,
             "e2e_p06_protect(data: bytearray, length: int, data_id: int, *, offset: int = 0, increment_counter: bool = True) -> None \n"
             "Calculate CRC inplace according to AUTOSAR E2E Profile 6. \n"
             "\n"
             ":param bytearray data: \n"
             "    Mutable `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_.\n"
             ":param int length: \n"
             "    Number of data bytes which are considered for CRC calculation. `length` must fulfill \n"
             "    the following condition: ``5 <= length <= len(data)`` \n"
             ":param int data_id: \n"
             "    A unique identifier which is used to protect against masquerading. The `data_id` is a 16bit unsigned integer. \n"
             ":param int offset: \n"
             "    Byte offset of the E2E header. \n"
             ":param bool increment_counter: \n"
             "    If `True` the counter will be incremented before calculating the CRC. \n");
// clang-format on
static PyObject *py_e2e_p06_protect(PyObject *module, PyObject *args, PyObject *kwargs)
{
    Py_buffer      data;
    unsigned short length;
    unsigned short data_id;
    unsigned short offset    = 0;
    int            increment = true;

    static char   *kwlist[]  = {"data", "length", "data_id", "offset", "increment_counter", NULL};

    if (!PyArg_ParseTupleAndKeywords(args,
                                     kwargs,
                                     "y*HH|$Hp:e2e_p06_protect",
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
    if (data.len < P06HEADER_LEN) {
        PyErr_SetString(PyExc_ValueError,
                        "The length of bytearray \"data\" must be greater than "
                        "or equal to 20.");
        goto error;
    }
    if (length < P06HEADER_LEN || length > data.len) {
        PyErr_SetString(PyExc_ValueError,
                        "Parameter \"length\" must fulfill the following "
                        "condition: 5 <= length <= len(data).");
        goto error;
    }
    if (offset > data.len - P06HEADER_LEN) {
        PyErr_SetString(PyExc_ValueError, "Argument \"offset\" invalid.");
        goto error;
    }

    uint8_t *data_ptr = (uint8_t *)data.buf;

    // write length
    uint16_to_bigendian(data_ptr + offset + P06LENGTH_POS, length);

    // increment counter
    if (increment) {
        uint8_t counter = data_ptr[offset + P06COUNTER_POS];
        counter += 1;
        data_ptr[offset + P06COUNTER_POS] = counter;
    }

    // calculate CRC
    uint16_t crc = compute_p06_crc(data_ptr, length, data_id, offset);
    uint16_to_bigendian(data_ptr + offset + P06CRC_POS, crc);

    PyBuffer_Release(&data);

    Py_RETURN_NONE;

error:
    PyBuffer_Release(&data);
    return NULL;
}

// clang-format off
PyDoc_STRVAR(e2e_p06_check_doc,
             "e2e_p06_check(data: bytes, length: int, data_id: int, *, offset: int = 0) -> bool \n"
             "Return ``True`` if CRC is correct according to AUTOSAR E2E Profile 6. \n"
             "\n"
             ":param data: \n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_. \n"
             ":param length: \n"
             "    Data byte count over which the CRC must be calculated. `length` must fulfill \n"
             "    the following condition: ``5 <= length <= len(data)`` \n"
             ":param int data_id: \n"
             "    A unique identifier which is used to protect against masquerading. The `data_id` is a 16bit unsigned integer. \n"
             ":param int offset: \n"
             "    Byte offset of the E2E header. \n"
             ":return:\n"
             "    `True` if CRC is valid, otherwise return `False`");
// clang-format on
static PyObject *py_e2e_p06_check(PyObject *module, PyObject *args, PyObject *kwargs)
{
    Py_buffer     data;
    unsigned long length;
    unsigned long data_id;
    unsigned long offset   = 0;

    static char  *kwlist[] = {"data", "length", "data_id", "offset", NULL};

    if (!PyArg_ParseTupleAndKeywords(args,
                                     kwargs,
                                     "y*kk|$k:e2e_p06_check",
                                     kwlist,
                                     &data,
                                     &length,
                                     &data_id,
                                     &offset)) {
        return NULL;
    }
    if (data.len < P06HEADER_LEN) {
        PyErr_SetString(PyExc_ValueError,
                        "The length of bytearray \"data\" must be greater or equal to 5.");
        goto error;
    }
    if (length < P06HEADER_LEN || length > data.len) {
        PyErr_SetString(PyExc_ValueError,
                        "Parameter \"length\" must fulfill the following "
                        "condition: 5 <= length <= len(data).");
        goto error;
    }
    if (offset > data.len - P06HEADER_LEN) {
        PyErr_SetString(PyExc_ValueError, "Argument \"offset\" invalid.");
        goto error;
    }

    uint8_t *data_ptr      = (uint8_t *)data.buf;

    // read length
    uint16_t length_actual = bigendian_to_uint16(data_ptr + offset + P06LENGTH_POS);

    // read crc
    uint16_t crc_actual    = bigendian_to_uint16(data_ptr + offset + P06CRC_POS);

    // calculate CRC
    uint16_t crc           = compute_p06_crc(data_ptr, length, data_id, offset);

    PyBuffer_Release(&data);

    if ((length_actual == length) && (crc_actual == crc)) {
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
    {"e2e_p06_protect", (PyCFunction)py_e2e_p06_protect, METH_VARARGS | METH_KEYWORDS, e2e_p06_protect_doc},
    {"e2e_p06_check",   (PyCFunction)py_e2e_p06_check,   METH_VARARGS | METH_KEYWORDS, e2e_p06_check_doc},
    {NULL} // sentinel
};
// clang-format on

// Module execution function for multi-phase initialization
static int p06_exec(PyObject *module)
{
    if (PyModule_AddFunctions(module, methods) < 0) {
        return -1;
    }
    return 0;
}

// Array of slot definitions for multi-phase initialization
static PyModuleDef_Slot p06_slots[] = {{Py_mod_exec, (void *)p06_exec},
#ifdef Py_GIL_DISABLED
                                       {Py_mod_gil, Py_MOD_GIL_NOT_USED},
#endif
                                       {0, NULL}};

// Module definition
static struct PyModuleDef p06_module = {
    PyModuleDef_HEAD_INIT,
    .m_name    = "e2e.p06",
    .m_doc     = "",
    .m_size    = 0,
    .m_methods = NULL,
    .m_slots   = p06_slots,
};

// Module init function
PyMODINIT_FUNC PyInit_p06(void) { return PyModuleDef_Init(&p06_module); }
