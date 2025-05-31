/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdbool.h>
#include <stdint.h>

#include "crclib.h"

#define E2E_P01_DATAID_BOTH   0x0
#define E2E_P01_DATAID_ALT    0x1
#define E2E_P01_DATAID_LOW    0x2
#define E2E_P01_DATAID_NIBBLE 0x3

uint8_t compute_p01_crc(uint8_t *data_ptr,
                        uint16_t length,
                        uint16_t data_id,
                        uint16_t data_id_mode,
                        uint8_t  counter,
                        uint16_t crc_offset)
{
    uint8_t data_id_lo_byte = (uint8_t)data_id;
    uint8_t data_id_hi_byte = (uint8_t)(data_id >> 8);
    uint8_t crc             = 0x00u;

    switch (data_id_mode) {
        case E2E_P01_DATAID_BOTH:
            crc = Crc_CalculateCRC8(&data_id_lo_byte, 1u, CRC8_XOR_VALUE, false);
            crc = Crc_CalculateCRC8(&data_id_hi_byte, 1u, crc, false);
            break;

        case E2E_P01_DATAID_LOW:
            crc = Crc_CalculateCRC8(&data_id_lo_byte, 1u, CRC8_XOR_VALUE, false);
            break;

        case E2E_P01_DATAID_ALT:
            if (counter % 2 == 0) {
                crc = Crc_CalculateCRC8(&data_id_lo_byte, 1u, CRC8_XOR_VALUE, false);
            }
            else {
                crc = Crc_CalculateCRC8(&data_id_hi_byte, 1u, CRC8_XOR_VALUE, false);
            }
            break;

        case E2E_P01_DATAID_NIBBLE:
            crc             = Crc_CalculateCRC8(&data_id_lo_byte, 1u, CRC8_XOR_VALUE, false);
            data_id_hi_byte = 0;
            crc             = Crc_CalculateCRC8(&data_id_hi_byte, 1u, crc, false);
            break;
    }

    if (crc_offset >= 8) {
        // compute crc over data before the crc byte
        crc = Crc_CalculateCRC8(data_ptr, (crc_offset >> 3), crc, false);
    }

    if ((crc_offset >> 3) < length) {
        // compute crc over area after crc byte
        unsigned short start_byte = (crc_offset >> 3) + 1;
        unsigned short byte_count = length - (crc_offset >> 3);
        crc                       = Crc_CalculateCRC8(data_ptr + start_byte, byte_count, crc, false);
    }

    // write CRC to data
    return crc ^ CRC8_XOR_VALUE;
}

// clang-format off
PyDoc_STRVAR(e2e_p01_protect_doc,
             "e2e_p01_protect(data: bytearray, length: int, data_id: int, *, data_id_mode: int = E2E_P01_DATAID_BOTH, increment_counter: bool = True) -> None \n"
             "Calculate CRC inplace according to AUTOSAR E2E Profile 1. \n"
             "\n"
             ":param bytearray data: \n"
             "    Mutable `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    starting with the CRC byte. This CRC byte will be updated inplace. \n"
             ":param int length: \n"
             "    Number of data bytes which are considered for CRC calculation. `length` must fulfill \n"
             "    the following condition: ``1 <= length <= len(data) - 1`` \n"
             ":param int data_id: \n"
             "    A unique identifier which is used to protect against masquerading. The `data_id` is a 16bit unsigned integer. \n"
             ":param int data_id_mode: \n"
             "    This attribute describes the inclusion mode that is used to include the `data_id`. The possible inclusion modes are\n"
             "    :attr:`~e2e.p01.E2E_P01_DATAID_BOTH`, :attr:`~e2e.p01.E2E_P01_DATAID_ALT`, :attr:`~e2e.p01.E2E_P01_DATAID_LOW` \n"
             "    and :attr:`~e2e.p01.E2E_P01_DATAID_NIBBLE`.\n"
             ":param bool increment_counter: \n"
             "    If `True` the counter in byte 1 will be incremented before calculating the CRC. \n");
// clang-format on
static PyObject *py_e2e_p01_protect(PyObject *module, PyObject *args, PyObject *kwargs)
{
    Py_buffer      data;
    unsigned short length;
    unsigned short data_id;
    unsigned short data_id_mode          = E2E_P01_DATAID_BOTH;
    int            increment_counter     = true;
    unsigned short crc_offset            = 0;
    unsigned short counter_offset        = 8;
    unsigned short data_id_nibble_offset = 12;

    static char   *kwlist[] = {"data", "length", "data_id", "data_id_mode", "increment_counter", NULL};

    if (!PyArg_ParseTupleAndKeywords(args,
                                     kwargs,
                                     "y*HH|$Hp:e2e_p01_protect",
                                     kwlist,
                                     &data,
                                     &length,
                                     &data_id,
                                     &data_id_mode,
                                     &increment_counter)) {
        return NULL;
    }

    if (data.readonly) {
        PyErr_SetString(PyExc_ValueError,
                        "\"data\" must be mutable. Use a bytearray or any "
                        "object that implements the buffer protocol.");
        goto error;
    }
    if (data.len <= 2) {
        PyErr_SetString(PyExc_ValueError, "The length of bytearray \"data\" must be greater than 2.");
        goto error;
    }
    if (length < 1 || length > data.len - 1) {
        PyErr_SetString(PyExc_ValueError,
                        "Parameter \"length\" must fulfill the following "
                        "condition: 1 <= length <= len(data) - 1.");
        goto error;
    }

    uint8_t *data_ptr = (uint8_t *)data.buf;

    // The counter goes either into low nibble or high nibble of data
    uint8_t  counter  = 0;
    if (counter_offset % 8 == 0) {
        counter = (*(data_ptr + (counter_offset >> 3)) & 0x0F);
        if (increment_counter) {
            counter                             = (counter + 1) % 0x0F; // alive counter in range 0-14
            *(data_ptr + (counter_offset >> 3)) = (*(data_ptr + (counter_offset >> 3)) & 0xF0) | counter;
        }
    }
    else {
        counter = (*(data_ptr + (counter_offset >> 3)) & 0xF0) >> 4;
        if (increment_counter) {
            counter = (counter + 1) % 0x0F; // alive counter in range 0-14
            *(data_ptr + (counter_offset >> 3)) =
                (*(data_ptr + (counter_offset >> 3)) & 0x0F) | ((counter << 4) & 0xF0);
        }
    }

    if (data_id_mode == E2E_P01_DATAID_NIBBLE) {
        // Write the low nibble of high byte of data_id
        if (data_id_nibble_offset % 8 == 0) {
            *(data_ptr + (data_id_nibble_offset >> 3)) =
                (*(data_ptr + (data_id_nibble_offset >> 3)) & 0xF0) | ((data_id >> 8) & 0x0F);
        }
        else {
            *(data_ptr + (data_id_nibble_offset >> 3)) =
                (*(data_ptr + (data_id_nibble_offset >> 3)) & 0x0F) | ((data_id >> 4) & 0xF0);
        }
    }

    // calculate CRC
    uint8_t crc = compute_p01_crc(data_ptr, length, data_id, data_id_mode, counter, crc_offset);

    // write CRC to data
    *(data_ptr + (crc_offset / 8)) = crc;

    PyBuffer_Release(&data);
    Py_RETURN_NONE;

error:
    PyBuffer_Release(&data);
    return NULL;
}

// clang-format off
PyDoc_STRVAR(e2e_p01_check_doc,
             "e2e_p01_check(data: bytearray, length: int, data_id: int, *, data_id_mode: int = E2E_P01_DATAID_BOTH) -> bool \n"
             "Return ``True`` if CRC is correct according to AUTOSAR E2E Profile 1. \n"
             "\n"
             ":param bytearray data: \n"
             "    Mutable `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    starting with the CRC byte. This CRC byte will be updated inplace. \n"
             ":param int length: \n"
             "    Number of data bytes which are considered for CRC calculation. `length` must fulfill \n"
             "    the following condition: ``1 <= length < len(data)`` \n"
             ":param int data_id: \n"
             "    A unique identifier which is used to protect against masquerading. The `data_id` is a 16bit unsigned integer. \n"
             ":param int data_id_mode: \n"
             "    Mode of the data ID. Possible values are \n"
             "    :attr:`~e2e.p01.E2E_P01_DATAID_BOTH`, :attr:`~e2e.p01.E2E_P01_DATAID_ALT`, :attr:`~e2e.p01.E2E_P01_DATAID_LOW` \n"
             "    and :attr:`~e2e.p01.E2E_P01_DATAID_NIBBLE`.\n"
             ":return:\n"
             "    `True` if CRC is valid, otherwise return `False`");
// clang-format on

static PyObject *py_e2e_p01_check(PyObject *module, PyObject *args, PyObject *kwargs)
{
    Py_buffer      data;
    unsigned short length;
    unsigned short data_id;
    unsigned short data_id_mode          = E2E_P01_DATAID_BOTH;
    unsigned short crc_offset            = 0;
    unsigned short counter_offset        = 8;
    unsigned short data_id_nibble_offset = 12;

    static char   *kwlist[]              = {"data", "length", "data_id", "data_id_mode", NULL};

    if (!PyArg_ParseTupleAndKeywords(args,
                                     kwargs,
                                     "y*HH|$H:e2e_p01_check",
                                     kwlist,
                                     &data,
                                     &length,
                                     &data_id,
                                     &data_id_mode)) {
        return NULL;
    }

    if (data.readonly) {
        PyErr_SetString(PyExc_ValueError,
                        "\"data\" must be mutable. Use a bytearray or any "
                        "object that implements the buffer protocol.");
        goto error;
    }
    if (data.len < 2) {
        PyErr_SetString(PyExc_ValueError, "The length of bytearray \"data\" must be greater than 1.");
        goto error;
    }
    if (length < 1 || length > data.len - 1) {
        PyErr_SetString(PyExc_ValueError,
                        "Parameter \"length\" must fulfill the following "
                        "condition: 1 <= length < len(data).");
        goto error;
    }

    uint8_t *data_ptr = (uint8_t *)data.buf;

    // Check the alive counter value. The counter is located either in the low
    // or the high nibble of data
    uint8_t  counter  = 0u;
    if (counter_offset % 8u == 0u) {
        counter = (*(data_ptr + (counter_offset >> 3)) & 0x0F);
    }
    else {
        counter = (*(data_ptr + (counter_offset >> 3)) & 0xF0) >> 4;
    }
    if (counter > 14u) {
        // counter must be in 0-14
        goto return_false;
    }

    // check the data_id nibble if it is sent excplicitely
    if (data_id_mode == E2E_P01_DATAID_NIBBLE) {
        uint8_t data_id_nibble = *(data_ptr + (data_id_nibble_offset >> 3)) >> 4;
        if (data_id_nibble != ((uint8_t)(data_id >> 8) & 0x0F))
            goto return_false;
    }

    // check CRC
    uint8_t crc_in_data = *(data_ptr + (crc_offset / 8));
    uint8_t calculated_crc =
        compute_p01_crc(data_ptr, length, data_id, data_id_mode, counter, crc_offset);
    if (crc_in_data != calculated_crc)
        goto return_false;

    PyBuffer_Release(&data);
    Py_RETURN_TRUE;

return_false:
    PyBuffer_Release(&data);
    Py_RETURN_FALSE;

error:
    PyBuffer_Release(&data);
    return NULL;
}

// Method definitions
// clang-format off
static struct PyMethodDef methods[] = {
    {"e2e_p01_protect", (PyCFunction)py_e2e_p01_protect, METH_VARARGS | METH_KEYWORDS, e2e_p01_protect_doc},
    {"e2e_p01_check",   (PyCFunction)py_e2e_p01_check,   METH_VARARGS | METH_KEYWORDS, e2e_p01_check_doc},
    {NULL} // sentinel
};
// clang-format on

static int _AddUnsignedIntConstant(PyObject *module, const char *name, uint64_t value)
{
    PyObject *obj = PyLong_FromUnsignedLongLong(value);
    if (PyModule_AddObject(module, name, obj) < 0) {
        Py_XDECREF(obj);
        return -1;
    }
    return 0;
}

#define _AddUnsignedIntMacro(m, c) _AddUnsignedIntConstant(m, #c, c)

// Module execution function for multi-phase initialization
static int p01_exec(PyObject *module)
{
    // Add constants
    _AddUnsignedIntMacro(module, E2E_P01_DATAID_BOTH);
    _AddUnsignedIntMacro(module, E2E_P01_DATAID_ALT);
    _AddUnsignedIntMacro(module, E2E_P01_DATAID_LOW);
    _AddUnsignedIntMacro(module, E2E_P01_DATAID_NIBBLE);

    // Register methods dynamically
    if (PyModule_AddFunctions(module, methods) < 0) {
        return -1;
    }

    return 0;
}

// Array of slot definitions for multi-phase initialization
static PyModuleDef_Slot p01_slots[] = {{Py_mod_exec, (void *)p01_exec},
#ifdef Py_GIL_DISABLED
                                       {Py_mod_gil, Py_MOD_GIL_NOT_USED},
#endif
                                       {0, NULL}};

// Module definition using multi-phase init
static struct PyModuleDef p01_module = {PyModuleDef_HEAD_INIT,
                                        .m_name    = "e2e.p01",
                                        .m_doc     = "",
                                        .m_size    = 0,
                                        .m_methods = NULL,
                                        .m_slots   = p01_slots};

// Init function
PyMODINIT_FUNC            PyInit_p01(void) { return PyModuleDef_Init(&p01_module); }
