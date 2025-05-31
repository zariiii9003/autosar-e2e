/* SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT */

#include <stddef.h>
#include <stdint.h>

// copy uint16_t into buffer in littleendian byte order
void uint16_to_littleendian(uint8_t *target, uint16_t value)
{
    size_t len = sizeof(value);
    for (size_t i = 0; i < len; ++i) {
        target[i] = (uint8_t)((value >> (i * 8)) & 0xffu);
    }
}

// copy uint32_t into buffer in littleendian byte order
void uint32_to_littleendian(uint8_t *target, uint32_t value)
{
    size_t len = sizeof(value);
    for (size_t i = 0; i < len; ++i) {
        target[i] = (uint8_t)((value >> (i * 8)) & 0xffu);
    }
}

// copy uint64_t into buffer in littleendian byte order
void uint64_to_littleendian(uint8_t *target, uint64_t value)
{
    size_t len = sizeof(value);
    for (size_t i = 0; i < len; ++i) {
        target[i] = (uint8_t)((value >> (i * 8)) & 0xffu);
    }
}

// read uint16_t from littleendian byte order buffer
uint16_t littleendian_to_uint16(uint8_t *source)
{
    uint16_t value = 0;
    size_t   len   = sizeof(value);
    for (size_t i = 0; i < len; ++i) {
        value += (uint16_t)source[i] << (i * 8);
    }
    return value;
}

// read uint32_t from littleendian byte order buffer
uint32_t littleendian_to_uint32(uint8_t *source)
{
    uint32_t value = 0;
    size_t   len   = sizeof(value);
    for (size_t i = 0; i < len; ++i) {
        value += (uint32_t)source[i] << (i * 8);
    }
    return value;
}

// read uint64_t from littleendian byte order buffer
uint64_t littleendian_to_uint64(uint8_t *source)
{
    uint64_t value = 0;
    size_t   len   = sizeof(value);
    for (size_t i = 0; i < len; ++i) {
        value += (uint64_t)source[i] << (i * 8);
    }
    return value;
}

// copy uint16_t into buffer in bigendian byte order
void uint16_to_bigendian(uint8_t *target, uint16_t value)
{
    size_t len = sizeof(value);
    for (size_t i = 0; i < len; ++i) {
        target[i] = (uint8_t)((value >> ((len - 1 - i) * 8)) & 0xffu);
    }
}

// copy uint32_t into buffer in bigendian byte order
void uint32_to_bigendian(uint8_t *target, uint32_t value)
{
    size_t len = sizeof(value);
    for (size_t i = 0; i < len; ++i) {
        target[i] = (uint8_t)((value >> ((len - 1 - i) * 8)) & 0xffu);
    }
}

// copy uint64_t into buffer in bigendian byte order
void uint64_to_bigendian(uint8_t *target, uint64_t value)
{
    size_t len = sizeof(value);
    for (size_t i = 0; i < len; ++i) {
        target[i] = (uint8_t)((value >> ((len - 1 - i) * 8)) & 0xffu);
    }
}

// read uint16_t from bigendian byte order buffer
uint16_t bigendian_to_uint16(uint8_t *source)
{
    uint16_t value = 0;
    size_t   len   = sizeof(value);
    for (size_t i = 0; i < len; ++i) {
        value += (uint16_t)source[i] << ((len - 1 - i) * 8);
    }
    return value;
}

// read uint32_t from bigendian byte order buffer
uint32_t bigendian_to_uint32(uint8_t *source)
{
    uint32_t value = 0;
    size_t   len   = sizeof(value);
    for (size_t i = 0; i < len; ++i) {
        value += (uint32_t)source[i] << ((len - 1 - i) * 8);
    }
    return value;
}

// read uint64_t from bigendian byte order buffer
uint64_t bigendian_to_uint64(uint8_t *source)
{
    uint64_t value = 0;
    size_t   len   = sizeof(value);
    for (size_t i = 0; i < len; ++i) {
        value += (uint64_t)source[i] << ((len - 1 - i) * 8);
    }
    return value;
}
