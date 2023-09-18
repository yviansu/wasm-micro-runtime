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

bool
is_lead_surrogate(uint16_t code_point);

bool
is_trail_surrogate(uint16_t code_point);

bool
is_isolated_surrogate(uint16_t code_point);

unsigned int
align_wtf8_sequential(char *wtf8_string, unsigned int pos,
                      unsigned int wtf8_length);

unsigned int
align_wtf8_reverse(char *wtf8_string, unsigned int pos,
                   unsigned int wtf8_length);

void
decode_well_formed_wtf8(char *wtf8_string, unsigned int wtf8_length,
                        uint32 *code_points, unsigned int *code_point_length);

unsigned int
measure_wtf8(char *bytes, unsigned int bytes_length, encoding_flag flag);

#ifdef __cplusplus
}
#endif

#endif
