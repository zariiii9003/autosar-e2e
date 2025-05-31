/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

void     uint16_to_littleendian(uint8_t *target, uint16_t value);
void     uint32_to_littleendian(uint8_t *target, uint32_t value);
void     uint64_to_littleendian(uint8_t *target, uint64_t value);

uint16_t littleendian_to_uint16(uint8_t *source);
uint32_t littleendian_to_uint32(uint8_t *source);
uint64_t littleendian_to_uint64(uint8_t *source);

void     uint16_to_bigendian(uint8_t *target, uint16_t value);
void     uint32_to_bigendian(uint8_t *target, uint32_t value);
void     uint64_to_bigendian(uint8_t *target, uint64_t value);

uint16_t bigendian_to_uint16(uint8_t *source);
uint32_t bigendian_to_uint32(uint8_t *source);
uint64_t bigendian_to_uint64(uint8_t *source);

#endif
