/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "string_object.h"

/******************* gc finalizer *****************/

void
wasm_stringref_obj_finalizer(WASMStringrefObjectRef stringref_obj, void *data)
{
    WASMStringWTF8 *string_obj =
        (WASMStringWTF8 *)wasm_stringref_obj_get_value(stringref_obj);
    WASMRttTypeRef rtt_type =
        (WASMRttTypeRef)wasm_object_header((WASMObjectRef)stringref_obj);

    if (string_obj) {
        string_obj->ref_count--;
    }
    if (string_obj && !(string_obj->is_const) && string_obj->ref_count == 0) {
        if (string_obj->string_bytes) {
            wasm_runtime_free(string_obj->string_bytes);
        }
        wasm_runtime_free(string_obj);
    }
    if (rtt_type) {
        wasm_runtime_free(rtt_type);
    }
}

void
wasm_stringview_wtf8_obj_finalizer(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj, void *data)
{
    WASMStringWTF8 *string_obj =
        (WASMStringWTF8 *)wasm_stringview_wtf8_obj_get_value(
            stringview_wtf8_obj);
    WASMRttTypeRef rtt_type =
        (WASMRttTypeRef)wasm_object_header((WASMObjectRef)stringview_wtf8_obj);

    if (string_obj) {
        string_obj->ref_count--;
    }
    if (string_obj && !(string_obj->is_const) && string_obj->ref_count == 0) {
        if (string_obj->string_bytes) {
            wasm_runtime_free(string_obj->string_bytes);
        }
        wasm_runtime_free(string_obj);
    }
    if (rtt_type) {
        wasm_runtime_free(rtt_type);
    }
}

void
wasm_stringview_wtf16_obj_finalizer(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj, void *data)
{
    WASMStringWTF16 *string_obj =
        (WASMStringWTF16 *)wasm_stringview_wtf16_obj_get_value(
            stringview_wtf16_obj);
    WASMRttTypeRef rtt_type =
        (WASMRttTypeRef)wasm_object_header((WASMObjectRef)stringview_wtf16_obj);

    if (string_obj) {
        string_obj->ref_count--;
    }
    if (string_obj && string_obj->ref_count == 0) {
        if (string_obj->string_bytes) {
            wasm_runtime_free(string_obj->string_bytes);
        }
        wasm_runtime_free(string_obj);
    }
    if (rtt_type) {
        wasm_runtime_free(rtt_type);
    }
}

void
wasm_stringview_iter_obj_finalizer(
    WASMStringviewIterObjectRef stringview_iter_obj, void *data)
{
    WASMStringWTF8 *string_obj =
        (WASMStringWTF8 *)wasm_stringview_iter_obj_get_value(
            stringview_iter_obj);
    WASMRttTypeRef rtt_type =
        (WASMRttTypeRef)wasm_object_header((WASMObjectRef)stringview_iter_obj);

    if (string_obj) {
        string_obj->ref_count--;
    }
    if (string_obj && !(string_obj->is_const) && string_obj->ref_count == 0) {
        if (string_obj->string_bytes) {
            wasm_runtime_free(string_obj->string_bytes);
        }
        wasm_runtime_free(string_obj);
    }
    if (rtt_type) {
        wasm_runtime_free(rtt_type);
    }
}

/******************* functional functions *****************/

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

static WASMStringWTF8 *
wasm_stringwtf8_obj_new(uint8 *bytes, uint32 length)
{
    WASMStringWTF8 *string_obj;

    if (!(string_obj = wasm_runtime_malloc(sizeof(WASMStringWTF8)))) {
        return NULL;
    }
    if (length > 0 && !bytes) {
        return NULL;
    }
    string_obj->string_bytes = bytes;
    string_obj->length = length;
    string_obj->is_const = false;
    string_obj->ref_count = 1;

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
    string_obj->ref_count = 1;

    return string_obj;
}

WASMStringrefObjectRef
wasm_stringref_obj_new_with_8bit_embedder(struct WASMExecEnv *exec_env,
                                          uint8 *bytes, uint32 bytes_length)
{
    WASMStringWTF8 *str_obj;
    WASMStringrefObjectRef stringref_obj;

    str_obj = wasm_stringwtf8_obj_new(bytes, bytes_length);
    if (!str_obj) {
        return NULL;
    }
    stringref_obj = wasm_stringref_obj_new(exec_env, str_obj);

    return stringref_obj;
}

WASMStringrefObjectRef
wasm_stringref_obj_new_with_16bit_embedder(struct WASMExecEnv *exec_env,
                                           uint16 *bytes, uint32 bytes_length)
{
    WASMStringWTF8 *str_obj;
    WASMStringrefObjectRef stringref_obj;
    uint8 *string_bytes;
    uint32 *code_points;
    int32 code_point_length, target_bytes_length;

    code_points = encode_codepoints_by_16bit_bytes(bytes, bytes_length,
                                                   &code_point_length);
    string_bytes = encode_8bit_bytes_by_codepoints(
        code_points, code_point_length, &target_bytes_length);

    str_obj = wasm_stringwtf8_obj_new(string_bytes, target_bytes_length);
    if (!str_obj) {
        return NULL;
    }
    stringref_obj = wasm_stringref_obj_new(exec_env, str_obj);

    if (code_points) {
        wasm_runtime_free(code_points);
    }

    return stringref_obj;
}

/******************* opcode functions *****************/

/* string.const */
WASMStringrefObjectRef
wasm_stringref_obj_new_with_const(struct WASMExecEnv *exec_env,
                                  WASMStringWTF8 *str_obj)
{
    WASMStringrefObjectRef stringref_obj;

    str_obj->ref_count++;
    stringref_obj = wasm_stringref_obj_new(exec_env, str_obj);

    return stringref_obj;
}

/* string.new_xx8 */
WASMStringrefObjectRef
wasm_stringref_obj_new_with_8bit_memory(struct WASMExecEnv *exec_env,
                                        void *maddr, uint32 bytes_length,
                                        encoding_flag flag)
{
    uint8 *target_bytes;
    int32 target_bytes_length;
    WASMStringrefObjectRef stringref_obj;

    target_bytes = encode_8bit_bytes_by_8bit_bytes_with_flag(
        maddr, bytes_length, &target_bytes_length, flag);
    if (target_bytes_length == -1) {
        return NULL;
    }
    stringref_obj = wasm_stringref_obj_new_with_8bit_embedder(
        exec_env, target_bytes, target_bytes_length);

    return stringref_obj;
}

/* string.new_wtf16 */
WASMStringrefObjectRef
wasm_stringref_obj_new_with_16bit_memory(struct WASMExecEnv *exec_env,
                                         void *maddr, uint32 bytes_length)
{
    uint16 *target_code_units;
    uint32 i;
    WASMStringrefObjectRef stringref_obj;

    if (!(target_code_units =
              wasm_runtime_malloc(sizeof(uint16) * bytes_length))) {
        return NULL;
    }
    for (i = 0; i < bytes_length; i++) {
        target_code_units[i] = LOAD_I16(maddr + (i * 2));
    }
    stringref_obj = wasm_stringref_obj_new_with_16bit_embedder(
        exec_env, target_code_units, bytes_length);
    if (target_code_units) {
        wasm_runtime_free(target_code_units);
    }

    return stringref_obj;
}

/* string.measure */
int32
wasm_stringref_obj_measure(WASMStringrefObjectRef stringref_obj,
                           encoding_flag flag)
{
    return wasm_stringref_obj_measure_from_start(stringref_obj, flag, 0);
}

/* string.encode_xx8 */
int32
wasm_stringref_obj_encode_with_8bit_memory(struct WASMExecEnv *exec_env,
                                           void *maddr,
                                           WASMStringrefObjectRef stringref_obj,
                                           encoding_flag flag)
{
    int32 bytes_length;
    uint8 *target_bytes;

    bytes_length = wasm_stringref_obj_measure(stringref_obj, flag);
    target_bytes =
        (uint8 *)wasm_stringref_obj_encode_with_flag(stringref_obj, flag);
    if (!target_bytes) {
        return -2;
    }
    if (bytes_length == -1) {
        return bytes_length;
    }
    bh_memcpy_s(maddr, bytes_length, target_bytes, bytes_length);
    if (target_bytes) {
        wasm_runtime_free(target_bytes);
    }

    return bytes_length;
}

/* string.encode_wtf16 */
int32
wasm_stringref_obj_encode_with_16bit_memory(
    struct WASMExecEnv *exec_env, void *maddr,
    WASMStringrefObjectRef stringref_obj, encoding_flag flag)
{
    int32 bytes_length, i;
    uint16 *target_code_units;

    bytes_length = wasm_stringref_obj_measure(stringref_obj, flag);
    target_code_units =
        (uint16 *)wasm_stringref_obj_encode_with_flag(stringref_obj, flag);
    if (!target_code_units) {
        return -1;
    }
    for (i = 0; i < bytes_length; i++) {
        STORE_U16(maddr + (i * 2), target_code_units[i]);
    }
    if (target_code_units) {
        wasm_runtime_free(target_code_units);
    }
    return bytes_length;
}

/* string.concat */
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

/* string.eq */
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

/* string.is_usv_sequence */
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

/* string.as_wtf8 */
WASMStringviewWTF8ObjectRef
wasm_stringview_wtf8_obj_new_by_stringref(struct WASMExecEnv *exec_env,
                                          WASMStringrefObjectRef stringref_obj)
{
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj;
    WASMStringWTF8 *string_obj;

    string_obj = (WASMStringWTF8 *)wasm_stringref_obj_get_value(stringref_obj);
    string_obj->ref_count++;
    stringview_wtf8_obj = wasm_stringview_wtf8_obj_new(exec_env, string_obj);

    return stringview_wtf8_obj;
}

/* stringview_wtf8.advance */
int32
wasm_stringview_wtf8_obj_advance(
    struct WASMExecEnv *exec_env,
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj, uint32 pos,
    uint32 bytes_length)
{
    uint8 *string_bytes;
    int32 string_bytes_length;
    uint32 next_pos;

    string_bytes = wasm_stringview_wtf8_obj_get_bytes(stringview_wtf8_obj);
    string_bytes_length =
        wasm_stringview_wtf8_obj_get_length(stringview_wtf8_obj);
    next_pos = wtf8_string_bytes_advance(string_bytes, string_bytes_length, pos,
                                         bytes_length);

    return next_pos;
}

/* stringview_wtf8.encode_xx */
int32
wasm_stringview_wtf8_obj_encode_memory(
    struct WASMExecEnv *exec_env, void *maddr,
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj, uint32 pos,
    uint32 bytes_length, uint32 *next_pos, encoding_flag flag)
{
    uint8 *string_bytes, *target_bytes;
    int32 string_bytes_length, target_bytes_length;
    uint32 start_pos, view_bytes_length;

    string_bytes = wasm_stringview_wtf8_obj_get_bytes(stringview_wtf8_obj);
    string_bytes_length =
        wasm_stringview_wtf8_obj_get_length(stringview_wtf8_obj);

    start_pos =
        wtf8_string_bytes_advance(string_bytes, string_bytes_length, pos, 0);
    *next_pos = wtf8_string_bytes_advance(string_bytes, string_bytes_length,
                                          start_pos, bytes_length);
    view_bytes_length = *next_pos - start_pos;

    target_bytes = encode_8bit_bytes_by_8bit_bytes_with_flag(
        string_bytes + start_pos, view_bytes_length, &target_bytes_length,
        flag);
    if (target_bytes_length == -1) {
        return target_bytes_length;
    }
    bh_memcpy_s(maddr, target_bytes_length, target_bytes, target_bytes_length);

    return target_bytes_length;
}

/* stringview_wtf8.slice */
WASMStringrefObjectRef
wasm_stringview_wtf8_obj_slice(struct WASMExecEnv *exec_env,
                               WASMStringviewWTF8ObjectRef stringview_wtf8_obj,
                               uint32 start, uint32 end)
{
    uint8 *string_bytes, *target_bytes;
    int32 string_bytes_length, target_bytes_length;
    uint32 start_pos, end_pos;
    WASMStringrefObjectRef stringref_obj;

    string_bytes = wasm_stringview_wtf8_obj_get_bytes(stringview_wtf8_obj);
    string_bytes_length =
        wasm_stringview_wtf8_obj_get_length(stringview_wtf8_obj);

    start_pos =
        wtf8_string_bytes_advance(string_bytes, string_bytes_length, start, 0);
    end_pos =
        wtf8_string_bytes_advance(string_bytes, string_bytes_length, end, 0);

    target_bytes_length = end_pos - start_pos;

    if (!(target_bytes =
              wasm_runtime_malloc(sizeof(uint8) * target_bytes_length))) {
        return NULL;
    }
    bh_memcpy_s(target_bytes, target_bytes_length, string_bytes + start_pos,
                target_bytes_length);

    stringref_obj = wasm_stringref_obj_new_with_8bit_embedder(
        exec_env, target_bytes, target_bytes_length);

    return stringref_obj;
}

/* string.as_wtf16 */
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

/* stringview_wtf16.length */
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

/* stringview_wtf16.get_codeunit */
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

/* stringview_wtf16.encode */
int32
wasm_stringview_wtf16_obj_encode_memory(
    struct WASMExecEnv *exec_env, void *maddr,
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj, uint32 pos, uint32 len)
{
    uint32 start_pos, i;
    int32 code_units_length, written_code_units = 0;
    uint16 *target_code_units;

    code_units_length =
        wasm_stringview_wtf16_obj_get_length(stringview_wtf16_obj);
    target_code_units =
        wasm_stringview_wtf16_obj_get_bytes(stringview_wtf16_obj);
    start_pos = wtf16_pos_treatment(pos, code_units_length);

    for (i = 0; i < len; i++) {
        STORE_U16(maddr + (i * 2), target_code_units[i + start_pos]);
        written_code_units++;
    }

    return written_code_units;
}

/* stringview_wtf16.slice */
WASMStringrefObjectRef
wasm_stringview_wtf16_obj_slice(
    struct WASMExecEnv *exec_env,
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj, uint32 start, uint32 end)
{
    uint16 *target_code_units;
    int32 code_units_length;
    uint32 start_pos, end_pos;
    WASMStringrefObjectRef stringref_obj;

    code_units_length =
        wasm_stringview_wtf16_obj_get_length(stringview_wtf16_obj);
    target_code_units =
        wasm_stringview_wtf16_obj_get_bytes(stringview_wtf16_obj);

    start_pos = wtf16_pos_treatment(start, code_units_length);
    end_pos = wtf16_pos_treatment(end, code_units_length);
    stringref_obj = wasm_stringref_obj_new_with_16bit_embedder(
        exec_env, target_code_units + start_pos, end_pos - start_pos);

    return stringref_obj;
}

/* string.as_iter */
WASMStringviewIterObjectRef
wasm_stringview_iter_obj_new_by_stringref(struct WASMExecEnv *exec_env,
                                          WASMStringrefObjectRef stringref_obj)
{
    WASMStringviewIterObjectRef stringview_iter_obj;
    WASMStringWTF8 *string_obj;

    string_obj = (WASMStringWTF8 *)wasm_stringref_obj_get_value(stringref_obj);
    string_obj->ref_count++;
    stringview_iter_obj = wasm_stringview_iter_obj_new(exec_env, string_obj, 0);

    return stringview_iter_obj;
}

/* stringview_iter.next */
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

/* stringview_iter.advance */
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

/* stringview_iter.rewind */
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

/* stringview_iter.slice */
WASMStringrefObjectRef
wasm_stringview_iter_obj_slice(struct WASMExecEnv *exec_env,
                               WASMStringviewIterObjectRef stringview_iter_obj,
                               uint32 code_points_count)
{
    uint8 *string_bytes, *target_bytes;
    int32 string_bytes_length, cur_pos, end_pos, target_bytes_length;
    WASMStringrefObjectRef stringref_obj;

    string_bytes = wasm_stringview_iter_obj_get_bytes(stringview_iter_obj);
    string_bytes_length =
        wasm_stringview_iter_obj_get_length(stringview_iter_obj);
    cur_pos = wasm_stringview_iter_obj_get_pos(stringview_iter_obj);

    end_pos = wtf8_string_bytes_iter_slice(string_bytes, string_bytes_length,
                                           cur_pos, code_points_count);

    target_bytes_length = end_pos - cur_pos;

    if (!(target_bytes =
              wasm_runtime_malloc(sizeof(uint8) * target_bytes_length))) {
        return NULL;
    }
    bh_memcpy_s(target_bytes, target_bytes_length, string_bytes + cur_pos,
                target_bytes_length);

    stringref_obj = wasm_stringref_obj_new_with_8bit_embedder(
        exec_env, target_bytes, target_bytes_length);

    return stringref_obj;
}

/* string.new_xx8_array */
WASMStringrefObjectRef
wasm_stringref_obj_new_with_8bit_array(struct WASMExecEnv *exec_env,
                                       WASMArrayObjectRef array_obj,
                                       uint32 start, uint32 end,
                                       encoding_flag flag)
{
    uint32 bytes_length, arr_length;
    int32 target_bytes_length;
    WASMArrayType *array_type;
    void *arr_addr, *str_addr;
    uint8 *target_bytes;
    WASMStringrefObjectRef stringref_obj;

    array_type =
        (WASMArrayType *)wasm_obj_get_defined_type((WASMObjectRef)array_obj);
    if (array_type->elem_type != PACKED_TYPE_I8) {
        return NULL;
    }
    arr_addr = wasm_array_obj_first_elem_addr(array_obj);
    str_addr = arr_addr + start;
    arr_length = wasm_array_obj_length(array_obj);
    if (end < start || end > arr_length) {
        return NULL;
    }
    bytes_length = end - start;

    target_bytes = encode_8bit_bytes_by_8bit_bytes_with_flag(
        str_addr, bytes_length, &target_bytes_length, flag);

    if (target_bytes_length == -1) {
        if (target_bytes) {
            wasm_runtime_free(target_bytes);
        }
        return NULL;
    }

    stringref_obj = wasm_stringref_obj_new_with_8bit_embedder(
        exec_env, target_bytes, target_bytes_length);

    return stringref_obj;
}

/* string.new_wtf16_array */
WASMStringrefObjectRef
wasm_stringref_obj_new_with_16bit_array(struct WASMExecEnv *exec_env,
                                        WASMArrayObjectRef array_obj,
                                        uint32 start, uint32 end)
{
    uint32 bytes_length, arr_length;
    WASMArrayType *array_type;
    void *arr_addr, *str_addr;
    WASMStringrefObjectRef stringref_obj;

    array_type =
        (WASMArrayType *)wasm_obj_get_defined_type((WASMObjectRef)array_obj);
    if (array_type->elem_type != PACKED_TYPE_I16) {
        return NULL;
    }
    arr_addr = wasm_array_obj_first_elem_addr(array_obj);
    str_addr = arr_addr + start;
    arr_length = wasm_array_obj_length(array_obj);
    if (end < start || end > arr_length) {
        return NULL;
    }
    bytes_length = end - start;

    stringref_obj = wasm_stringref_obj_new_with_16bit_embedder(
        exec_env, str_addr, bytes_length);

    return stringref_obj;
}

/* string.encode_xx8_array */
uint32
wasm_stringref_obj_encode_with_8bit_array(struct WASMExecEnv *exec_env,
                                          WASMStringrefObjectRef stringref_obj,
                                          WASMArrayObjectRef array_obj,
                                          uint32 start, encoding_flag flag)
{
    uint32 arr_length;
    int32 target_bytes_length, i;
    WASMArrayType *array_type;
    uint8 *target_bytes;
    WASMValue array_elem = { 0 };

    array_type =
        (WASMArrayType *)wasm_obj_get_defined_type((WASMObjectRef)array_obj);
    if (array_type->elem_type != PACKED_TYPE_I8) {
        return -2;
    }
    target_bytes_length =
        wasm_stringref_obj_measure_from_start(stringref_obj, flag, start);
    if (target_bytes_length == -1) {
        return target_bytes_length;
    }
    arr_length = wasm_array_obj_length(array_obj);
    if (target_bytes_length > (int32)arr_length) {
        return -2;
    }
    target_bytes = wasm_stringref_obj_encode_with_flag(stringref_obj, flag);

    for (i = 0; i < target_bytes_length; i++) {
        array_elem.i32 = *((uint8 *)target_bytes + start + i);
        wasm_array_obj_set_elem(array_obj, i, &array_elem);
    }

    if (target_bytes) {
        wasm_runtime_free(target_bytes);
    }

    return target_bytes_length;
}

/* string.encode_wtf16_array */
uint32
wasm_stringref_obj_encode_with_16bit_array(struct WASMExecEnv *exec_env,
                                           WASMStringrefObjectRef stringref_obj,
                                           WASMArrayObjectRef array_obj,
                                           uint32 start, encoding_flag flag)
{
    uint32 arr_length;
    int32 target_bytes_length, i;
    WASMArrayType *array_type;
    uint8 *target_bytes;
    WASMValue array_elem = { 0 };

    array_type =
        (WASMArrayType *)wasm_obj_get_defined_type((WASMObjectRef)array_obj);
    if (array_type->elem_type != PACKED_TYPE_I16) {
        return -2;
    }
    target_bytes_length =
        wasm_stringref_obj_measure_from_start(stringref_obj, flag, start);
    if (target_bytes_length == -1) {
        return target_bytes_length;
    }
    arr_length = wasm_array_obj_length(array_obj);
    if (target_bytes_length > (int32)arr_length) {
        return -2;
    }
    target_bytes = wasm_stringref_obj_encode_with_flag(stringref_obj, flag);

    for (i = 0; i < target_bytes_length; i++) {
        array_elem.i32 = *((uint16 *)target_bytes + start + i);
        wasm_array_obj_set_elem(array_obj, i, &array_elem);
    }

    if (target_bytes) {
        wasm_runtime_free(target_bytes);
    }

    return target_bytes_length;
}

/******************* application functions *****************/

void
wasm_stringref_obj_dump(WASMStringrefObjectRef stringref_obj)
{
    int32 str_len, i;
    uint8 *string_bytes;

    str_len = wasm_stringref_obj_measure(stringref_obj, WTF8);
    string_bytes =
        (uint8 *)wasm_stringref_obj_encode_with_flag(stringref_obj, WTF8);

    if (str_len != 0) {
        for (i = 0; i < str_len; i++) {
            os_printf("%c", string_bytes[i]);
        }
    }

    if (string_bytes) {
        wasm_runtime_free(string_bytes);
    }
}

char *
wasm_stringview_wtf8_obj_convert_char(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj)
{
    char *str;
    int32 str_len, i;
    uint8 *string_bytes;

    str_len = wasm_stringview_wtf8_obj_get_length(stringview_wtf8_obj);
    string_bytes = wasm_stringview_wtf8_obj_get_bytes(stringview_wtf8_obj);
    str = wasm_runtime_malloc(sizeof(char) * (str_len + 1));
    if (str_len != 0) {
        for (i = 0; i < str_len; i++) {
            str[i] = string_bytes[i];
        }
    }
    str[str_len] = '\0';

    return str;
}

char *
wasm_stringview_wtf16_obj_convert_char(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj)
{
    char *str;
    int32 wtf16_obj_len, code_point_length, str_len, i;
    uint32 *code_points;
    uint16 *wtf16_obj_bytes;
    uint8 *string_bytes;

    wtf16_obj_len = wasm_stringview_wtf16_obj_get_length(stringview_wtf16_obj);
    wtf16_obj_bytes = wasm_stringview_wtf16_obj_get_bytes(stringview_wtf16_obj);
    code_points = encode_codepoints_by_16bit_bytes(
        wtf16_obj_bytes, wtf16_obj_len, &code_point_length);
    string_bytes = encode_8bit_bytes_by_codepoints(code_points,
                                                   code_point_length, &str_len);

    str = wasm_runtime_malloc(sizeof(char) * (str_len + 1));
    if (str_len != 0) {
        for (i = 0; i < str_len; i++) {
            str[i] = string_bytes[i];
        }
    }
    str[str_len] = '\0';

    if (code_points) {
        wasm_runtime_free(code_points);
    }
    if (string_bytes) {
        wasm_runtime_free(string_bytes);
    }

    return str;
}
