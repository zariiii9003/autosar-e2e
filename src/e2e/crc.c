/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdbool.h>
#include <stdint.h>

#include "crclib.h"

PyDoc_STRVAR(py_calculate_crc8_doc,
             "calculate_crc8(data: bytes, start_value: int = 0xFF, first_call: bool = True) -> int\n"
             "8-bit SAE J1850 CRC Calculation\n"
             "\n"
             ":param bytes data:\n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    which contains the data for CRC calculation\n"
             ":param int start_value:\n"
             "    First CRC of the algorithm (ignored when *first_call* is `True`).\n"
             "    In a sequence, this is expected to be the return value of the previous function call.\n"
             ":param bool first_call:\n"
             "    `True` if this is the first call of a sequence or an individual function call.\n"
             "    `False` if this is a subsequent call in a sequence.\n"
             ":return:\n"
             "    CRC value");
static PyObject *
py_calculate_crc8(PyObject *module,
                  PyObject *args,
                  PyObject *kwargs)
{
    Py_buffer data;
    unsigned char start_value = CRC8_INITIAL_VALUE;
    int first_call = true;
    static char *kwlist[] = {
        "data",
        "start_value",
        "first_call",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*|Bp:calculate_crc8",
                                     kwlist, &data, &start_value, &first_call))
    {
        return NULL;
    }

    uint8_t crc = Crc_CalculateCRC8((uint8_t*)data.buf,
                                    (uint32_t)data.len,
                                    (uint8_t)start_value,
                                    (bool)first_call);
    PyBuffer_Release(&data);

    return (PyLong_FromUnsignedLong(crc));
}

PyDoc_STRVAR(py_calculate_crc8_h2f_doc,
             "calculate_crc8_h2f(data: bytes, start_value: int = 0xFF, first_call: bool = True) -> int\n"
             "8-bit 0x2F polynomial CRC Calculation\n"
             "\n"
             ":param bytes data:\n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    which contains the data for CRC calculation\n"
             ":param int start_value:\n"
             "    First CRC of the algorithm (ignored when *first_call* is `True`).\n"
             "    In a sequence, this is expected to be the return value of the previous function call.\n"
             ":param bool first_call:\n"
             "    `True` if this is the first call of a sequence or an individual function call.\n"
             "    `False` if this is a subsequent call in a sequence.\n"
             ":return:\n"
             "    CRC value");
static PyObject *
py_calculate_crc8_h2f(PyObject *module,
                      PyObject *args,
                      PyObject *kwargs)
{
    Py_buffer data;
    unsigned char start_value = CRC8H2F_INITIAL_VALUE;
    int first_call = true;
    static char *kwlist[] = {
        "data",
        "start_value",
        "first_call",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*|Bp:calculate_crc8_h2f",
                                     kwlist, &data, &start_value, &first_call))
    {
        return NULL;
    }

    uint8_t crc = Crc_CalculateCRC8H2F((uint8_t*)data.buf,
                                       (uint32_t)data.len,
                                       (uint8_t)start_value,
                                       (bool)first_call);

    PyBuffer_Release(&data);

    return (PyLong_FromUnsignedLong(crc));
}

PyDoc_STRVAR(py_calculate_crc16_doc,
             "calculate_crc16(data: bytes, start_value: int = 0xFFFF, first_call: bool = True) -> int\n"
             "16-bit CCITT-FALSE CRC16\n"
             "\n"
             ":param bytes data:\n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    which contains the data for CRC calculation\n"
             ":param int start_value:\n"
             "    First CRC of the algorithm (ignored when *first_call* is `True`).\n"
             "    In a sequence, this is expected to be the return value of the previous function call.\n"
             ":param bool first_call:\n"
             "    `True` if this is the first call of a sequence or an individual function call.\n"
             "    `False` if this is a subsequent call in a sequence.\n"
             ":return:\n"
             "    CRC value");
static PyObject *
py_calculate_crc16(PyObject *module,
                   PyObject *args,
                   PyObject *kwargs)
{
    Py_buffer data;
    unsigned short start_value = CRC16_INITIAL_VALUE;
    int first_call = true;
    static char *kwlist[] = {
        "data",
        "start_value",
        "first_call",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*|Hp:calculate_crc16",
                                     kwlist, &data, &start_value, &first_call))
    {
        return NULL;
    }

    uint16_t crc = Crc_CalculateCRC16((uint8_t*)data.buf,
                                      (uint32_t)data.len,
                                      (uint16_t)start_value,
                                      (bool)first_call);

    PyBuffer_Release(&data);

    return (PyLong_FromUnsignedLong(crc));
}

PyDoc_STRVAR(py_calculate_crc16_arc_doc,
             "calculate_crc16_arc(data: bytes, start_value: int = 0x0000, first_call: bool = True) -> int\n"
             "16-bit 0x8005 polynomial CRC calculation\n"
             "\n"
             ":param bytes data:\n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    which contains the data for CRC calculation\n"
             ":param int start_value:\n"
             "    First CRC of the algorithm (ignored when *first_call* is `True`).\n"
             "    In a sequence, this is expected to be the return value of the previous function call.\n"
             ":param bool first_call:\n"
             "    `True` if this is the first call of a sequence or an individual function call.\n"
             "    `False` if this is a subsequent call in a sequence.\n"
             ":return:\n"
             "    CRC value");
static PyObject *
py_calculate_crc16_arc(PyObject *module,
                   PyObject *args,
                   PyObject *kwargs)
{
    Py_buffer data;
    unsigned short start_value = CRC16ARC_INITIAL_VALUE;
    int first_call = true;
    static char *kwlist[] = {
        "data",
        "start_value",
        "first_call",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*|Hp:calculate_crc16_arc",
                                     kwlist, &data, &start_value, &first_call))
    {
        return NULL;
    }

    uint16_t crc = Crc_CalculateCRC16ARC((uint8_t*)data.buf,
                                         (uint32_t)data.len,
                                         (uint16_t)start_value,
                                         (bool)first_call);
    PyBuffer_Release(&data);

    return (PyLong_FromUnsignedLong(crc));
}

PyDoc_STRVAR(py_calculate_crc32_doc,
             "calculate_crc32(data: bytes, start_value: int = 0xFFFFFFFF, first_call: bool = True) -> int\n"
             "32-bit Ethernet CRC Calculation\n"
             "\n"
             ":param bytes data:\n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    which contains the data for CRC calculation\n"
             ":param int start_value:\n"
             "    First CRC of the algorithm (ignored when *first_call* is `True`).\n"
             "    In a sequence, this is expected to be the return value of the previous function call.\n"
             ":param bool first_call:\n"
             "    `True` if this is the first call of a sequence or an individual function call.\n"
             "    `False` if this is a subsequent call in a sequence.\n"
             ":return:\n"
             "    CRC value");
static PyObject *
py_calculate_crc32(PyObject *module,
                   PyObject *args,
                   PyObject *kwargs)
{
    Py_buffer data;
    unsigned long start_value = CRC32_INITIAL_VALUE;
    int first_call = true;
    static char *kwlist[] = {
        "data",
        "start_value",
        "first_call",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*|kp:calculate_crc32",
                                     kwlist, &data, &start_value, &first_call))
    {
        return NULL;
    }

    uint32_t crc = Crc_CalculateCRC32((uint8_t*)data.buf,
                                      (uint32_t)data.len,
                                      (uint32_t)start_value,
                                      (bool)first_call);
    PyBuffer_Release(&data);

    return (PyLong_FromUnsignedLong(crc));
}

PyDoc_STRVAR(py_calculate_crc32_p4_doc,
             "calculate_crc32_p4(data: bytes, start_value: int = 0xFFFFFFFF, first_call: bool = True) -> int\n"
             "32-bit 0xF4ACFB13 polynomial CRC calculation\n"
             "\n"
             ":param bytes data:\n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    which contains the data for CRC calculation\n"
             ":param int start_value:\n"
             "    First CRC of the algorithm (ignored when *first_call* is `True`).\n"
             "    In a sequence, this is expected to be the return value of the previous function call.\n"
             ":param bool first_call:\n"
             "    `True` if this is the first call of a sequence or an individual function call.\n"
             "    `False` if this is a subsequent call in a sequence.\n"
             ":return:\n"
             "    CRC value");
static PyObject *
py_calculate_crc32_p4(PyObject *module,
                      PyObject *args,
                      PyObject *kwargs)
{
    Py_buffer data;
    unsigned long start_value = CRC32P4_INITIAL_VALUE;
    int first_call = true;
    static char *kwlist[] = {
        "data",
        "start_value",
        "first_call",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*|kp:calculate_crc32_p4",
                                     kwlist, &data, &start_value, &first_call))
    {
        return NULL;
    }

    uint32_t crc = Crc_CalculateCRC32P4((uint8_t*)data.buf,
                                        (uint32_t)data.len,
                                        (uint32_t)start_value,
                                        (bool)first_call);
    PyBuffer_Release(&data);

    return (PyLong_FromUnsignedLong(crc));
}

PyDoc_STRVAR(py_calculate_crc64_doc,
             "calculate_crc64(data: bytes, start_value: int = 0xFFFFFFFFFFFFFFFF, first_call: bool = True) -> int\n"
             "64-bit 0x42F0E1EBA9EA3693 polynomial CRC calculation\n"
             "\n"
             ":param bytes data:\n"
             "    `bytes-like object <https://docs.python.org/3/glossary.html#term-bytes-like-object>`_\n"
             "    which contains the data for CRC calculation\n"
             ":param int start_value:\n"
             "    First CRC of the algorithm (ignored when *first_call* is `True`).\n"
             "    In a sequence, this is expected to be the return value of the previous function call.\n"
             ":param bool first_call:\n"
             "    `True` if this is the first call of a sequence or an individual function call.\n"
             "    `False` if this is a subsequent call in a sequence.\n"
             ":return:\n"
             "    CRC value");
static PyObject *
py_calculate_crc64(PyObject *module,
                   PyObject *args,
                   PyObject *kwargs)
{
    Py_buffer data;
    unsigned long long start_value = CRC64_INITIAL_VALUE;
    int first_call = true;
    static char *kwlist[] = {
        "data",
        "start_value",
        "first_call",
        NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "y*|Kp:calculate_crc64",
                                     kwlist, &data, &start_value, &first_call))
    {
        return NULL;
    }

    uint64_t crc = Crc_CalculateCRC64((uint8_t*)data.buf,
                                      (uint32_t)data.len,
                                      (uint64_t)start_value,
                                      (bool)first_call);
    PyBuffer_Release(&data);

    return (PyLong_FromUnsignedLongLong(crc));
}

// Method definitions
static struct PyMethodDef methods[] = {
    {"calculate_crc8",      (PyCFunction)py_calculate_crc8,      METH_VARARGS | METH_KEYWORDS, py_calculate_crc8_doc     },
    {"calculate_crc8_h2f",  (PyCFunction)py_calculate_crc8_h2f,  METH_VARARGS | METH_KEYWORDS, py_calculate_crc8_h2f_doc },
    {"calculate_crc16",     (PyCFunction)py_calculate_crc16,     METH_VARARGS | METH_KEYWORDS, py_calculate_crc16_doc    },
    {"calculate_crc16_arc", (PyCFunction)py_calculate_crc16_arc, METH_VARARGS | METH_KEYWORDS, py_calculate_crc16_arc_doc},
    {"calculate_crc32",     (PyCFunction)py_calculate_crc32,     METH_VARARGS | METH_KEYWORDS, py_calculate_crc32_doc    },
    {"calculate_crc32_p4",  (PyCFunction)py_calculate_crc32_p4,  METH_VARARGS | METH_KEYWORDS, py_calculate_crc32_p4_doc },
    {"calculate_crc64",     (PyCFunction)py_calculate_crc64,     METH_VARARGS | METH_KEYWORDS, py_calculate_crc64_doc    },
    {NULL}  // sentinel
};

static int
_AddUnsignedIntConstant(PyObject *module, const char *name, uint64_t value)
{
    PyObject *obj = PyLong_FromUnsignedLongLong(value);
    if (PyModule_AddObject(module, name, obj) < 0) {
        Py_XDECREF(obj);
        return -1;
    }
    return 0;
}

#define _AddUnsignedIntMacro(m, c) _AddUnsignedIntConstant(m, #c, c)

// Module execution function
static int crc_exec(PyObject *module)
{
    // Add constants to module
    _AddUnsignedIntMacro(module, CRC8_INITIAL_VALUE);
    _AddUnsignedIntMacro(module, CRC8_XOR_VALUE);
    _AddUnsignedIntMacro(module, CRC8_CHECK);
    _AddUnsignedIntMacro(module, CRC8_MAGIC_CHECK);

    _AddUnsignedIntMacro(module, CRC8H2F_INITIAL_VALUE);
    _AddUnsignedIntMacro(module, CRC8H2F_XOR_VALUE);
    _AddUnsignedIntMacro(module, CRC8H2F_CHECK);
    _AddUnsignedIntMacro(module, CRC8H2F_MAGIC_CHECK);

    _AddUnsignedIntMacro(module, CRC16_INITIAL_VALUE);
    _AddUnsignedIntMacro(module, CRC16_XOR_VALUE);
    _AddUnsignedIntMacro(module, CRC16_CHECK);
    _AddUnsignedIntMacro(module, CRC16_MAGIC_CHECK);

    _AddUnsignedIntMacro(module, CRC16ARC_INITIAL_VALUE);
    _AddUnsignedIntMacro(module, CRC16ARC_XOR_VALUE);
    _AddUnsignedIntMacro(module, CRC16ARC_CHECK);
    _AddUnsignedIntMacro(module, CRC16ARC_MAGIC_CHECK);

    _AddUnsignedIntMacro(module, CRC32_INITIAL_VALUE);
    _AddUnsignedIntMacro(module, CRC32_XOR_VALUE);
    _AddUnsignedIntMacro(module, CRC32_CHECK);
    _AddUnsignedIntMacro(module, CRC32_MAGIC_CHECK);

    _AddUnsignedIntMacro(module, CRC32P4_INITIAL_VALUE);
    _AddUnsignedIntMacro(module, CRC32P4_XOR_VALUE);
    _AddUnsignedIntMacro(module, CRC32P4_CHECK);
    _AddUnsignedIntMacro(module, CRC32P4_MAGIC_CHECK);

    _AddUnsignedIntMacro(module, CRC64_INITIAL_VALUE);
    _AddUnsignedIntMacro(module, CRC64_XOR_VALUE);
    _AddUnsignedIntMacro(module, CRC64_CHECK);
    _AddUnsignedIntMacro(module, CRC64_MAGIC_CHECK);

    // Register methods
    if (PyModule_AddFunctions(module, methods) < 0) {
        return -1;
    }

    return 0;
}

// Slots array with version check for Py_mod_gil
static PyModuleDef_Slot crc_slots[] = {
    {Py_mod_exec, (void *)crc_exec},
#ifdef Py_GIL_DISABLED
    {Py_mod_gil, Py_MOD_GIL_NOT_USED},
#endif
    {0, NULL}
};

// Module definition
static struct PyModuleDef crc_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "e2e.crc",
    .m_doc = "",
    .m_size = 0,
    .m_methods = NULL,       // Methods added dynamically
    .m_slots = crc_slots
};

// Init function
PyMODINIT_FUNC PyInit_crc(void)
{
    return PyModuleDef_Init(&crc_module);
}
