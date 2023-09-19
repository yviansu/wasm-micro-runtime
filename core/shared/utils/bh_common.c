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
    for (int i = 0; i < code_points_length; i++) {
        if (is_isolated_surrogate(*(code_points + i))) {
            return false;
        }
    }
    return true;
}

// uint32
// encode_to_wtf8(uint32 *code_points,
//              uint32 *code_points_length, uint8 *target_bytes,
//              encoding_flag flag)
// {
//     size_t result_length = 0;
//     int i;

//     for (i = 0; i < code_points_length; i++) {
//         int code_point = code_points[i];
//         if (is_high_surrogate(code_point)) {
//             /* If is a high surrogate code point, and next is a low surrogate
//              * code point, reset value */
//             if (i < code_points_length - 1
//                 && is_low_surrogate(code_points[i + 1])) {
//                 code_point = 0x10000 + ((code_point - 0xD800) << 10)
//                              + (code_points[i + 1] - 0xDC00);
//                 i++;
//             }
//             else {
//                 fprintf(stderr, "Invalid surrogate pair\n");
//                 return 0;
//             }
//         }

//         if (0x0000 <= code_point && code_point <= 0x007F) {
//             /* U+0000 to U+007F */
//             (*wtf8_string)[result_length++] = code_point & 0x7F;
//         }
//         else if (0x0080 <= code_point && code_point <= 0x07FF) {
//             /* U+0080 to U+07FF */
//             (*wtf8_string)[result_length++] = 0xC0 | (code_point >> 6);
//             (*wtf8_string)[result_length++] = 0x80 | (code_point & 0x3F);
//         }
//         else if (0x0800 <= code_point && code_point <= 0xFFFF) {
//             /* U+0800 to U+FFFF */
//             (*wtf8_string)[result_length++] = 0xE0 | (code_point >> 12);
//             (*wtf8_string)[result_length++] = 0x80 | ((code_point >> 6) &
//             0x3F);
//             (*wtf8_string)[result_length++] = 0x80 | (code_point & 0x3F);
//         }
//         else if (0x10000 <= code_point && code_point <= 0x10FFFF) {
//             /* U+10000 to U+10FFFF */
//             (*wtf8_string)[result_length++] = 0xF0 | (code_point >> 18);
//             (*wtf8_string)[result_length++] =
//                 0x80 | ((code_point >> 12) & 0x3F);
//             (*wtf8_string)[result_length++] = 0x80 | ((code_point >> 6) &
//             0x3F);
//             (*wtf8_string)[result_length++] = 0x80 | (code_point & 0x3F);
//         }
//     }

//     return result_length;
// }

uint32
decode_wtf8(uint8 *bytes, uint32 bytes_length, uint32 *code_points,
            uint32 *code_points_length, uint8 *target_bytes, encoding_flag flag)
{
    uint32 i = 0, j = 0, k = 0;
    uint32 total_target_byte_count = 0, target_byte_count;
    uint32_t code_point;
    uint8_t byte, byte2, byte3, byte4;

    while (i < bytes_length) {
        byte = bytes[i++];
        if (byte <= 0x7F) {
            code_point = byte;
            target_byte_count = 1;
        }
        else if (byte >= 0xC2 && byte <= 0xDF && i < bytes_length) {
            byte2 = bytes[i++];
            code_point = ((byte & 0x1F) << 6) + (byte2 & 0x3F);
            target_byte_count = 2;
        }
        else if (byte >= 0xE0 && byte <= 0xEF && i + 1 < bytes_length) {
            byte2 = bytes[i++];
            byte3 = bytes[i++];
            code_point =
                ((byte & 0x0F) << 12) + ((byte2 & 0x3F) << 6) + (byte3 & 0x3F);
            target_byte_count = 3;
        }
        else if (byte >= 0xF0 && byte <= 0xF4 && i + 2 < bytes_length) {
            byte2 = bytes[i++];
            byte3 = bytes[i++];
            byte4 = bytes[i++];
            code_point = ((byte & 0x07) << 18) + ((byte2 & 0x3F) << 12)
                         + ((byte3 & 0x3F) << 6) + (byte4 & 0x3F);
            target_byte_count = 4;
        }
        if (is_isolated_surrogate(code_point)) {
            if (flag == UTF8) {
                return -1;
            }
            else if (flag == WTF8) {
                if (target_bytes) {
                    for (k = 0; k < target_byte_count; k++) {
                        *(target_bytes + k + total_target_byte_count) =
                            *(bytes + i - target_byte_count + k);
                    }
                }
                total_target_byte_count += target_byte_count;
            }
            else if (flag == LOSSY_UTF8) {
                code_point = 0xFFFD;
                if (target_bytes) {
                    *(target_bytes + total_target_byte_count) =
                        0xE0 | (code_point >> 12);
                    *(target_bytes + total_target_byte_count + 1) =
                        0x80 | ((code_point >> 6) & 0x3F);
                    *(target_bytes + total_target_byte_count + 2) =
                        0x80 | (code_point & 0x3F);
                }
                total_target_byte_count += 3;
            }
        }
        else {
            if (target_bytes) {
                for (k = 0; k < target_byte_count; k++) {
                    *(target_bytes + k + total_target_byte_count) =
                        *(bytes + i - target_byte_count + k);
                }
            }
            total_target_byte_count += target_byte_count;
        }

        if (code_points) {
            code_points[j++] = code_point;
        }
    }

    if (code_points_length) {
        *code_points_length = j;
    }

    return total_target_byte_count;
}
