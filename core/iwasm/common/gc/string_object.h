/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "gc_object.h"

struct WASMExecEnv;

WASMStringWTF8 *
wasm_string_obj_new(uint8 *target_bytes, uint32 length);

WASMStringrefObjectRef
wasm_stringref_obj_new_with_embedder(struct WASMExecEnv *exec_env,
                                     uint8 *target_bytes, uint32 length);

int32
wasm_get_stringref_length(WASMStringrefObjectRef stringref_obj);

uint8 *
wasm_get_stringref_bytes(WASMStringrefObjectRef stringref_obj);

int32
wasm_stringref_eq(WASMStringrefObjectRef stringref_obj1,
                  WASMStringrefObjectRef stringref_obj2);
