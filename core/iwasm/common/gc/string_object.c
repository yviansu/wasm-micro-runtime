/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "string_object.h"

WASMString *
wasm_string_obj_new(void *pointer, uint32 length, bool is_const,
                    encoding_flag flag)
{
    WASMString *string_obj;
    uint64 string_size;
    uint32 i, wtf8_length = 0;

    if (!(string_obj = wasm_runtime_malloc(sizeof(WASMString)))) {
        return NULL;
    }

    if (length > 0) {
        string_size = sizeof(uint8) * (uint64)length;
        if (!(string_obj->string_bytes = wasm_runtime_malloc(string_size))) {
            return NULL;
        }

        if (flag == WTF8 || flag == UTF8) {
            for (i = 0; i < length; i++) {
                *(string_obj->string_bytes + i) = *(char *)(pointer + i);
            }
            wtf8_length = length;
        }
    }
    else {
        wtf8_length = 0;
        string_obj->string_bytes = NULL;
    }

    string_obj->length = wtf8_length;
    string_obj->is_const = is_const;

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

    strncpy(value, string_obj->string_bytes, str_len);
    value[str_len] = '\0';

    return true;
}