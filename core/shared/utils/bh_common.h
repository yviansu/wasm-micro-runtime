/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef _BH_COMMON_H
#define _BH_COMMON_H

#include "bh_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#define bh_memcpy_s(dest, dlen, src, slen)            \
    do {                                              \
        int _ret = b_memcpy_s(dest, dlen, src, slen); \
        (void)_ret;                                   \
        bh_assert(_ret == 0);                         \
    } while (0)

#define bh_memcpy_wa(dest, dlen, src, slen)            \
    do {                                               \
        int _ret = b_memcpy_wa(dest, dlen, src, slen); \
        (void)_ret;                                    \
        bh_assert(_ret == 0);                          \
    } while (0)

#define bh_memmove_s(dest, dlen, src, slen)            \
    do {                                               \
        int _ret = b_memmove_s(dest, dlen, src, slen); \
        (void)_ret;                                    \
        bh_assert(_ret == 0);                          \
    } while (0)

#define bh_strcat_s(dest, dlen, src)            \
    do {                                        \
        int _ret = b_strcat_s(dest, dlen, src); \
        (void)_ret;                             \
        bh_assert(_ret == 0);                   \
    } while (0)

#define bh_strcpy_s(dest, dlen, src)            \
    do {                                        \
        int _ret = b_strcpy_s(dest, dlen, src); \
        (void)_ret;                             \
        bh_assert(_ret == 0);                   \
    } while (0)

int
b_memcpy_s(void *s1, unsigned int s1max, const void *s2, unsigned int n);
int
b_memcpy_wa(void *s1, unsigned int s1max, const void *s2, unsigned int n);
int
b_memmove_s(void *s1, unsigned int s1max, const void *s2, unsigned int n);
int
b_strcat_s(char *s1, unsigned int s1max, const char *s2);
int
b_strcpy_s(char *s1, unsigned int s1max, const char *s2);

/* strdup with string allocated by BH_MALLOC */
char *
bh_strdup(const char *s);

/* strdup with string allocated by WA_MALLOC */
char *
wa_strdup(const char *s);

typedef enum encoding_flag {
    UTF8,
    WTF8,
    WTF16,
    LOSSY_UTF8,
} encoding_flag;

int32
calculate_encoded_codepoints_length_by_8bit_bytes_with_flag(uint8 *bytes,
                                                            int32 bytes_length,
                                                            encoding_flag flag);

uint8 *
encode_8bit_bytes_by_8bit_bytes_with_flag(uint8 *bytes, int32 bytes_length,
                                          int32 *target_bytes_length,
                                          encoding_flag flag);

uint8 *
encode_8bit_bytes_by_codepoints(uint32 *code_points, uint32 code_points_length,
                                int32 *target_bytes_length);

int32
calculate_encoded_16bit_bytes_length_by_codepoints(uint32 *code_points,
                                                   uint32 code_points_length);

uint8 *
encode_16bit_bytes_by_codepoints(uint32 *code_points, uint32 code_points_length,
                                 int32 *target_bytes_length);

uint32 *
encode_codepoints_by_8bit_bytes_with_flag(uint8 *bytes, int32 bytes_length,
                                          int32 *code_points_length,
                                          encoding_flag flag);

uint32 *
encode_codepoints_by_16bit_bytes(uint16 *bytes, int32 bytes_length,
                                 int32 *code_points_length);

uint8 *
concat_8bit_bytes(uint8 *bytes1, int32 bytes_length1, uint8 *bytes2,
                  int32 bytes_length2, int32 *bytes_length_total,
                  encoding_flag flag);

uint32
wtf8_string_bytes_advance(uint8 *string_bytes, int32 string_bytes_length,
                          uint32 pos, uint32 bytes);

int32
wtf8_string_bytes_iter_next(uint8 *string_bytes, int32 string_bytes_length,
                            int32 cur_pos, uint32 *code_point);

int32
wtf8_string_bytes_iter_advance(uint8 *string_bytes, int32 string_bytes_length,
                               int32 cur_pos, uint32 code_points_count,
                               uint32 *code_points_consumed);

int32
wtf8_string_bytes_iter_rewind(uint8 *string_bytes, int32 string_bytes_length,
                              int32 cur_pos, uint32 code_points_count,
                              uint32 *code_points_consumed);

int32
wtf8_string_bytes_iter_slice(uint8 *string_bytes, int32 string_bytes_length,
                             int32 cur_pos, uint32 code_points_count);

#ifdef __cplusplus
}
#endif

#endif
