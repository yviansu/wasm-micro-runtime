/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "gc_object.h"

struct WASMExecEnv;

WASMStringWTF8 *
wasm_stringwtf8_obj_new(uint8 *target_bytes, uint32 length);

WASMStringWTF16 *
wasm_stringwtf16_obj_new(uint16 *target_bytes, uint32 length);

WASMStringrefObjectRef
wasm_stringref_obj_new_with_8bit_embedder(struct WASMExecEnv *exec_env,
                                          uint8 *target_bytes, uint32 length);

WASMStringrefObjectRef
wasm_stringref_obj_new_with_16bit_embedder(struct WASMExecEnv *exec_env,
                                           uint16 *target_bytes, uint32 length);

int32
wasm_stringref_obj_get_length(WASMStringrefObjectRef stringref_obj);

uint8 *
wasm_stringref_obj_get_bytes(WASMStringrefObjectRef stringref_obj);

int32
wasm_stringview_wtf8_obj_get_length(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj);

uint8 *
wasm_stringview_wtf8_obj_get_bytes(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj);

int32
wasm_stringview_iter_obj_get_length(
    WASMStringviewIterObjectRef stringview_iter_obj);

uint8 *
wasm_stringview_iter_obj_get_bytes(
    WASMStringviewIterObjectRef stringview_iter_obj);

int32
wasm_stringref_obj_eq(WASMStringrefObjectRef stringref_obj1,
                      WASMStringrefObjectRef stringref_obj2);
