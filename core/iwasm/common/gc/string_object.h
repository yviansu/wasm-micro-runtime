/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "gc_object.h"

WASMString *
wasm_string_obj_new(void *pointer, uint32 length, bool is_const);

uint32
wasm_get_stringref_length(WASMStringrefObjectRef stringref_obj);

bool
wasm_get_stringref_value(WASMStringrefObjectRef stringref_obj, char *value);

uint32
wasm_string_eq(void *string_obj1, void *string_obj2);
