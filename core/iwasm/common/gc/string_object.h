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

uint32
wasm_get_stringref_length(WASMStringrefObjectRef stringref_obj);

uint8 *
wasm_get_stringref_bytes(WASMStringrefObjectRef stringref_obj);

uint32
wasm_string_eq(void *string_obj1, void *string_obj2);
