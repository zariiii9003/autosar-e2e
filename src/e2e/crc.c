/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdbool.h>
#include <stdint.h>

#include "crc.h"

uint8_t Crc_CalculateCRC8(
    const uint8_t *Crc_DataPtr,
    uint32_t Crc_Length,
    uint8_t Crc_StartValue8,
    bool Crc_IsFirstCall)
{
    uint8_t crc;

    if (Crc_IsFirstCall)
    {
        crc = CRC8_INITIAL_VALUE;
    }
    else
    {
        crc = (CRC8_XOR_VALUE ^ Crc_StartValue8);
    }

    for (size_t i = 0; i < Crc_Length; ++i)
    {
        crc = CRC8_TABLE[crc ^ Crc_DataPtr[i]];
    }

    return (crc ^ CRC8_XOR_VALUE);
}

uint8_t Crc_CalculateCRC8H2F(
    const uint8_t *Crc_DataPtr,
    uint32_t Crc_Length,
    uint8_t Crc_StartValue8H2F,
    bool Crc_IsFirstCall)
{
    uint8_t crc;

    if (Crc_IsFirstCall)
    {
        crc = CRC8H2F_INITIAL_VALUE;
    }
    else
    {
        crc = (CRC8H2F_XOR_VALUE ^ Crc_StartValue8H2F);
    }

    for (size_t i = 0; i < Crc_Length; ++i)
    {
        crc = CRC8H2F_TABLE[crc ^ Crc_DataPtr[i]];
    }

    return (crc ^ CRC8H2F_XOR_VALUE);
}

uint16_t Crc_CalculateCRC16(
    const uint8_t *Crc_DataPtr,
    uint32_t Crc_Length,
    uint16_t Crc_StartValue16,
    bool Crc_IsFirstCall)
{
    uint16_t crc;

    if (Crc_IsFirstCall)
    {
        crc = CRC16_INITIAL_VALUE;
    }
    else
    {
        crc = (CRC16_XOR_VALUE ^ Crc_StartValue16);
    }

    for (size_t i = 0; i < Crc_Length; ++i)
    {
        crc = (crc << 8) ^ CRC16_TABLE[((crc >> 8) ^ Crc_DataPtr[i]) & 0xFFU];
    }
    return (crc ^ CRC16_XOR_VALUE);
}

uint16_t Crc_CalculateCRC16ARC(
    const uint8_t *Crc_DataPtr,
    uint32_t Crc_Length,
    uint16_t Crc_StartValue16,
    bool Crc_IsFirstCall)
{
    uint16_t crc;

    if (Crc_IsFirstCall)
    {
        crc = CRC16ARC_INITIAL_VALUE;
    }
    else
    {
        crc = (CRC16ARC_XOR_VALUE ^ Crc_StartValue16);
    }

    for (size_t i = 0; i < Crc_Length; ++i)
    {
        crc = (crc >> 8) ^ CRC16ARC_TABLE[(crc ^ Crc_DataPtr[i]) & 0xFFu];
    }
    return (crc ^ CRC16ARC_XOR_VALUE);
}

uint32_t Crc_CalculateCRC32(
    const uint8_t *Crc_DataPtr,
    uint32_t Crc_Length,
    uint32_t Crc_StartValue32,
    bool Crc_IsFirstCall)
{
    uint32_t crc;

    if (Crc_IsFirstCall)
    {
        crc = CRC32_INITIAL_VALUE;
    }
    else
    {
        crc = (CRC32_XOR_VALUE ^ Crc_StartValue32);
    }

    for (size_t i = 0; i < Crc_Length; ++i)
    {
        crc ^= (uint32_t)Crc_DataPtr[i];
        crc = (crc >> 8u) ^ (CRC32_TABLE[crc & 0xFFu]);
    }
    return (crc ^ CRC32_XOR_VALUE);
}

uint32_t Crc_CalculateCRC32P4(
    const uint8_t *Crc_DataPtr,
    uint32_t Crc_Length,
    uint32_t Crc_StartValue32,
    bool Crc_IsFirstCall)
{
    uint32_t crc;

    if (Crc_IsFirstCall)
    {
        crc = CRC32P4_INITIAL_VALUE;
    }
    else
    {
        crc = (CRC32P4_XOR_VALUE ^ Crc_StartValue32);
    }

    for (size_t i = 0; i < Crc_Length; ++i)
    {
        crc ^= (uint32_t)Crc_DataPtr[i];
        crc = (crc >> 8u) ^ (CRC32P4_TABLE[crc & 0xFFu]);
    }
    return (crc ^ CRC32P4_XOR_VALUE);
}

uint64_t Crc_CalculateCRC64(
    const uint8_t *Crc_DataPtr,
    uint32_t Crc_Length,
    uint64_t Crc_StartValue64,
    bool Crc_IsFirstCall)
{
    uint64_t crc;

    if (Crc_IsFirstCall)
    {
        crc = CRC64_INITIAL_VALUE;
    }
    else
    {
        crc = (CRC64_XOR_VALUE ^ Crc_StartValue64);
    }

    for (size_t i = 0; i < Crc_Length; ++i)
    {
        crc ^= (uint64_t)Crc_DataPtr[i];
        crc = (crc >> 8uLL) ^ (CRC64_TABLE[crc & 0xFFuLL]);
    }
    return (crc ^ CRC64_XOR_VALUE);
}

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

static struct PyMethodDef methods[] = {
    {"calculate_crc8",
     py_calculate_crc8,
     METH_VARARGS | METH_KEYWORDS,
     "8-bit SAE J1850 CRC Calculation"},
    {"calculate_crc8_h2f",
     py_calculate_crc8_h2f,
     METH_VARARGS | METH_KEYWORDS,
     "8-bit 0x2F polynomial CRC Calculation"},
    {"calculate_crc16",
     py_calculate_crc16,
     METH_VARARGS | METH_KEYWORDS,
     "16-bit CCITT-FALSE CRC16"},
    {"calculate_crc16_arc",
     py_calculate_crc16_arc,
     METH_VARARGS | METH_KEYWORDS,
     "16-bit 0x8005 polynomial CRC calculation"},
    {"calculate_crc32",
     py_calculate_crc32,
     METH_VARARGS | METH_KEYWORDS,
     "32-bit Ethernet CRC Calculation"},
    {"calculate_crc32_p4",
     py_calculate_crc32_p4,
     METH_VARARGS | METH_KEYWORDS,
     "32-bit 0xF4ACFB13 polynomial CRC calculation"},
    {"calculate_crc64",
     py_calculate_crc64,
     METH_VARARGS | METH_KEYWORDS,
     "64-bit 0x42F0E1EBA9EA3693 polynomial CRC calculation"},
    {NULL} // sentinel
};

static PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "e2e.crc",
    .m_doc = "",
    .m_size = -1,
    .m_methods = methods};

static int
_AddUnsignedIntConstant(PyObject *module, const char* name, uint64_t value)
{
    PyObject *obj = PyLong_FromUnsignedLongLong(value);
    if (PyModule_AddObject(module, name, obj) < 0) {
        Py_XDECREF(obj);
        return -1;
    }
    return 0;
}

#define _AddUnsignedIntMacro(m,c) _AddUnsignedIntConstant(m, #c, c)

PyMODINIT_FUNC PyInit_crc(void)
{

    PyObject *module_p;
    module_p = PyModule_Create(&module);

    if (module_p == NULL)
        return (NULL);
    
    _AddUnsignedIntMacro(module_p, CRC8_INITIAL_VALUE);
    _AddUnsignedIntMacro(module_p, CRC8_XOR_VALUE);
    _AddUnsignedIntMacro(module_p, CRC8_CHECK);
    _AddUnsignedIntMacro(module_p, CRC8_MAGIC_CHECK);

    _AddUnsignedIntMacro(module_p, CRC8H2F_INITIAL_VALUE);
    _AddUnsignedIntMacro(module_p, CRC8H2F_XOR_VALUE);
    _AddUnsignedIntMacro(module_p, CRC8H2F_CHECK);
    _AddUnsignedIntMacro(module_p, CRC8H2F_MAGIC_CHECK);

    _AddUnsignedIntMacro(module_p, CRC16_INITIAL_VALUE);
    _AddUnsignedIntMacro(module_p, CRC16_XOR_VALUE);
    _AddUnsignedIntMacro(module_p, CRC16_CHECK);
    _AddUnsignedIntMacro(module_p, CRC16_MAGIC_CHECK);

    _AddUnsignedIntMacro(module_p, CRC16ARC_INITIAL_VALUE);
    _AddUnsignedIntMacro(module_p, CRC16ARC_XOR_VALUE);
    _AddUnsignedIntMacro(module_p, CRC16ARC_CHECK);
    _AddUnsignedIntMacro(module_p, CRC16ARC_MAGIC_CHECK);

    _AddUnsignedIntMacro(module_p, CRC32_INITIAL_VALUE);
    _AddUnsignedIntMacro(module_p, CRC32_XOR_VALUE);
    _AddUnsignedIntMacro(module_p, CRC32_CHECK);
    _AddUnsignedIntMacro(module_p, CRC32_MAGIC_CHECK);

    _AddUnsignedIntMacro(module_p, CRC32P4_INITIAL_VALUE);
    _AddUnsignedIntMacro(module_p, CRC32P4_XOR_VALUE);
    _AddUnsignedIntMacro(module_p, CRC32P4_CHECK);
    _AddUnsignedIntMacro(module_p, CRC32P4_MAGIC_CHECK);

    _AddUnsignedIntMacro(module_p, CRC64_INITIAL_VALUE);
    _AddUnsignedIntMacro(module_p, CRC64_XOR_VALUE);
    _AddUnsignedIntMacro(module_p, CRC64_CHECK);
    _AddUnsignedIntMacro(module_p, CRC64_MAGIC_CHECK);

    return (module_p);
}