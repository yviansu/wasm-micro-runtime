/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "string_object.h"

WASMStringWTF8 *
wasm_string_obj_new(uint8 *target_bytes, uint32 length)
{
    WASMStringWTF8 *string_obj;
    uint64 string_size;
    uint32 i, wtf8_length = 0;

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

// void
// wasm_string_obj_new_by_pos(WASMStringWTF8 **string_obj, uint8 *string_bytes,
//                            uint32 start_pos, uint32 end_pos)
// {
//     if (start_pos == end_pos) {
//         *string_obj = wasm_string_obj_new(NULL, 0, false);
//     }
//     else {
//         *string_obj = wasm_string_obj_new(string_bytes + start_pos,
//                                           end_pos - start_pos, false);
//     }
// }

uint32
wasm_get_stringref_length(WASMStringrefObjectRef stringref_obj)
{
    WASMStringWTF8 *string_obj;
    string_obj = stringref_obj->str_obj;
    return string_obj->length;
}

uint8 *
wasm_get_stringref_bytes(WASMStringrefObjectRef stringref_obj)
{
    WASMStringWTF8 *string_obj;
    string_obj = stringref_obj->str_obj;
    return string_obj->string_bytes;
}

uint32
wasm_string_eq(void *string_obj1, void *string_obj2)
{
    uint32 string_length1, string_length2, i;
    uint8 *string_bytes1, *string_bytes2;

    if (string_obj1 == string_obj2) {
        return 1;
    }

    if (string_obj1 == NULL || string_obj2 == NULL) {
        return 0;
    }

    string_length1 = ((WASMStringWTF8 *)string_obj1)->length;
    string_length2 = ((WASMStringWTF8 *)string_obj2)->length;
    string_bytes1 = ((WASMStringWTF8 *)string_obj1)->string_bytes;
    string_bytes2 = ((WASMStringWTF8 *)string_obj2)->string_bytes;

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
