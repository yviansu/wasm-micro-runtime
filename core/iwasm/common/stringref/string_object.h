/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#ifndef _STRING_OBJECT_H_
#define _STRING_OBJECT_H_

#include "gc_object.h"
#include "wasm_runtime_common.h"

struct WASMExecEnv;

/******************* gc finalizer *****************/

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

/******************* opcode functions *****************/

/* string.const */
WASMStringrefObjectRef
wasm_stringref_obj_new_with_const(struct WASMExecEnv *exec_env,
                                  WASMStringWTF8 *str_obj);

/* string.new_xx8 */
WASMStringrefObjectRef
wasm_stringref_obj_new_with_8bit_memory(struct WASMExecEnv *exec_env,
                                        void *maddr, uint32 bytes_length,
                                        encoding_flag flag);

/* string.new_wtf16 */
WASMStringrefObjectRef
wasm_stringref_obj_new_with_16bit_memory(struct WASMExecEnv *exec_env,
                                         void *maddr, uint32 bytes_length);

/* string.measure */
int32
wasm_stringref_obj_measure(WASMStringrefObjectRef stringref_obj,
                           encoding_flag flag);

/* string.encode_xx8 */
int32
wasm_stringref_obj_encode_with_8bit_memory(struct WASMExecEnv *exec_env,
                                           void *maddr,
                                           WASMStringrefObjectRef stringref_obj,
                                           encoding_flag flag);

/* string.encode_wtf16 */
int32
wasm_stringref_obj_encode_with_16bit_memory(
    struct WASMExecEnv *exec_env, void *maddr,
    WASMStringrefObjectRef stringref_obj, encoding_flag flag);

/* string.concat */
WASMStringrefObjectRef
wasm_stringref_obj_concat(struct WASMExecEnv *exec_env,
                          WASMStringrefObjectRef stringref_obj1,
                          WASMStringrefObjectRef stringref_obj2);

/* string.eq */
int32
wasm_stringref_obj_eq(WASMStringrefObjectRef stringref_obj1,
                      WASMStringrefObjectRef stringref_obj2);

/* string.is_usv_sequence */
int32
wasm_stringref_obj_is_usv_sequence(WASMStringrefObjectRef stringref_obj);

/* string.as_wtf8 */
WASMStringviewWTF8ObjectRef
wasm_stringview_wtf8_obj_new_by_stringref(struct WASMExecEnv *exec_env,
                                          WASMStringrefObjectRef stringref_obj);

/* stringview_wtf8.advance */
int32
wasm_stringview_wtf8_obj_advance(
    struct WASMExecEnv *exec_env,
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj, uint32 pos,
    uint32 bytes_length);

/* stringview_wtf8.encode_xx */
int32
wasm_stringview_wtf8_obj_encode_memory(
    struct WASMExecEnv *exec_env, void *maddr,
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj, uint32 pos,
    uint32 bytes_length, uint32 *next_pos, encoding_flag flag);

/* stringview_wtf8.slice */
WASMStringrefObjectRef
wasm_stringview_wtf8_obj_slice(struct WASMExecEnv *exec_env,
                               WASMStringviewWTF8ObjectRef stringview_wtf8_obj,
                               uint32 start, uint32 end);
/* string.as_wtf16 */
WASMStringviewWTF16ObjectRef
wasm_stringview_wtf16_obj_new_by_stringref(
    struct WASMExecEnv *exec_env, WASMStringrefObjectRef stringref_obj);

/* stringview_wtf16.length */
int32
wasm_stringview_wtf16_obj_get_length(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj);

/* stringview_wtf16.get_codeunit */
int16
wasm_stringview_wtf16_obj_get_codeunit_at_pos(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj, int32 pos);

/* stringview_wtf16.encode */
int32
wasm_stringview_wtf16_obj_encode_memory(
    struct WASMExecEnv *exec_env, void *maddr,
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj, uint32 pos, uint32 len);

/* stringview_wtf16.slice */
WASMStringrefObjectRef
wasm_stringview_wtf16_obj_slice(
    struct WASMExecEnv *exec_env,
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj, uint32 start,
    uint32 end);

/* string.as_iter */
WASMStringviewIterObjectRef
wasm_stringview_iter_obj_new_by_stringref(struct WASMExecEnv *exec_env,
                                          WASMStringrefObjectRef stringref_obj);

/* stringview_iter.next */
uint32
wasm_stringview_iter_obj_get_next_codepoint(
    WASMStringviewIterObjectRef stringview_iter_obj);

/* stringview_iter.advance */
uint32
wasm_stringview_iter_obj_advance(
    WASMStringviewIterObjectRef stringview_iter_obj, uint32 code_points_count);

/* stringview_iter.rewind */
uint32
wasm_stringview_iter_obj_rewind(WASMStringviewIterObjectRef stringview_iter_obj,
                                uint32 code_points_count);

/* stringview_iter.slice */
WASMStringrefObjectRef
wasm_stringview_iter_obj_slice(struct WASMExecEnv *exec_env,
                               WASMStringviewIterObjectRef stringview_iter_obj,
                               uint32 code_points_count);

/* string.new_xx8_array */
WASMStringrefObjectRef
wasm_stringref_obj_new_with_8bit_array(struct WASMExecEnv *exec_env,
                                       WASMArrayObjectRef array_obj,
                                       uint32 start, uint32 end,
                                       encoding_flag flag);

/* string.new_wtf16_array */
WASMStringrefObjectRef
wasm_stringref_obj_new_with_16bit_array(struct WASMExecEnv *exec_env,
                                        WASMArrayObjectRef array_obj,
                                        uint32 start, uint32 end);

/* string.encode_xx8_array */
uint32
wasm_stringref_obj_encode_with_8bit_array(struct WASMExecEnv *exec_env,
                                          WASMStringrefObjectRef stringref_obj,
                                          WASMArrayObjectRef array_obj,
                                          uint32 start, encoding_flag flag);

/* string.encode_wtf16_array */
uint32
wasm_stringref_obj_encode_with_16bit_array(struct WASMExecEnv *exec_env,
                                           WASMStringrefObjectRef stringref_obj,
                                           WASMArrayObjectRef array_obj,
                                           uint32 start, encoding_flag flag);

/******************* application functions *****************/

void
wasm_stringref_obj_dump(WASMStringrefObjectRef stringref_obj);

char *
wasm_stringview_wtf8_obj_convert_char(
    WASMStringviewWTF8ObjectRef stringview_wtf8_obj);

char *
wasm_stringview_wtf16_obj_convert_char(
    WASMStringviewWTF16ObjectRef stringview_wtf16_obj);

#endif /* end of _STRING_OBJECT_H_ */
