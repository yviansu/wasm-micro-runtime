/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "bh_common.h"

static char *
align_ptr(char *src, unsigned int b)
{
    uintptr_t v = (uintptr_t)src;
    uintptr_t m = b - 1;
    return (char *)((v + m) & ~m);
}

/*
Memory copy, with word alignment
*/
int
b_memcpy_wa(void *s1, unsigned int s1max, const void *s2, unsigned int n)
{
    char *dest = (char *)s1;
    char *src = (char *)s2;

    char *pa = align_ptr(src, 4);
    char *pb = align_ptr((src + n), 4);

    unsigned int buff;
    const char *p_byte_read;

    unsigned int *p;
    char *ps;

    if (n == 0) {
        return 0;
    }

    if (pa > src) {
        pa -= 4;
    }

    for (p = (unsigned int *)pa; p < (unsigned int *)pb; p++) {
        buff = *(p);
        p_byte_read = ((char *)&buff);

        /* read highing word */
        if ((char *)p <= src) {
            for (ps = src; ps < ((char *)p + 4); ps++) {
                if (ps >= src + n) {
                    break;
                }
                p_byte_read = ((char *)&buff) + (ps - (char *)p);
                *dest++ = *p_byte_read;
            }
        }
        /* read lowing word */
        else if ((char *)p >= pb - 4) {
            for (ps = (char *)p; ps < src + n; ps++) {
                *dest++ = *p_byte_read++;
            }
        }
        /* read meaning word(s) */
        else {
            if ((char *)p + 4 >= src + n) {
                for (ps = (char *)p; ps < src + n; ps++) {
                    *dest++ = *p_byte_read++;
                }
            }
            else {
                *(unsigned int *)dest = buff;
                dest += 4;
            }
        }
    }

    return 0;
}

int
b_memcpy_s(void *s1, unsigned int s1max, const void *s2, unsigned int n)
{
    char *dest = (char *)s1;
    char *src = (char *)s2;
    if (n == 0) {
        return 0;
    }

    if (s1 == NULL) {
        return -1;
    }
    if (s2 == NULL || n > s1max) {
        memset(dest, 0, s1max);
        return -1;
    }
    memcpy(dest, src, n);
    return 0;
}

int
b_memmove_s(void *s1, unsigned int s1max, const void *s2, unsigned int n)
{
    char *dest = (char *)s1;
    char *src = (char *)s2;
    if (n == 0) {
        return 0;
    }

    if (s1 == NULL) {
        return -1;
    }
    if (s2 == NULL || n > s1max) {
        memset(dest, 0, s1max);
        return -1;
    }
    memmove(dest, src, n);
    return 0;
}

int
b_strcat_s(char *s1, unsigned int s1max, const char *s2)
{
    if (NULL == s1 || NULL == s2 || s1max < (strlen(s1) + strlen(s2) + 1)) {
        return -1;
    }

    memcpy(s1 + strlen(s1), s2, strlen(s2) + 1);
    return 0;
}

int
b_strcpy_s(char *s1, unsigned int s1max, const char *s2)
{
    if (NULL == s1 || NULL == s2 || s1max < (strlen(s2) + 1)) {
        return -1;
    }

    memcpy(s1, s2, strlen(s2) + 1);
    return 0;
}

char *
bh_strdup(const char *s)
{
    uint32 size;
    char *s1 = NULL;

    if (s) {
        size = (uint32)(strlen(s) + 1);
        if ((s1 = BH_MALLOC(size)))
            bh_memcpy_s(s1, size, s, size);
    }
    return s1;
}

char *
wa_strdup(const char *s)
{
    uint32 size;
    char *s1 = NULL;

    if (s) {
        size = (uint32)(strlen(s) + 1);
        if ((s1 = WA_MALLOC(size)))
            bh_memcpy_s(s1, size, s, size);
    }
    return s1;
}

bool
is_high_surrogate(uint32 code_point)
{
    return (code_point >= 0xD800 && code_point <= 0xDBFF);
}

bool
is_low_surrogate(uint32 code_point)
{
    return (code_point >= 0xDC00 && code_point <= 0xDFFF);
}

bool
is_isolated_surrogate(uint32 code_point)
{
    return (code_point >= 0xD800 && code_point <= 0xDFFF);
}

bool
is_supplementary_code_point(uint32 code_point)
{
    return (code_point >= 0x10000 && code_point <= 0x10FFFF);
}

bool
is_BMP_code_point(uint32 code_point)
{
    return (code_point <= 0xFFFF);
}

bool
is_isolated_surrogate_pair(uint32 high, uint32 low)
{
    return is_isolated_surrogate(high) && is_isolated_surrogate(low);
}

bool
is_isolated_surrogate_unpair(uint32 code_point)
{
    return is_isolated_surrogate(code_point)
           && !is_isolated_surrogate_pair(code_point, code_point);
}

bool
is_wtf8_codepoint_start(uint8 *bytes, uint32 pos)
{
    return ((*(bytes + pos)) & 0xC0) != 0x80;
}

uint32
align_wtf8_sequential(uint8 *bytes, uint32 pos, uint32 bytes_length)
{
    if (pos >= bytes_length) {
        return bytes_length;
    }

    if (is_wtf8_codepoint_start(bytes, pos)) {
        return pos;
    }

    if (pos + 1 == bytes_length) {
        return pos + 1;
    }
    if (is_wtf8_codepoint_start(bytes, pos + 1)) {
        return pos + 1;
    }

    if (pos + 2 == bytes_length) {
        return pos + 2;
    }
    if (is_wtf8_codepoint_start(bytes, pos + 2)) {
        return pos + 2;
    }

    return pos + 3;
}

uint32
align_wtf8_reverse(uint8 *bytes, uint32 pos, uint32 bytes_length)
{
    bh_assert(pos < bytes_length);

    if (is_wtf8_codepoint_start(bytes, pos)) {
        return pos;
    }

    if (is_wtf8_codepoint_start(bytes, pos - 1)) {
        return pos - 1;
    }

    if (is_wtf8_codepoint_start(bytes, pos - 2)) {
        return pos - 2;
    }

    return pos - 3;
}

bool
has_isolated_surrogate(uint32 *code_points, uint32 code_points_length)
{
    uint32 i;

    for (i = 0; i < code_points_length; i++) {
        if (is_isolated_surrogate(*(code_points + i))) {
            return false;
        }
    }
    return true;
}

void
decode_codepoints_to_8bit_bytes(uint32 *code_points, uint32 code_points_length,
                                uint8 *target_bytes, int32 *target_bytes_length)
{
    int32 target_bytes_count = 0;
    uint32 i, code_point;

    for (i = 0; i < code_points_length; i++) {
        code_point = code_points[i];
        if (is_high_surrogate(code_point)) {
            /* If is a high surrogate code point, and next is a low surrogate
             * code point, reset value */
            if (i < code_points_length - 1
                && is_low_surrogate(code_points[i + 1])) {
                code_point = 0x10000 + ((code_point - 0xD800) << 10)
                             + (code_points[i + 1] - 0xDC00);
                i++;
            }
            else {
                if (target_bytes_length) {
                    *target_bytes_length = -1;
                }
                return;
            }
        }

        if (code_point <= 0x007F) {
            /* U+0000 to U+007F */
            if (target_bytes) {
                target_bytes[target_bytes_count++] = code_point & 0x7F;
            }
            else {
                target_bytes_count += 1;
            }
        }
        else if (0x0080 <= code_point && code_point <= 0x07FF) {
            /* U+0080 to U+07FF */
            if (target_bytes) {
                target_bytes[target_bytes_count++] = 0xC0 | (code_point >> 6);
                target_bytes[target_bytes_count++] = 0x80 | (code_point & 0x3F);
            }
            else {
                target_bytes_count += 2;
            }
        }
        else if (0x0800 <= code_point && code_point <= 0xFFFF) {
            /* U+0800 to U+FFFF */
            if (target_bytes) {
                target_bytes[target_bytes_count++] = 0xE0 | (code_point >> 12);
                target_bytes[target_bytes_count++] =
                    0x80 | ((code_point >> 6) & 0x3F);
                target_bytes[target_bytes_count++] = 0x80 | (code_point & 0x3F);
            }
            else {
                target_bytes_count += 3;
            }
        }
        else if (0x10000 <= code_point && code_point <= 0x10FFFF) {
            /* U+10000 to U+10FFFF */
            if (target_bytes) {
                target_bytes[target_bytes_count++] = 0xF0 | (code_point >> 18);
                target_bytes[target_bytes_count++] =
                    0x80 | ((code_point >> 12) & 0x3F);
                target_bytes[target_bytes_count++] =
                    0x80 | ((code_point >> 6) & 0x3F);
                target_bytes[target_bytes_count++] = 0x80 | (code_point & 0x3F);
            }
            else {
                target_bytes_count += 4;
            }
        }
    }

    if (target_bytes_length) {
        *target_bytes_length = target_bytes_count;
    }
}

void
decode_codepoints_to_16bit_bytes(uint32 *code_points, uint32 code_points_length,
                                 uint16 *target_bytes,
                                 int32 *target_bytes_length)
{
    int32 target_bytes_count = 0;
    uint32 i, code_point;

    for (i = 0; i < code_points_length; i++) {
        code_point = code_points[i];
        if (is_supplementary_code_point(code_point)) {
            if (target_bytes) {
                target_bytes[target_bytes_count++] =
                    ((code_point - 0x10000) >> 10) + 0xD800;
                target_bytes[target_bytes_count++] =
                    ((code_point - 0x10000) & 0x3FF) + 0xDC00;
            }
            else {
                target_bytes_count += 2;
            }
        }
        else if (is_BMP_code_point(code_point)) {
            if (target_bytes) {
                target_bytes[target_bytes_count++] = (uint16)code_point;
            }
            else {
                target_bytes_count += 1;
            }
        }
    }

    if (target_bytes_length) {
        *target_bytes_length = target_bytes_count;
    }
}

uint32
decode_8bit_bytes_to_one_codepoint(uint8 *bytes, uint32 pos,
                                   uint32 bytes_length, uint32 *code_point)
{
    uint8 byte, byte2, byte3, byte4;
    uint32 target_bytes_count = 0;

    byte = bytes[pos++];
    if (byte <= 0x7F) {
        if (code_point) {
            *code_point = byte;
        }
        target_bytes_count = 1;
    }
    else if (byte >= 0xC2 && byte <= 0xDF && pos < bytes_length) {
        byte2 = bytes[pos++];
        if (code_point) {
            *code_point = ((byte & 0x1F) << 6) + (byte2 & 0x3F);
        }
        target_bytes_count = 2;
    }
    else if (byte >= 0xE0 && byte <= 0xEF && pos + 1 < bytes_length) {
        byte2 = bytes[pos++];
        byte3 = bytes[pos++];
        if (code_point) {
            *code_point =
                ((byte & 0x0F) << 12) + ((byte2 & 0x3F) << 6) + (byte3 & 0x3F);
        }
        target_bytes_count = 3;
    }
    else if (byte >= 0xF0 && byte <= 0xF4 && pos + 2 < bytes_length) {
        byte2 = bytes[pos++];
        byte3 = bytes[pos++];
        byte4 = bytes[pos++];
        if (code_point) {
            *code_point = ((byte & 0x07) << 18) + ((byte2 & 0x3F) << 12)
                          + ((byte3 & 0x3F) << 6) + (byte4 & 0x3F);
        }
        target_bytes_count = 4;
    }

    return target_bytes_count;
}

void
decode_8bit_bytes(uint8 *bytes, int32 bytes_length, uint32 *code_points,
                  int32 *code_points_length, uint8 *target_bytes,
                  int32 *target_bytes_length, encoding_flag flag)
{
    int32 i = 0, j = 0, k = 0;
    int32 total_target_bytes_count = 0, target_bytes_count = 0;
    uint32 code_point = 0;

    while (i < bytes_length) {
        target_bytes_count = decode_8bit_bytes_to_one_codepoint(
            bytes, i, bytes_length, &code_point);
        i += target_bytes_count;
        if (is_isolated_surrogate(code_point)) {
            if (flag == UTF8) {
                if (target_bytes_length) {
                    *target_bytes_length = -1;
                }
                return;
            }
            else if (flag == WTF8) {
                if (target_bytes) {
                    for (k = 0; k < target_bytes_count; k++) {
                        *(target_bytes + k + total_target_bytes_count) =
                            *(bytes + i - target_bytes_count + k);
                    }
                }
                total_target_bytes_count += target_bytes_count;
            }
            else if (flag == LOSSY_UTF8) {
                code_point = 0xFFFD;
                if (target_bytes) {
                    *(target_bytes + total_target_bytes_count) =
                        0xE0 | (code_point >> 12);
                    *(target_bytes + total_target_bytes_count + 1) =
                        0x80 | ((code_point >> 6) & 0x3F);
                    *(target_bytes + total_target_bytes_count + 2) =
                        0x80 | (code_point & 0x3F);
                }
                total_target_bytes_count += 3;
            }
        }
        else {
            if (target_bytes) {
                for (k = 0; k < target_bytes_count; k++) {
                    *(target_bytes + k + total_target_bytes_count) =
                        *(bytes + i - target_bytes_count + k);
                }
            }
            total_target_bytes_count += target_bytes_count;
        }

        if (code_points) {
            code_points[j] = code_point;
        }
        j++;
    }

    if (code_points_length) {
        *code_points_length = j;
    }

    if (target_bytes_length) {
        *target_bytes_length = total_target_bytes_count;
    }
}

int32
decode_16bit_bytes_to_one_codepoint(uint16 *bytes, uint32 pos,
                                    uint32 bytes_length, uint32 *code_point)
{
    uint16 byte, byte2;
    int32 target_bytes_count = 0;

    byte = bytes[pos++];
    if (is_high_surrogate(byte) && pos < bytes_length) {
        byte2 = bytes[pos++];
        if (is_low_surrogate(byte)) {
            if (*code_point) {
                *code_point =
                    0x10000 + ((byte - 0xD800) << 10) + (byte2 - 0xDC00);
            }
            target_bytes_count = 2;
        }
        else {
            target_bytes_count = -1;
        }
    }
    else {
        if (*code_point) {
            *code_point = byte;
        }
        target_bytes_count = 1;
    }

    return target_bytes_count;
}

void
decode_16bit_bytes(uint16 *bytes, int32 bytes_length, uint32 *code_points,
                   int32 *code_points_length)
{
    int32 i = 0, j = 0, target_bytes_count = 0;
    uint32 code_point = 0;

    while (i < bytes_length) {
        target_bytes_count = decode_16bit_bytes_to_one_codepoint(
            bytes, i, bytes_length, &code_point);
        i += target_bytes_count;
        if (code_points) {
            code_points[j] = code_point;
        }
        j++;
    }
    if (code_points_length) {
        *code_points_length = j;
    }
}

int32
calculate_encoded_8bit_bytes_length_by_codepoints(uint32 *code_points,
                                                  uint32 code_points_length)
{
    int32 target_bytes_length;

    decode_codepoints_to_8bit_bytes(code_points, code_points_length, NULL,
                                    &target_bytes_length);

    return target_bytes_length;
}

uint8 *
encode_8bit_bytes_by_codepoints(uint32 *code_points, uint32 code_points_length,
                                int32 *target_bytes_length)
{
    uint8 *target_bytes;

    *target_bytes_length = calculate_encoded_8bit_bytes_length_by_codepoints(
        code_points, code_points_length);

    if (*target_bytes_length > 0) {
        if (!(target_bytes = wasm_runtime_malloc(sizeof(uint8)
                                                 * (*target_bytes_length)))) {
            return NULL;
        }
        /* get target bytes */
        decode_codepoints_to_8bit_bytes(code_points, code_points_length,
                                        target_bytes, NULL);
    }
    else {
        target_bytes = NULL;
    }

    return target_bytes;
}

int32
calculate_encoded_16bit_bytes_length_by_codepoints(uint32 *code_points,
                                                   uint32 code_points_length)
{
    int32 target_bytes_length;

    decode_codepoints_to_16bit_bytes(code_points, code_points_length, NULL,
                                     &target_bytes_length);

    return target_bytes_length;
}

uint16 *
encode_16bit_bytes_by_codepoints(uint32 *code_points, uint32 code_points_length,
                                 int32 *target_bytes_length)
{
    uint16 *target_bytes;

    *target_bytes_length = calculate_encoded_16bit_bytes_length_by_codepoints(
        code_points, code_points_length);

    if (*target_bytes_length > 0) {
        if (!(target_bytes = wasm_runtime_malloc(sizeof(uint16)
                                                 * (*target_bytes_length)))) {
            return NULL;
        }
        /* get target bytes */
        decode_codepoints_to_16bit_bytes(code_points, code_points_length,
                                         target_bytes, NULL);
    }
    else {
        target_bytes = NULL;
    }

    return target_bytes;
}

uint16 *
encode_16bit_bytes_by_8bit_bytes(uint8 *bytes, int32 bytes_length,
                                 int32 *target_code_units)
{
    uint16 *target_bytes;
    uint32 *code_points;
    int32 code_points_length;

    code_points = encode_codepoints_by_8bit_bytes_with_flag(
        bytes, bytes_length, &code_points_length, WTF8);
    *target_code_units = calculate_encoded_16bit_bytes_length_by_codepoints(
        code_points, code_points_length);

    if (*target_code_units > 0) {
        if (!(target_bytes =
                  wasm_runtime_malloc(sizeof(uint16) * (*target_code_units)))) {
            return NULL;
        }
        /* get target bytes */
        decode_codepoints_to_16bit_bytes(code_points, code_points_length,
                                         target_bytes, NULL);
    }
    else {
        target_bytes = NULL;
    }
    if (code_points) {
        wasm_runtime_free(code_points);
    }

    return target_bytes;
}

int32
calculate_encoded_8bit_bytes_length_by_8bit_bytes_with_flag(uint8 *bytes,
                                                            int32 bytes_length,
                                                            encoding_flag flag)
{
    int32 target_bytes_length;

    decode_8bit_bytes(bytes, bytes_length, NULL, NULL, NULL,
                      &target_bytes_length, flag);

    return target_bytes_length;
}

uint8 *
encode_8bit_bytes_by_8bit_bytes_with_flag(uint8 *bytes, int32 bytes_length,
                                          int32 *target_bytes_length,
                                          encoding_flag flag)
{
    uint8 *target_bytes;

    /* get target bytes length */
    *target_bytes_length =
        calculate_encoded_8bit_bytes_length_by_8bit_bytes_with_flag(
            bytes, bytes_length, flag);

    if (*target_bytes_length > 0) {
        if (!(target_bytes = wasm_runtime_malloc(sizeof(uint8)
                                                 * (*target_bytes_length)))) {
            return NULL;
        }
        /* get target bytes */
        decode_8bit_bytes(bytes, bytes_length, NULL, NULL, target_bytes, NULL,
                          flag);
    }
    else {
        target_bytes = NULL;
    }

    return target_bytes;
}

uint32 *
encode_codepoints_by_8bit_bytes_with_flag(uint8 *bytes, int32 bytes_length,
                                          int32 *code_points_length,
                                          encoding_flag flag)
{
    uint32 *code_points;

    /* get code points length */
    decode_8bit_bytes(bytes, bytes_length, NULL, code_points_length, NULL, NULL,
                      flag);

    if (*code_points_length > 0) {
        if (!(code_points = wasm_runtime_malloc(sizeof(uint32)
                                                * (*code_points_length)))) {
            return NULL;
        }
        /* get code points */
        decode_8bit_bytes(bytes, bytes_length, code_points, NULL, NULL, NULL,
                          flag);
    }
    else {
        code_points = NULL;
    }
    return code_points;
}

uint32 *
encode_codepoints_by_16bit_bytes(uint16 *bytes, int32 bytes_length,
                                 int32 *code_points_length)
{
    uint32 *code_points;

    /* get code points length */
    decode_16bit_bytes(bytes, bytes_length, NULL, code_points_length);

    if (*code_points_length > 0) {
        if (!(code_points = wasm_runtime_malloc(sizeof(uint32)
                                                * (*code_points_length)))) {
            return NULL;
        }
        /* get code points */
        decode_16bit_bytes(bytes, bytes_length, code_points, NULL);
    }
    else {
        code_points = NULL;
    }
    return code_points;
}

int32
calculate_encoded_code_units_by_8bit_bytes_with_flag(uint8 *bytes,
                                                     int32 bytes_length,
                                                     encoding_flag flag)
{
    int32 target_bytes_length, code_points_length;
    uint32 *code_points;

    if (flag == WTF16) {
        code_points = encode_codepoints_by_8bit_bytes_with_flag(
            bytes, bytes_length, &code_points_length, WTF8);
        target_bytes_length =
            calculate_encoded_16bit_bytes_length_by_codepoints(
                code_points, code_points_length);
        if (code_points) {
            wasm_runtime_free(code_points);
        }
    }
    else {
        target_bytes_length =
            calculate_encoded_8bit_bytes_length_by_8bit_bytes_with_flag(
                bytes, bytes_length, flag);
    }

    return target_bytes_length;
}

void *
encode_target_bytes_by_8bit_bytes_with_flag(uint8 *bytes, int32 bytes_length,
                                            int32 *target_bytes_length,
                                            encoding_flag flag)
{
    void *target_bytes;
    if (flag == WTF16) {
        target_bytes = encode_16bit_bytes_by_8bit_bytes(bytes, bytes_length,
                                                        target_bytes_length);
    }
    else {
        target_bytes = encode_8bit_bytes_by_8bit_bytes_with_flag(
            bytes, bytes_length, target_bytes_length, flag);
    }
    return target_bytes;
}

uint8 *
concat_8bit_bytes(uint8 *bytes1, int32 bytes_length1, uint8 *bytes2,
                  int32 bytes_length2, int32 *bytes_length_total,
                  encoding_flag flag)
{
    uint32 *code_points1 = NULL, *code_points2 = NULL, *code_points_total = NULL;
    int32 code_points_length1 = 0, code_points_length2 = 0, code_points_total_length = 0;
    uint8 *target_bytes = NULL;

    code_points1 = encode_codepoints_by_8bit_bytes_with_flag(
        bytes1, bytes_length1, &code_points_length1, flag);
    code_points2 = encode_codepoints_by_8bit_bytes_with_flag(
        bytes2, bytes_length2, &code_points_length2, flag);
    code_points_total_length = code_points_length1 + code_points_length2;
    if (code_points_total_length > 0) {
        code_points_total =
            wasm_runtime_malloc(sizeof(uint32) * code_points_total_length);
        bh_memcpy_s(code_points_total, sizeof(uint32) * code_points_length1,
                    code_points1, sizeof(uint32) * code_points_length1);
        bh_memcpy_s(code_points_total + code_points_length1,
                    sizeof(uint32) * code_points_length2, code_points2,
                    sizeof(uint32) * code_points_length2);
    }
    target_bytes = encode_8bit_bytes_by_codepoints(
        code_points_total, code_points_total_length, bytes_length_total);
    if (code_points1) {
        wasm_runtime_free(code_points1);
    }
    if (code_points2) {
        wasm_runtime_free(code_points2);
    }
    if (code_points_total) {
        wasm_runtime_free(code_points_total);
    }
    return target_bytes;
}

uint32
wtf16_pos_treatment(uint32 pos, uint32 code_units_length)
{
    uint32 ret_pos;

    ret_pos = pos;
    if (pos > code_units_length) {
        ret_pos = code_units_length;
    }

    return ret_pos;
}

uint32
wtf8_string_bytes_advance(uint8 *string_bytes, int32 string_bytes_length,
                          uint32 pos, uint32 bytes)
{
    uint32 start_pos, next_pos;

    start_pos = align_wtf8_sequential(string_bytes, pos, string_bytes_length);
    if (bytes == 0) {
        next_pos = start_pos;
    }
    else if (bytes >= string_bytes_length - start_pos) {
        next_pos = string_bytes_length;
    }
    else {
        next_pos = align_wtf8_reverse(string_bytes, start_pos + bytes,
                                      string_bytes_length);
    }

    return next_pos;
}

int32
wtf8_string_bytes_iter_next(uint8 *string_bytes, int32 string_bytes_length,
                            int32 cur_pos, uint32 *code_point)
{
    uint32 target_bytes_count;

    if (cur_pos >= string_bytes_length) {
        return -1;
    }

    target_bytes_count = decode_8bit_bytes_to_one_codepoint(
        string_bytes, cur_pos, string_bytes_length, code_point);
    cur_pos += target_bytes_count;

    return cur_pos;
}

int32
wtf8_string_bytes_iter_advance(uint8 *string_bytes, int32 string_bytes_length,
                               int32 cur_pos, uint32 code_points_count,
                               uint32 *code_points_consumed)
{
    uint32 advance_count = 0, target_bytes_count, advance_pos;

    while (advance_count < code_points_count) {
        if (cur_pos == string_bytes_length) {
            break;
        }
        advance_count++;
        advance_pos = align_wtf8_sequential(string_bytes, cur_pos + 1,
                                            string_bytes_length);
        target_bytes_count = advance_pos - cur_pos;
        cur_pos += target_bytes_count;
    }

    if (code_points_consumed) {
        *code_points_consumed = advance_count;
    }

    return cur_pos;
}

int32
wtf8_string_bytes_iter_rewind(uint8 *string_bytes, int32 string_bytes_length,
                              int32 cur_pos, uint32 code_points_count,
                              uint32 *code_points_consumed)
{
    uint32 rewind_count = 0, target_bytes_count, rewind_pos;

    while (rewind_count < code_points_count) {
        if (cur_pos == 0) {
            break;
        }
        rewind_count++;
        rewind_pos =
            align_wtf8_reverse(string_bytes, cur_pos - 1, string_bytes_length);
        target_bytes_count = cur_pos - rewind_pos;
        cur_pos -= target_bytes_count;
    }

    if (code_points_consumed) {
        *code_points_consumed = rewind_count;
    }

    return cur_pos;
}

int32
wtf8_string_bytes_iter_slice(uint8 *string_bytes, int32 string_bytes_length,
                             int32 cur_pos, int32 code_points_count)
{
    int32 end_pos, advance_count, target_bytes_count, advance_pos;

    advance_count = 0;
    end_pos = cur_pos;
    while (advance_count < code_points_count) {
        if (end_pos == string_bytes_length) {
            break;
        }
        advance_count++;

        advance_pos = align_wtf8_sequential(string_bytes, end_pos + 1,
                                            string_bytes_length);
        target_bytes_count = advance_pos - end_pos;
        end_pos += target_bytes_count;
    }

    return end_pos;
}
