/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "crclib.h"

uint8_t Crc_CalculateCRC8(const uint8_t *Crc_DataPtr,
                          uint32_t       Crc_Length,
                          uint8_t        Crc_StartValue8,
                          bool           Crc_IsFirstCall)
{
    uint8_t crc;

    if (Crc_IsFirstCall) {
        crc = CRC8_INITIAL_VALUE;
    }
    else {
        crc = (CRC8_XOR_VALUE ^ Crc_StartValue8);
    }

    for (size_t i = 0; i < Crc_Length; ++i) {
        crc = CRC8_TABLE[crc ^ Crc_DataPtr[i]];
    }

    return (crc ^ CRC8_XOR_VALUE);
}

uint8_t Crc_CalculateCRC8H2F(const uint8_t *Crc_DataPtr,
                             uint32_t       Crc_Length,
                             uint8_t        Crc_StartValue8H2F,
                             bool           Crc_IsFirstCall)
{
    uint8_t crc;

    if (Crc_IsFirstCall) {
        crc = CRC8H2F_INITIAL_VALUE;
    }
    else {
        crc = (CRC8H2F_XOR_VALUE ^ Crc_StartValue8H2F);
    }

    for (size_t i = 0; i < Crc_Length; ++i) {
        crc = CRC8H2F_TABLE[crc ^ Crc_DataPtr[i]];
    }

    return (crc ^ CRC8H2F_XOR_VALUE);
}

uint16_t Crc_CalculateCRC16(const uint8_t *Crc_DataPtr,
                            uint32_t       Crc_Length,
                            uint16_t       Crc_StartValue16,
                            bool           Crc_IsFirstCall)
{
    uint16_t crc;

    if (Crc_IsFirstCall) {
        crc = CRC16_INITIAL_VALUE;
    }
    else {
        crc = (CRC16_XOR_VALUE ^ Crc_StartValue16);
    }

    for (size_t i = 0; i < Crc_Length; ++i) {
        crc = (crc << 8) ^ CRC16_TABLE[((crc >> 8) ^ Crc_DataPtr[i]) & 0xFFU];
    }
    return (crc ^ CRC16_XOR_VALUE);
}

uint16_t Crc_CalculateCRC16ARC(const uint8_t *Crc_DataPtr,
                               uint32_t       Crc_Length,
                               uint16_t       Crc_StartValue16,
                               bool           Crc_IsFirstCall)
{
    uint16_t crc;

    if (Crc_IsFirstCall) {
        crc = CRC16ARC_INITIAL_VALUE;
    }
    else {
        crc = (CRC16ARC_XOR_VALUE ^ Crc_StartValue16);
    }

    for (size_t i = 0; i < Crc_Length; ++i) {
        crc = (crc >> 8) ^ CRC16ARC_TABLE[(crc ^ Crc_DataPtr[i]) & 0xFFu];
    }
    return (crc ^ CRC16ARC_XOR_VALUE);
}

uint32_t Crc_CalculateCRC32(const uint8_t *Crc_DataPtr,
                            uint32_t       Crc_Length,
                            uint32_t       Crc_StartValue32,
                            bool           Crc_IsFirstCall)
{
    uint32_t crc;

    if (Crc_IsFirstCall) {
        crc = CRC32_INITIAL_VALUE;
    }
    else {
        crc = (CRC32_XOR_VALUE ^ Crc_StartValue32);
    }

    for (size_t i = 0; i < Crc_Length; ++i) {
        crc ^= (uint32_t)Crc_DataPtr[i];
        crc = (crc >> 8u) ^ (CRC32_TABLE[crc & 0xFFu]);
    }
    return (crc ^ CRC32_XOR_VALUE);
}

uint32_t Crc_CalculateCRC32P4(const uint8_t *Crc_DataPtr,
                              uint32_t       Crc_Length,
                              uint32_t       Crc_StartValue32,
                              bool           Crc_IsFirstCall)
{
    uint32_t crc;

    if (Crc_IsFirstCall) {
        crc = CRC32P4_INITIAL_VALUE;
    }
    else {
        crc = (CRC32P4_XOR_VALUE ^ Crc_StartValue32);
    }

    for (size_t i = 0; i < Crc_Length; ++i) {
        crc ^= (uint32_t)Crc_DataPtr[i];
        crc = (crc >> 8u) ^ (CRC32P4_TABLE[crc & 0xFFu]);
    }
    return (crc ^ CRC32P4_XOR_VALUE);
}

uint64_t Crc_CalculateCRC64(const uint8_t *Crc_DataPtr,
                            uint32_t       Crc_Length,
                            uint64_t       Crc_StartValue64,
                            bool           Crc_IsFirstCall)
{
    uint64_t crc;

    if (Crc_IsFirstCall) {
        crc = CRC64_INITIAL_VALUE;
    }
    else {
        crc = (CRC64_XOR_VALUE ^ Crc_StartValue64);
    }

    for (size_t i = 0; i < Crc_Length; ++i) {
        crc ^= (uint64_t)Crc_DataPtr[i];
        crc = (crc >> 8uLL) ^ (CRC64_TABLE[crc & 0xFFuLL]);
    }
    return (crc ^ CRC64_XOR_VALUE);
}
