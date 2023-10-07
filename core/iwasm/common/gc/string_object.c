/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "string_object.h"

void
wasm_stringref_obj_finalizer(WASMStringrefObjectRef stringref_obj, void *data)
{
    WASMStringWTF8 *string_obj =
        (WASMStringWTF8 *)wasm_stringref_obj_get_value(stringref_obj);
    if (string_obj && !(string_obj->is_const)) {
        if (string_obj->string_bytes) {
            wasm_runtime_free(string_obj->string_bytes);
        }
        wasm_runtime_free(string_obj);
    }
}

void
wasm_stringview_wtf8_obj_finalizer(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj, void *data)
{
    WASMStringWTF8 *string_obj =
        (WASMStringWTF8 *)wasm_stringview_wtf8_obj_get_value(
            stringview_wtf8_obj);
    if (string_obj && !(string_obj->is_const)) {
        if (string_obj->string_bytes) {
            wasm_runtime_free(string_obj->string_bytes);
        }
        wasm_runtime_free(string_obj);
    }
}

void
wasm_stringview_wtf16_obj_finalizer(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj, void *data)
{
    WASMStringWTF16 *string_obj =
        (WASMStringWTF16 *)wasm_stringview_wtf16_obj_get_value(
            stringview_wtf16_obj);
    if (string_obj) {
        if (string_obj->string_bytes) {
            wasm_runtime_free(string_obj->string_bytes);
        }
        wasm_runtime_free(string_obj);
    }
}

void
wasm_stringview_iter_obj_finalizer(
    WASMStringviewIterObjectRef stringview_iter_obj, void *data)
{
    WASMStringWTF8 *string_obj =
        (WASMStringWTF8 *)wasm_stringview_iter_obj_get_value(
            stringview_iter_obj);
    if (string_obj && !(string_obj->is_const)) {
        if (string_obj->string_bytes) {
            wasm_runtime_free(string_obj->string_bytes);
        }
        wasm_runtime_free(string_obj);
    }
}

static WASMStringWTF8 *
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

static WASMStringWTF16 *
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

static int32
wasm_stringref_obj_get_length(WASMStringrefObjectRef stringref_obj)
{
    WASMStringWTF8 *string_obj = NULL;
    int32 length = 0;

    string_obj = (WASMStringWTF8 *)wasm_stringref_obj_get_value(stringref_obj);
    if (string_obj) {
        length = string_obj->length;
    }

    return length;
}

static uint8 *
wasm_stringref_obj_get_bytes(WASMStringrefObjectRef stringref_obj)
{
    WASMStringWTF8 *string_obj = NULL;
    uint8 *string_bytes = NULL;

    string_obj = (WASMStringWTF8 *)wasm_stringref_obj_get_value(stringref_obj);
    if (string_obj) {
        string_bytes = string_obj->string_bytes;
    }

    return string_bytes;
}

static int32
wasm_stringview_iter_obj_get_length(
    WASMStringviewIterObjectRef stringview_iter_obj)
{
    WASMStringWTF8 *string_obj = NULL;
    int32 length = 0;

    string_obj = (WASMStringWTF8 *)wasm_stringview_iter_obj_get_value(
        stringview_iter_obj);
    if (string_obj) {
        length = string_obj->length;
    }

    return length;
}

static uint8 *
wasm_stringview_iter_obj_get_bytes(
    WASMStringviewIterObjectRef stringview_iter_obj)
{
    WASMStringWTF8 *string_obj = NULL;
    uint8 *string_bytes = NULL;

    string_obj = (WASMStringWTF8 *)wasm_stringview_iter_obj_get_value(
        stringview_iter_obj);
    if (string_obj) {
        string_bytes = string_obj->string_bytes;
    }

    return string_bytes;
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
wasm_stringview_wtf8_obj_get_length(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj)
{
    WASMStringWTF8 *string_obj = NULL;
    int32 length = 0;

    string_obj = (WASMStringWTF8 *)wasm_stringview_wtf8_obj_get_value(
        stringview_wtf8_obj);
    if (string_obj) {
        length = string_obj->length;
    }

    return length;
}

uint8 *
wasm_stringview_wtf8_obj_get_bytes(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj)
{
    WASMStringWTF8 *string_obj = NULL;
    uint8 *string_bytes = NULL;

    string_obj = (WASMStringWTF8 *)wasm_stringview_wtf8_obj_get_value(
        stringview_wtf8_obj);
    if (string_obj) {
        string_bytes = string_obj->string_bytes;
    }
    return string_bytes;
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
    WASMStringWTF16 *string_obj = NULL;
    int32 length = 0;

    string_obj = (WASMStringWTF16 *)wasm_stringview_wtf16_obj_get_value(
        stringview_wtf16_obj);
    if (string_obj) {
        length = string_obj->length;
    }

    return length;
}

uint16 *
wasm_stringview_wtf16_obj_get_bytes(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj)
{
    WASMStringWTF16 *string_obj = NULL;
    uint16 *string_bytes = NULL;

    string_obj = (WASMStringWTF16 *)wasm_stringview_wtf16_obj_get_value(
        stringview_wtf16_obj);
    if (string_obj) {
        string_bytes = string_obj->string_bytes;
    }

    return string_bytes;
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
    target_code_unit = (int16) * (code_units + pos);

    return target_code_unit;
}

WASMStringviewWTF16ObjectRef
wasm_stringview_wtf16_obj_new_by_stringref(struct WASMExecEnv *exec_env,
                                           WASMStringrefObjectRef stringref_obj)
{
    uint8 *target_bytes;
    uint16 *target_code_units;
    int32 target_bytes_length, target_code_units_length;
    WASMStringWTF16 *string_obj;
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj;

    target_bytes = wasm_stringref_obj_get_bytes(stringref_obj);
    target_bytes_length = wasm_stringref_obj_get_length(stringref_obj);
    target_code_units = (uint16 *)encode_target_bytes_by_8bit_bytes_with_flag(
        target_bytes, target_bytes_length, &target_code_units_length, WTF16);
    string_obj =
        wasm_stringwtf16_obj_new(target_code_units, target_code_units_length);
    stringview_wtf16_obj = wasm_stringview_wtf16_obj_new(exec_env, string_obj);

    return stringview_wtf16_obj;
}

WASMStringviewIterObjectRef
wasm_stringview_iter_obj_new_by_stringref(struct WASMExecEnv *exec_env,
                                          WASMStringrefObjectRef stringref_obj)
{
    WASMStringviewIterObjectRef stringview_iter_obj;

    stringview_iter_obj = wasm_stringview_iter_obj_new(
        exec_env, wasm_stringref_obj_get_value(stringref_obj), 0);

    return stringview_iter_obj;
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

int32
wasm_stringref_obj_measure_from_start(WASMStringrefObjectRef stringref_obj,
                                      encoding_flag flag, uint32 start)
{
    int32 string_bytes_length, target_bytes_length;
    uint8 *string_bytes;

    string_bytes = wasm_stringref_obj_get_bytes(stringref_obj);
    string_bytes_length = wasm_stringref_obj_get_length(stringref_obj);
    target_bytes_length = calculate_encoded_code_units_by_8bit_bytes_with_flag(
        string_bytes + start, string_bytes_length - start, flag);

    return target_bytes_length;
}

int32
wasm_stringref_obj_measure(WASMStringrefObjectRef stringref_obj,
                           encoding_flag flag)
{
    return wasm_stringref_obj_measure_from_start(stringref_obj, flag, 0);
}

void *
wasm_stringref_obj_encode_with_flag(WASMStringrefObjectRef stringref_obj,
                                    encoding_flag flag)
{
    int32 string_bytes_length, target_bytes_length;
    uint8 *string_bytes;
    void *target_bytes = NULL;

    string_bytes = wasm_stringref_obj_get_bytes(stringref_obj);
    string_bytes_length = wasm_stringref_obj_get_length(stringref_obj);

    target_bytes = encode_target_bytes_by_8bit_bytes_with_flag(
        string_bytes, string_bytes_length, &target_bytes_length, flag);

    return target_bytes;
}

WASMStringrefObjectRef
wasm_stringref_obj_concat(struct WASMExecEnv *exec_env,
                          WASMStringrefObjectRef stringref_obj1,
                          WASMStringrefObjectRef stringref_obj2)
{
    uint8 *string_bytes1, *string_bytes2, *target_bytes = NULL;
    int32 string_bytes_length1, string_bytes_length2, target_bytes_length;
    encoding_flag flag = WTF8;
    WASMStringrefObjectRef stringref_obj;

    string_bytes1 = wasm_stringref_obj_get_bytes(stringref_obj1);
    string_bytes_length1 = wasm_stringref_obj_get_length(stringref_obj1);
    string_bytes2 = wasm_stringref_obj_get_bytes(stringref_obj2);
    string_bytes_length2 = wasm_stringref_obj_get_length(stringref_obj2);
    target_bytes =
        concat_8bit_bytes(string_bytes1, string_bytes_length1, string_bytes2,
                          string_bytes_length2, &target_bytes_length, flag);
    stringref_obj = wasm_stringref_obj_new_with_8bit_embedder(
        exec_env, target_bytes, target_bytes_length);

    return stringref_obj;
}

int32
wasm_stringref_obj_is_usv_sequence(WASMStringrefObjectRef stringref_obj)
{
    int32 target_bytes_length, is_usv_sequence, string_bytes_length;
    uint8 *string_bytes;
    encoding_flag flag = WTF8;

    string_bytes = wasm_stringref_obj_get_bytes(stringref_obj);
    string_bytes_length = wasm_stringref_obj_get_length(stringref_obj);
    target_bytes_length = calculate_encoded_code_units_by_8bit_bytes_with_flag(
        string_bytes, string_bytes_length, flag);
    if (target_bytes_length == -1) {
        is_usv_sequence = 0;
    }
    else {
        is_usv_sequence = 1;
    }

    return is_usv_sequence;
}

uint32
wasm_stringview_iter_obj_get_next_codepoint(
    WASMStringviewIterObjectRef stringview_iter_obj)
{
    uint32 code_point;
    uint8 *string_bytes;
    int32 string_bytes_length, cur_pos, target_pos;

    string_bytes = wasm_stringview_iter_obj_get_bytes(stringview_iter_obj);
    string_bytes_length =
        wasm_stringview_iter_obj_get_length(stringview_iter_obj);
    cur_pos = wasm_stringview_iter_obj_get_pos(stringview_iter_obj);
    target_pos = wtf8_string_bytes_iter_next(string_bytes, string_bytes_length,
                                             cur_pos, &code_point);

    wasm_stringview_iter_obj_update_pos(stringview_iter_obj, target_pos);

    return code_point;
}

uint32
wasm_stringview_iter_obj_advance(
    WASMStringviewIterObjectRef stringview_iter_obj, uint32 code_points_count)
{
    uint32 code_points_consumed;
    uint8 *string_bytes;
    int32 string_bytes_length, cur_pos = 0, target_pos = 0;

    string_bytes = wasm_stringview_iter_obj_get_bytes(stringview_iter_obj);
    string_bytes_length =
        wasm_stringview_iter_obj_get_length(stringview_iter_obj);
    cur_pos = wasm_stringview_iter_obj_get_pos(stringview_iter_obj);

    target_pos = wtf8_string_bytes_iter_advance(
        string_bytes, string_bytes_length, cur_pos, code_points_count,
        &code_points_consumed);

    wasm_stringview_iter_obj_update_pos(stringview_iter_obj, target_pos);

    return code_points_consumed;
}

uint32
wasm_stringview_iter_obj_rewind(WASMStringviewIterObjectRef stringview_iter_obj,
                                uint32 code_points_count)
{
    uint32 code_points_consumed;
    uint8 *string_bytes;
    int32 string_bytes_length, cur_pos = 0, target_pos = 0;

    string_bytes = wasm_stringview_iter_obj_get_bytes(stringview_iter_obj);
    string_bytes_length =
        wasm_stringview_iter_obj_get_length(stringview_iter_obj);
    cur_pos = wasm_stringview_iter_obj_get_pos(stringview_iter_obj);

    target_pos = wtf8_string_bytes_iter_rewind(
        string_bytes, string_bytes_length, cur_pos, code_points_count,
        &code_points_consumed);

    wasm_stringview_iter_obj_update_pos(stringview_iter_obj, target_pos);

    return code_points_consumed;
}

WASMStringrefObjectRef
wasm_stringview_iter_obj_slice(struct WASMExecEnv *exec_env,
                               WASMStringviewIterObjectRef stringview_iter_obj,
                               uint32 code_points_count)
{

    uint8 *string_bytes;
    int32 string_bytes_length, cur_pos, end_pos;
    WASMStringrefObjectRef stringref_obj;

    string_bytes = wasm_stringview_iter_obj_get_bytes(stringview_iter_obj);
    string_bytes_length =
        wasm_stringview_iter_obj_get_length(stringview_iter_obj);
    cur_pos = wasm_stringview_iter_obj_get_pos(stringview_iter_obj);

    end_pos = wtf8_string_bytes_iter_slice(string_bytes, string_bytes_length,
                                           cur_pos, code_points_count);
    stringref_obj = wasm_stringref_obj_new_with_8bit_embedder(
        exec_env, string_bytes + cur_pos, end_pos - cur_pos);

    return stringref_obj;
}
