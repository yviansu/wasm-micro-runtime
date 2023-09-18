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

        /* read leading word */
        if ((char *)p <= src) {
            for (ps = src; ps < ((char *)p + 4); ps++) {
                if (ps >= src + n) {
                    break;
                }
                p_byte_read = ((char *)&buff) + (ps - (char *)p);
                *dest++ = *p_byte_read;
            }
        }
        /* read trailing word */
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

int
is_lead_surrogate(int code_point)
{
    return (0xD800 <= code_point && code_point <= 0xDBFF);
}

int
is_trail_surrogate(int code_point)
{
    return (0xDC00 <= code_point && code_point <= 0xDFFF);
}

size_t
encode_to_wtf8(int *code_points, size_t num_code_points, char **wtf8_string)
{
    size_t result_length = 0;
    int i;

    for (i = 0; i < num_code_points; i++) {
        int code_point = code_points[i];
        if (is_lead_surrogate(code_point)) {
            /* If is a lead surrogate code point, and next is a trail surrogate
             * code point, reset value */
            if (i < num_code_points - 1
                && is_trail_surrogate(code_points[i + 1])) {
                code_point = 0x10000 + ((code_point - 0xD800) << 10)
                             + (code_points[i + 1] - 0xDC00);
                i++;
            }
            else {
                fprintf(stderr, "Invalid surrogate pair\n");
                return 0;
            }
        }

        if (0x0000 <= code_point && code_point <= 0x007F) {
            /* U+0000 to U+007F */
            (*wtf8_string)[result_length++] = code_point & 0x7F;
        }
        else if (0x0080 <= code_point && code_point <= 0x07FF) {
            /* U+0080 to U+07FF */
            (*wtf8_string)[result_length++] = 0xC0 | (code_point >> 6);
            (*wtf8_string)[result_length++] = 0x80 | (code_point & 0x3F);
        }
        else if (0x0800 <= code_point && code_point <= 0xFFFF) {
            /* U+0800 to U+FFFF */
            (*wtf8_string)[result_length++] = 0xE0 | (code_point >> 12);
            (*wtf8_string)[result_length++] = 0x80 | ((code_point >> 6) & 0x3F);
            (*wtf8_string)[result_length++] = 0x80 | (code_point & 0x3F);
        }
        else if (0x10000 <= code_point && code_point <= 0x10FFFF) {
            /* U+10000 to U+10FFFF */
            (*wtf8_string)[result_length++] = 0xF0 | (code_point >> 18);
            (*wtf8_string)[result_length++] =
                0x80 | ((code_point >> 12) & 0x3F);
            (*wtf8_string)[result_length++] = 0x80 | ((code_point >> 6) & 0x3F);
            (*wtf8_string)[result_length++] = 0x80 | (code_point & 0x3F);
        }
        else {
            fprintf(stderr, "Invalid code point\n");
            return 0;
        }
    }

    return result_length;
}

bool
is_wtf8_codepoint_start(char *wtf8_string, uint32 pos)
{
    return ((*(wtf8_string + pos)) & 0xC0) != 0x80;
}

unsigned int
align_wtf8_sequential(char *wtf8_string, unsigned int pos,
                      unsigned int wtf8_length)
{
    if (pos >= wtf8_length) {
        return wtf8_length;
    }

    if (is_wtf8_codepoint_start(wtf8_string, pos)) {
        return pos;
    }

    if (pos + 1 == wtf8_length) {
        return pos + 1;
    }
    if (is_wtf8_codepoint_start(wtf8_string, pos + 1)) {
        return pos + 1;
    }

    if (pos + 2 == wtf8_length) {
        return pos + 2;
    }
    if (is_wtf8_codepoint_start(wtf8_string, pos + 2)) {
        return pos + 2;
    }

    return pos + 3;
}

unsigned int
align_wtf8_reverse(char *wtf8_string, unsigned int pos,
                   unsigned int wtf8_length)
{
    bh_assert(pos < wtf8_length);

    if (is_wtf8_codepoint_start(wtf8_string, pos)) {
        return pos;
    }

    if (is_wtf8_codepoint_start(wtf8_string, pos - 1)) {
        return pos - 1;
    }

    if (is_wtf8_codepoint_start(wtf8_string, pos - 2)) {
        return pos - 2;
    }

    return pos - 3;
}
