/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "string_object.h"

WASMStringWTF8 *
wasm_stringwtf8_obj_new(uint8 *target_bytes, uint32 length)
{
    WASMStringWTF8 *string_obj;

    if (!(string_obj = wasm_runtime_malloc(sizeof(WASMStringWTF8)))) {
        return NULL;
    }
    if (!target_bytes) {
        return NULL;
    }

    string_obj->string_bytes = target_bytes;
    string_obj->length = length;
    string_obj->is_const = false;

    return string_obj;
}

WASMStringWTF16 *
wasm_stringwtf16_obj_new(uint16 *target_bytes, uint32 length)
{
    WASMStringWTF16 *string_obj;

    if (!(string_obj = wasm_runtime_malloc(sizeof(WASMStringWTF16)))) {
        return NULL;
    }
    if (!target_bytes) {
        return NULL;
    }

    string_obj->string_bytes = target_bytes;
    string_obj->length = length;

    return string_obj;
}

WASMStringrefObjectRef
wasm_stringref_obj_new_with_8bit_embedder(struct WASMExecEnv *exec_env,
                                          uint8 *target_bytes, uint32 length)
{
    WASMStringWTF8 *str_obj;
    WASMStringrefObjectRef stringref_obj;

    str_obj = wasm_stringwtf8_obj_new(target_bytes, length);
    stringref_obj = wasm_stringref_obj_new(exec_env, str_obj);

    return stringref_obj;
}

WASMStringrefObjectRef
wasm_stringref_obj_new_with_16bit_embedder(struct WASMExecEnv *exec_env,
                                           uint16 *target_bytes, uint32 length)
{
    WASMStringWTF8 *str_obj;
    WASMStringrefObjectRef stringref_obj;
    uint8 *string_bytes;
    uint32 *code_points;
    int32 code_point_length, target_bytes_length;

    code_points = encode_codepoints_by_16bit_bytes(target_bytes, length,
                                                   &code_point_length);
    string_bytes = encode_8bit_bytes_by_codepoints(
        code_points, code_point_length, &target_bytes_length);

    str_obj = wasm_stringwtf8_obj_new(string_bytes, target_bytes_length);
    stringref_obj = wasm_stringref_obj_new(exec_env, str_obj);

    if (target_bytes) {
        wasm_runtime_free(target_bytes);
    }
    if (code_points) {
        wasm_runtime_free(code_points);
    }

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
    string_obj = (WASMStringWTF8 *)wasm_stringview_wtf8_obj_get_value(
        stringview_wtf8_obj);
    return string_obj->string_bytes;
}

WASMStringviewWTF8ObjectRef
wasm_stringview_wtf8_obj_new_by_stringref(struct WASMExecEnv *exec_env,
                                          WASMStringrefObjectRef stringref_obj)
{
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj;

    stringview_wtf8_obj = wasm_stringview_wtf8_obj_new(
        exec_env, wasm_stringref_obj_get_value(stringref_obj));

    return stringview_wtf8_obj;
}

int32
wasm_stringview_wtf16_obj_get_length(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj)
{
    WASMStringWTF16 *string_obj;
    string_obj = (WASMStringWTF16 *)wasm_stringview_wtf16_obj_get_value(
        stringview_wtf16_obj);
    return string_obj->length;
}

uint16 *
wasm_stringview_wtf16_obj_get_bytes(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj)
{
    WASMStringWTF16 *string_obj;
    string_obj = (WASMStringWTF16 *)wasm_stringview_wtf16_obj_get_value(
        stringview_wtf16_obj);
    return string_obj->string_bytes;
}

int16
wasm_stringview_wtf16_obj_get_codeunit_at_pos(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj, int32 pos)
{
    int32 code_units_len;
    uint16 *code_units;
    int16 target_code_unit;

    code_units_len = wasm_stringview_wtf16_obj_get_length(stringview_wtf16_obj);
    if (pos >= code_units_len) {
        return -1;
    }
    code_units = wasm_stringview_wtf16_obj_get_bytes(stringview_wtf16_obj);
    target_code_unit = (int16)*(code_units + pos);

    return target_code_unit;
}

WASMStringviewWTF16ObjectRef
wasm_stringview_wtf16_obj_new_by_stringref(struct WASMExecEnv *exec_env,
                                           WASMStringrefObjectRef stringref_obj)
{
    uint8 *wtf8_bytes;
    uint16 *wtf16_bytes;
    int32 wtf8_bytes_length, wtf16_bytes_length;
    WASMStringWTF16 *string_obj;
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj;

    wtf8_bytes = wasm_stringref_obj_get_bytes(stringref_obj);
    wtf8_bytes_length = wasm_stringref_obj_get_length(stringref_obj);
    wtf16_bytes = (uint16 *)encode_target_bytes_by_8bit_bytes_with_flag(
        wtf8_bytes, wtf8_bytes_length, &wtf16_bytes_length, WTF16);
    string_obj = wasm_stringwtf16_obj_new(wtf16_bytes, wtf16_bytes_length);
    stringview_wtf16_obj = wasm_stringview_wtf16_obj_new(exec_env, string_obj);

    return stringview_wtf16_obj;
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
