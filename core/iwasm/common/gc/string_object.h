/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "gc_object.h"

struct WASMExecEnv;

void
wasm_stringref_obj_finalizer(WASMStringrefObjectRef stringref_obj, void *data);

void
wasm_stringview_wtf8_obj_finalizer(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj, void *data);

void
wasm_stringview_wtf16_obj_finalizer(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj, void *data);

void
wasm_stringview_iter_obj_finalizer(
    WASMStringviewIterObjectRef stringview_iter_obj, void *data);

WASMStringrefObjectRef
wasm_stringref_obj_new_with_8bit_embedder(struct WASMExecEnv *exec_env,
                                          uint8 *target_bytes, uint32 length,
                                          bool is_const);

WASMStringrefObjectRef
wasm_stringref_obj_new_with_16bit_embedder(struct WASMExecEnv *exec_env,
                                           uint16 *target_bytes, uint32 length);

WASMStringviewWTF8ObjectRef
wasm_stringview_wtf8_obj_new_by_stringref(struct WASMExecEnv *exec_env,
                                          WASMStringrefObjectRef stringref_obj);

int32
wasm_stringview_wtf8_obj_get_length(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj);

uint8 *
wasm_stringview_wtf8_obj_get_bytes(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj);

WASMStringviewWTF16ObjectRef
wasm_stringview_wtf16_obj_new_by_stringref(
    struct WASMExecEnv *exec_env, WASMStringrefObjectRef stringref_obj);

int32
wasm_stringview_wtf16_obj_get_length(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj);

uint16 *
wasm_stringview_wtf16_obj_get_bytes(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj);

int16
wasm_stringview_wtf16_obj_get_codeunit_at_pos(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj, int32 pos);

WASMStringviewIterObjectRef
wasm_stringview_iter_obj_new_by_stringref(struct WASMExecEnv *exec_env,
                                          WASMStringrefObjectRef stringref_obj);

int32
wasm_stringref_obj_eq(WASMStringrefObjectRef stringref_obj1,
                      WASMStringrefObjectRef stringref_obj2);

int32
wasm_stringref_obj_measure_from_start(WASMStringrefObjectRef stringref_obj,
                                      encoding_flag flag, uint32 start);

int32
wasm_stringref_obj_measure(WASMStringrefObjectRef stringref_obj,
                           encoding_flag flag);

void *
wasm_stringref_obj_encode_with_flag(WASMStringrefObjectRef stringref_obj,
                                    encoding_flag flag);

WASMStringrefObjectRef
wasm_stringref_obj_concat(struct WASMExecEnv *exec_env,
                          WASMStringrefObjectRef stringref_obj1,
                          WASMStringrefObjectRef stringref_obj2);

int32
wasm_stringref_obj_is_usv_sequence(WASMStringrefObjectRef stringref_obj);

uint32
wasm_stringview_iter_obj_get_next_codepoint(
    WASMStringviewIterObjectRef stringview_iter_obj);

uint32
wasm_stringview_iter_obj_advance(
    WASMStringviewIterObjectRef stringview_iter_obj, uint32 code_points_count);

uint32
wasm_stringview_iter_obj_rewind(WASMStringviewIterObjectRef stringview_iter_obj,
                                uint32 code_points_count);

WASMStringrefObjectRef
wasm_stringview_iter_obj_slice(struct WASMExecEnv *exec_env,
                               WASMStringviewIterObjectRef stringview_iter_obj,
                               uint32 code_points_count);

char *
wasm_stringref_obj_convert_char(WASMStringrefObjectRef stringref_obj);

char *
wasm_stringview_wtf8_obj_convert_char(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj);

char *
wasm_stringview_wtf16_obj_convert_char(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj);
