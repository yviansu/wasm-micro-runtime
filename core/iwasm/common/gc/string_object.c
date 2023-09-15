/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "string_object.h"

WASMString *
wasm_string_obj_new(void *pointer, uint32 length, encoding_flag flag)
{
    WASMString *string_obj;
    uint64 string_size;
    uint32 i;
    bool is_success;

    if (!(string_obj = wasm_runtime_malloc(sizeof(WASMString)))) {
        return NULL;
    }

    string_size = sizeof(uint8) * (uint64)length;
    if (!(string_obj->string_byte = wasm_runtime_malloc(string_size))) {
        return NULL;
    }
    string_obj->length = length;

    if (flag == WTF8) {
        for (i = 0; i < length; i++) {
            *(string_obj->string_byte + i) = *(char *)(pointer + i);
        }
    }
    else if (flag == UTF8) {
        is_success = utf8_to_wtf8((char *)pointer,
                                  (char *)string_obj->string_byte, length);
        if (!is_success) {
            LOG_ERROR("error: Failed to transcode from UTF-8 to WTF-8.");
        }
    }

    return string_obj;
}

uint32
wasm_get_stringref_length(WASMStringrefObjectRef stringref_obj)
{
    WASMString *string_obj = stringref_obj->pointer;
    uint32 str_len = string_obj->length;

    return str_len;
}

bool
wasm_get_stringref_value(WASMStringrefObjectRef stringref_obj, char *value)
{
    WASMString *string_obj = stringref_obj->pointer;
    uint32 str_len = string_obj->length;

    strncpy(value, string_obj->string_byte, str_len);
    value[str_len] = '\0';

    return true;
}