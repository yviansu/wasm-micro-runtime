/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "string_object.h"

WASMStringWTF8 *
wasm_string_obj_new(uint8 *target_bytes, uint32 length)
{
    WASMStringWTF8 *string_obj;

    if (!(string_obj = wasm_runtime_malloc(sizeof(WASMStringWTF8)))) {
        return NULL;
    }

    string_obj->string_bytes = target_bytes;
    string_obj->length = length;
    string_obj->is_const = false;

    return string_obj;
}

WASMStringrefObjectRef
wasm_stringref_obj_new_with_embedder(struct WASMExecEnv *exec_env,
                                     uint8 *target_bytes, uint32 length)
{
    WASMStringWTF8 *str_obj;
    WASMStringrefObjectRef stringref_obj;

    str_obj = wasm_string_obj_new(target_bytes, length);
    stringref_obj = wasm_stringref_obj_new(exec_env, str_obj);

    return stringref_obj;
}

int32
wasm_stringref_obj_get_length(WASMStringrefObjectRef stringref_obj)
{
    WASMStringWTF8 *string_obj;
    string_obj = (WASMStringWTF8 *)wasm_stringref_obj_get_value(stringref_obj);
    return string_obj->length;
}

uint8 *
wasm_stringref_obj_get_bytes(WASMStringrefObjectRef stringref_obj)
{
    WASMStringWTF8 *string_obj;
    string_obj = (WASMStringWTF8 *)wasm_stringref_obj_get_value(stringref_obj);
    return string_obj->string_bytes;
}

int32
wasm_stringview_wtf8_obj_get_length(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj)
{
    WASMStringWTF8 *string_obj;
    string_obj = (WASMStringWTF8 *)wasm_stringview_wtf8_obj_get_value(
        stringview_wtf8_obj);
    return string_obj->length;
}

uint8 *
wasm_stringview_wtf8_obj_get_bytes(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj)
{
    WASMStringWTF8 *string_obj;
    string_obj =
        (WASMStringWTF8 *)wasm_stringref_obj_get_value(stringview_wtf8_obj);
    return string_obj->string_bytes;
}

int32
wasm_stringview_iter_obj_get_length(
    WASMStringviewIterObjectRef stringview_iter_obj)
{
    WASMStringWTF8 *string_obj;
    string_obj = (WASMStringWTF8 *)wasm_stringview_iter_obj_get_value(
        stringview_iter_obj);
    return string_obj->length;
}

uint8 *
wasm_stringview_iter_obj_get_bytes(
    WASMStringviewIterObjectRef stringview_iter_obj)
{
    WASMStringWTF8 *string_obj;
    string_obj = (WASMStringWTF8 *)wasm_stringview_iter_obj_get_value(
        stringview_iter_obj);
    return string_obj->string_bytes;
}

int32
wasm_stringref_obj_eq(WASMStringrefObjectRef stringref_obj1,
                      WASMStringrefObjectRef stringref_obj2)
{
    WASMStringWTF8 *string_obj1, *string_obj2;
    int32 string_length1, string_length2, i;
    uint8 *string_bytes1, *string_bytes2;

    string_obj1 = (WASMStringWTF8 *)(stringref_obj1->str_obj);
    string_obj2 = (WASMStringWTF8 *)(stringref_obj2->str_obj);

    if (string_obj1 == string_obj2) {
        return 1;
    }

    if (string_obj1 == NULL || string_obj2 == NULL) {
        return 0;
    }

    string_length1 = string_obj1->length;
    string_length2 = string_obj2->length;
    string_bytes1 = string_obj1->string_bytes;
    string_bytes2 = string_obj2->string_bytes;

    if (string_length1 != string_length2) {
        return 0;
    }

    for (i = 0; i < string_length1; i++) {
        if (string_bytes1[i] != string_bytes2[i]) {
            return 0;
        }
    }

    return 1;
}
