# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

string(TOUPPER ${WAMR_BUILD_TARGET} WAMR_BUILD_TARGET)

# Add definitions for the build target
if (WAMR_BUILD_TARGET STREQUAL "X86_64")
  add_definitions(-DBUILD_TARGET_X86_64)
elseif (WAMR_BUILD_TARGET STREQUAL "AMD_64")
  add_definitions(-DBUILD_TARGET_AMD_64)
elseif (WAMR_BUILD_TARGET STREQUAL "X86_32")
  add_definitions(-DBUILD_TARGET_X86_32)
elseif (WAMR_BUILD_TARGET MATCHES "ARM.*")
  if (WAMR_BUILD_TARGET MATCHES "(ARM.*)_VFP")
    add_definitions(-DBUILD_TARGET_ARM_VFP)
    add_definitions(-DBUILD_TARGET="${CMAKE_MATCH_1}")
  else ()
    add_definitions(-DBUILD_TARGET_ARM)
    add_definitions(-DBUILD_TARGET="${WAMR_BUILD_TARGET}")
  endif ()
elseif (WAMR_BUILD_TARGET MATCHES "THUMB.*")
  if (WAMR_BUILD_TARGET MATCHES "(THUMB.*)_VFP")
    add_definitions(-DBUILD_TARGET_THUMB_VFP)
    add_definitions(-DBUILD_TARGET="${CMAKE_MATCH_1}")
  else ()
    add_definitions(-DBUILD_TARGET_THUMB)
    add_definitions(-DBUILD_TARGET="${WAMR_BUILD_TARGET}")
  endif ()
elseif (WAMR_BUILD_TARGET MATCHES "AARCH64.*")
  add_definitions(-DBUILD_TARGET_AARCH64)
  add_definitions(-DBUILD_TARGET="${WAMR_BUILD_TARGET}")
elseif (WAMR_BUILD_TARGET STREQUAL "MIPS")
  add_definitions(-DBUILD_TARGET_MIPS)
elseif (WAMR_BUILD_TARGET STREQUAL "XTENSA")
  add_definitions(-DBUILD_TARGET_XTENSA)
elseif (WAMR_BUILD_TARGET STREQUAL "RISCV64" OR WAMR_BUILD_TARGET STREQUAL "RISCV64_LP64D")
  add_definitions(-DBUILD_TARGET_RISCV64_LP64D)
elseif (WAMR_BUILD_TARGET STREQUAL "RISCV64_LP64")
  add_definitions(-DBUILD_TARGET_RISCV64_LP64)
elseif (WAMR_BUILD_TARGET STREQUAL "RISCV32" OR WAMR_BUILD_TARGET STREQUAL "RISCV32_ILP32D")
  add_definitions(-DBUILD_TARGET_RISCV32_ILP32D)
elseif (WAMR_BUILD_TARGET STREQUAL "RISCV32_ILP32")
  add_definitions(-DBUILD_TARGET_RISCV32_ILP32)
elseif (WAMR_BUILD_TARGET STREQUAL "ARC")
  add_definitions(-DBUILD_TARGET_ARC)
else ()
  message (FATAL_ERROR "-- WAMR build target isn't set")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DBH_DEBUG=1)
endif ()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  if (WAMR_BUILD_TARGET STREQUAL "X86_64" OR WAMR_BUILD_TARGET STREQUAL "AMD_64"
      OR WAMR_BUILD_TARGET MATCHES "AARCH64.*" OR WAMR_BUILD_TARGET MATCHES "RISCV64.*")
    if (NOT WAMR_BUILD_PLATFORM STREQUAL "windows")
      # Add -fPIC flag if build as 64-bit
      set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
      set (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -fPIC")
      set (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -fPIC")
    endif ()
  else ()
    include(CheckCCompilerFlag)
    Check_C_Compiler_Flag(-m32 M32_OK)
    if (M32_OK OR WAMR_BUILD_TARGET STREQUAL "X86_32")
      add_definitions (-m32)
      set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
      set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
    endif ()
  endif ()
endif ()

if (WAMR_BUILD_TARGET MATCHES "ARM.*")
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -marm")
elseif (WAMR_BUILD_TARGET MATCHES "THUMB.*")
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb")
  set (CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wa,-mthumb")
endif ()

if (NOT WAMR_BUILD_INTERP EQUAL 1)
if (NOT WAMR_BUILD_AOT EQUAL 1)
  message (FATAL_ERROR "-- WAMR Interpreter and AOT must be enabled at least one")
endif ()
endif ()

if (WAMR_BUILD_FAST_JIT EQUAL 1)
  if (NOT WAMR_BUILD_LAZY_JIT EQUAL 0)
    # Enable Lazy JIT by default
    set (WAMR_BUILD_LAZY_JIT 1)
  endif ()
endif ()

if (WAMR_BUILD_JIT EQUAL 1)
  if (NOT WAMR_BUILD_LAZY_JIT EQUAL 0)
    # Enable Lazy JIT by default
    set (WAMR_BUILD_LAZY_JIT 1)
  endif ()
  if (NOT DEFINED LLVM_DIR)
    set (LLVM_SRC_ROOT "${WAMR_ROOT_DIR}/core/deps/llvm")
    set (LLVM_BUILD_ROOT "${LLVM_SRC_ROOT}/build")
    if (WAMR_BUILD_PLATFORM STREQUAL "windows")
      set (LLVM_BUILD_ROOT "${LLVM_SRC_ROOT}/win32build")
    endif ()
    if (NOT EXISTS "${LLVM_BUILD_ROOT}")
        message (FATAL_ERROR "Cannot find LLVM dir: ${LLVM_BUILD_ROOT}")
    endif ()
    set (CMAKE_PREFIX_PATH "${LLVM_BUILD_ROOT};${CMAKE_PREFIX_PATH}")
    set (LLVM_DIR ${LLVM_BUILD_ROOT}/lib/cmake/llvm)
  endif ()
  find_package(LLVM REQUIRED CONFIG)
  include_directories(${LLVM_INCLUDE_DIRS})
  add_definitions(${LLVM_DEFINITIONS})
  message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
  message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

  # Disable -Wredundant-move when building LLVM JIT
  include(CheckCXXCompilerFlag)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    check_cxx_compiler_flag("-Wredundant-move" CXX_SUPPORTS_REDUNDANT_MOVE_FLAG)
    if (CXX_SUPPORTS_REDUNDANT_MOVE_FLAG)
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-redundant-move")
    endif ()
  endif ()
else ()
  unset (LLVM_AVAILABLE_LIBS)
endif ()

# Sanitizers

set(WAMR_BUILD_SANITIZER $ENV{WAMR_BUILD_SANITIZER})

if (NOT DEFINED WAMR_BUILD_SANITIZER)
  set(WAMR_BUILD_SANITIZER "")
elseif (WAMR_BUILD_SANITIZER STREQUAL "ubsan")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fno-omit-frame-pointer -fsanitize=undefined -fno-sanitize-recover=all -fno-sanitize=alignment" )
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
elseif (WAMR_BUILD_SANITIZER STREQUAL "asan")
  if (NOT WAMR_BUILD_JIT EQUAL 1)
    set (ASAN_OPTIONS "verbosity=2 debug=true ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fno-omit-frame-pointer -fsanitize=address -fno-sanitize-recover=all" )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
  endif()
elseif (WAMR_BUILD_SANITIZER STREQUAL "tsan") 
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fno-omit-frame-pointer -fsanitize=thread -fno-sanitize-recover=all" )
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
elseif (NOT (WAMR_BUILD_SANITIZER STREQUAL "") )
  message(SEND_ERROR "Unsupported sanitizer: ${WAMR_BUILD_SANITIZER}")
endif()

########################################

message ("-- Build Configurations:")
message ("     Build as target ${WAMR_BUILD_TARGET}")
message ("     CMAKE_BUILD_TYPE " ${CMAKE_BUILD_TYPE})
if (WAMR_BUILD_INTERP EQUAL 1)
  message ("     WAMR Interpreter enabled")
else ()
  message ("     WAMR Interpreter disabled")
endif ()
if (WAMR_BUILD_AOT EQUAL 1)
  message ("     WAMR AOT enabled")
else ()
  message ("     WAMR AOT disabled")
endif ()
if (WAMR_BUILD_FAST_JIT EQUAL 1)
  if (WAMR_BUILD_LAZY_JIT EQUAL 1)
    add_definitions("-DWASM_ENABLE_LAZY_JIT=1")
    message ("     WAMR Fast JIT enabled with Lazy Compilation")
  else ()
    message ("     WAMR Fast JIT enabled with Eager Compilation")
  endif ()
else ()
  message ("     WAMR Fast JIT disabled")
endif ()
if (WAMR_BUILD_JIT EQUAL 1)
  add_definitions("-DWASM_ENABLE_JIT=1")
  if (WAMR_BUILD_LAZY_JIT EQUAL 1)
    add_definitions("-DWASM_ENABLE_LAZY_JIT=1")
    message ("     WAMR LLVM ORC JIT enabled with Lazy Compilation")
  else ()
    message ("     WAMR LLVM ORC JIT enabled with Eager Compilation")
  endif ()
else ()
  message ("     WAMR LLVM ORC JIT disabled")
endif ()
if (WAMR_BUILD_FAST_JIT EQUAL 1 AND WAMR_BUILD_JIT EQUAL 1
    AND WAMR_BUILD_LAZY_JIT EQUAL 1)
  message ("     Multi-tier JIT enabled")
endif ()
if (WAMR_BUILD_LIBC_BUILTIN EQUAL 1)
  message ("     Libc builtin enabled")
else ()
  message ("     Libc builtin disabled")
endif ()
if (WAMR_BUILD_LIBC_UVWASI EQUAL 1)
  message ("     Libc WASI enabled with uvwasi implementation")
elseif (WAMR_BUILD_LIBC_WASI EQUAL 1)
  message ("     Libc WASI enabled")
else ()
  message ("     Libc WASI disabled")
endif ()
if ((WAMR_BUILD_FAST_INTERP EQUAL 1) AND (WAMR_BUILD_INTERP EQUAL 1))
  add_definitions (-DWASM_ENABLE_FAST_INTERP=1)
  message ("     Fast interpreter enabled")
else ()
  add_definitions (-DWASM_ENABLE_FAST_INTERP=0)
  message ("     Fast interpreter disabled")
endif ()
if (WAMR_BUILD_MULTI_MODULE EQUAL 1)
  add_definitions (-DWASM_ENABLE_MULTI_MODULE=1)
  message ("     Multiple modules enabled")
else ()
  add_definitions (-DWASM_ENABLE_MULTI_MODULE=0)
  message ("     Multiple modules disabled")
endif ()
if (WAMR_BUILD_SPEC_TEST EQUAL 1)
  add_definitions (-DWASM_ENABLE_SPEC_TEST=1)
  message ("     spec test compatible mode is on")
endif ()
if (NOT DEFINED WAMR_BUILD_BULK_MEMORY)
  # Enable bulk memory by default
  set (WAMR_BUILD_BULK_MEMORY 1)
endif ()
if (WAMR_BUILD_BULK_MEMORY EQUAL 1)
  add_definitions (-DWASM_ENABLE_BULK_MEMORY=1)
  message ("     Bulk memory feature enabled")
else ()
  add_definitions (-DWASM_ENABLE_BULK_MEMORY=0)
  message ("     Bulk memory feature disabled")
endif ()
if (WAMR_BUILD_SHARED_MEMORY EQUAL 1)
  add_definitions (-DWASM_ENABLE_SHARED_MEMORY=1)
  message ("     Shared memory enabled")
else ()
  add_definitions (-DWASM_ENABLE_SHARED_MEMORY=0)
endif ()
if (WAMR_BUILD_THREAD_MGR EQUAL 1)
  message ("     Thread manager enabled")
endif ()
if (WAMR_BUILD_LIB_PTHREAD EQUAL 1)
  message ("     Lib pthread enabled")
endif ()
if (WAMR_BUILD_LIB_PTHREAD_SEMAPHORE EQUAL 1)
  message ("     Lib pthread semaphore enabled")
endif ()
if (WAMR_BUILD_LIB_WASI_THREADS EQUAL 1)
  message ("     Lib wasi-threads enabled")
endif ()
if (WAMR_BUILD_LIBC_EMCC EQUAL 1)
  message ("     Libc emcc enabled")
endif ()
if (WAMR_BUILD_LIB_RATS EQUAL 1)
  message ("     Lib rats enabled")
endif()
if (WAMR_BUILD_MINI_LOADER EQUAL 1)
  add_definitions (-DWASM_ENABLE_MINI_LOADER=1)
  message ("     WASM mini loader enabled")
else ()
  add_definitions (-DWASM_ENABLE_MINI_LOADER=0)
endif ()
if (WAMR_DISABLE_HW_BOUND_CHECK EQUAL 1)
  add_definitions (-DWASM_DISABLE_HW_BOUND_CHECK=1)
  add_definitions (-DWASM_DISABLE_STACK_HW_BOUND_CHECK=1)
  message ("     Hardware boundary check disabled")
else ()
  add_definitions (-DWASM_DISABLE_HW_BOUND_CHECK=0)
  if (WAMR_DISABLE_STACK_HW_BOUND_CHECK EQUAL 1)
    add_definitions (-DWASM_DISABLE_STACK_HW_BOUND_CHECK=1)
    message ("     Hardware boundary check for native stack disabled")
  else ()
    add_definitions (-DWASM_DISABLE_STACK_HW_BOUND_CHECK=0)
  endif ()
endif ()
if (WAMR_DISABLE_WAKEUP_BLOCKING_OP EQUAL 1)
  add_definitions (-DWASM_DISABLE_WAKEUP_BLOCKING_OP=1)
  message ("     Wakeup of blocking operations disabled")
else ()
  add_definitions (-DWASM_DISABLE_WAKEUP_BLOCKING_OP=0)
  message ("     Wakeup of blocking operations enabled")
endif ()
if (WAMR_BUILD_SIMD EQUAL 1)
  if (NOT WAMR_BUILD_TARGET MATCHES "RISCV64.*")
    add_definitions (-DWASM_ENABLE_SIMD=1)
    message ("     SIMD enabled")
  else ()
    message ("     SIMD disabled due to not supported on target RISCV64")
  endif ()
endif ()
if (WAMR_BUILD_MEMORY_PROFILING EQUAL 1)
  add_definitions (-DWASM_ENABLE_MEMORY_PROFILING=1)
  message ("     Memory profiling enabled")
endif ()
if (WAMR_BUILD_PERF_PROFILING EQUAL 1)
  add_definitions (-DWASM_ENABLE_PERF_PROFILING=1)
  message ("     Performance profiling enabled")
endif ()
if (DEFINED WAMR_APP_THREAD_STACK_SIZE_MAX)
  add_definitions (-DAPP_THREAD_STACK_SIZE_MAX=${WAMR_APP_THREAD_STACK_SIZE_MAX})
endif ()
if (WAMR_BUILD_CUSTOM_NAME_SECTION EQUAL 1)
  add_definitions (-DWASM_ENABLE_CUSTOM_NAME_SECTION=1)
  message ("     Custom name section enabled")
endif ()
if (WAMR_BUILD_DUMP_CALL_STACK EQUAL 1)
  add_definitions (-DWASM_ENABLE_DUMP_CALL_STACK=1)
  message ("     Dump call stack enabled")
endif ()
if (WAMR_BUILD_TAIL_CALL EQUAL 1)
  add_definitions (-DWASM_ENABLE_TAIL_CALL=1)
  message ("     Tail call enabled")
endif ()
if (WAMR_BUILD_REF_TYPES EQUAL 1)
  add_definitions (-DWASM_ENABLE_REF_TYPES=1)
  message ("     Reference types enabled")
else ()
  message ("     Reference types disabled")
endif ()
if (WAMR_BUILD_GC EQUAL 1)
  message ("     GC enabled")
  if (WAMR_TEST_GC EQUAL 1)
    message("      GC testing enabled")
  endif()
endif ()
if (WAMR_BUILD_GC_BINARYEN EQUAL 1)
  add_definitions (-DWASM_ENABLE_GC_BINARYEN=1)
  message ("     GC binaryen compatible mode on")
endif ()
if (WAMR_BUILD_STRINGREF EQUAL 1)
  add_definitions (-DWASM_ENABLE_STRINGREF=1)
  message ("     Stringref enabled")
  if (NOT DEFINED WAMR_STRINGREF_IMPL_SOURCE)
    set(STRING_IMPL_DIR "${WAMR_ROOT_DIR}/core/iwasm/common/gc")
    message ("     Stringref implement source is WAMR")
  else ()
    set(STRING_IMPL_DIR "${WAMR_STRINGREF_IMPL_SOURCE}")
    message ("     Stringref implement source is customer defined")
  include_directories(${STRING_IMPL_DIR})
  endif ()
endif ()
if (DEFINED WAMR_BH_VPRINTF)
  add_definitions (-DBH_VPRINTF=${WAMR_BH_VPRINTF})
endif ()
if (WAMR_DISABLE_APP_ENTRY EQUAL 1)
  message ("     WAMR application entry functions excluded")
endif ()
if (WAMR_BUILD_DEBUG_INTERP EQUAL 1)
    message ("     Debug Interpreter enabled")
endif ()
if (WAMR_BUILD_DEBUG_AOT EQUAL 1)
    message ("     Debug AOT enabled")
endif ()
if (WAMR_BUILD_LOAD_CUSTOM_SECTION EQUAL 1)
    add_definitions (-DWASM_ENABLE_LOAD_CUSTOM_SECTION=1)
    message ("     Load custom section enabled")
endif ()
if (WAMR_BUILD_GLOBAL_HEAP_POOL EQUAL 1)
  add_definitions(-DWASM_ENABLE_GLOBAL_HEAP_POOL=1)
  message ("     Global heap pool enabled")
endif ()
if (WAMR_BUILD_GLOBAL_HEAP_SIZE GREATER 0)
  add_definitions (-DWASM_GLOBAL_HEAP_SIZE=${WAMR_BUILD_GLOBAL_HEAP_SIZE})
  message ("     Custom global heap size: " ${WAMR_BUILD_GLOBAL_HEAP_SIZE})
else ()
  # Spec test requires more heap pool size
  if (WAMR_BUILD_SPEC_TEST EQUAL 1)
    if (WAMR_BUILD_PLATFORM STREQUAL "linux-sgx")
      math(EXPR WAMR_BUILD_GLOBAL_HEAP_SIZE "100 * 1024 * 1024")
    else ()
      math(EXPR WAMR_BUILD_GLOBAL_HEAP_SIZE "300 * 1024 * 1024")
    endif ()
    add_definitions (-DWASM_GLOBAL_HEAP_SIZE=${WAMR_BUILD_GLOBAL_HEAP_SIZE})
  else ()
    # By default, the global heap size is of 10 MB
    math(EXPR WAMR_BUILD_GLOBAL_HEAP_SIZE "10 * 1024 * 1024")
    add_definitions (-DWASM_GLOBAL_HEAP_SIZE=${WAMR_BUILD_GLOBAL_HEAP_SIZE})
  endif ()
endif ()
if (WAMR_BUILD_STACK_GUARD_SIZE GREATER 0)
    add_definitions (-DWASM_STACK_GUARD_SIZE=${WAMR_BUILD_STACK_GUARD_SIZE})
    message ("     Custom stack guard size: " ${WAMR_BUILD_STACK_GUARD_SIZE})
endif ()
if (WAMR_BUILD_SGX_IPFS EQUAL 1)
    add_definitions (-DWASM_ENABLE_SGX_IPFS=1)
    message ("     SGX IPFS enabled")
endif ()
if (WAMR_BUILD_WASI_NN EQUAL 1)
  message ("     WASI-NN enabled")
  add_definitions (-DWASM_ENABLE_WASI_NN=1)
  if (WAMR_BUILD_WASI_NN_ENABLE_GPU EQUAL 1)
      message ("     WASI-NN: GPU enabled")
      add_definitions (-DWASM_ENABLE_WASI_NN_GPU=1)
  endif ()
  if (WAMR_BUILD_WASI_NN_ENABLE_EXTERNAL_DELEGATE EQUAL 1)
      message ("     WASI-NN: External Delegation enabled")
      add_definitions (-DWASM_ENABLE_WASI_NN_EXTERNAL_DELEGATE=1)
  endif ()
  if (DEFINED WAMR_BUILD_WASI_NN_EXTERNAL_DELEGATE_PATH)
      add_definitions (-DWASM_WASI_NN_EXTERNAL_DELEGATE_PATH="${WAMR_BUILD_WASI_NN_EXTERNAL_DELEGATE_PATH}")
  endif ()
endif ()
if (WAMR_BUILD_ALLOC_WITH_USER_DATA EQUAL 1)
  add_definitions(-DWASM_MEM_ALLOC_WITH_USER_DATA=1)
endif()
if (WAMR_BUILD_WASM_CACHE EQUAL 1)
  add_definitions (-DWASM_ENABLE_WASM_CACHE=1)
  message ("     Wasm files cache enabled")
endif ()
if (WAMR_BUILD_MODULE_INST_CONTEXT EQUAL 1)
  add_definitions (-DWASM_ENABLE_MODULE_INST_CONTEXT=1)
  message ("     Module instance context enabled")
endif ()
if (WAMR_BUILD_GC_HEAP_VERIFY EQUAL 1)
  add_definitions (-DWASM_ENABLE_GC_VERIFY=1)
  message ("     GC heap verification enabled")
endif ()
if ("$ENV{COLLECT_CODE_COVERAGE}" STREQUAL "1" OR COLLECT_CODE_COVERAGE EQUAL 1)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  add_definitions (-DCOLLECT_CODE_COVERAGE)
  message ("     Collect code coverage enabled")
endif ()
if (WAMR_BUILD_STATIC_PGO EQUAL 1)
  add_definitions (-DWASM_ENABLE_STATIC_PGO=1)
  message ("     AOT static PGO enabled")
endif ()
if (WAMR_DISABLE_WRITE_GS_BASE EQUAL 1)
  add_definitions (-DWASM_DISABLE_WRITE_GS_BASE=1)
  message ("     Write linear memory base addr to x86 GS register disabled")
elseif (WAMR_BUILD_TARGET STREQUAL "X86_64"
        AND WAMR_BUILD_PLATFORM STREQUAL "linux")
  set (TEST_WRGSBASE_SOURCE "${CMAKE_BINARY_DIR}/test_wrgsbase.c")
  file (WRITE "${TEST_WRGSBASE_SOURCE}" "
  #include <stdio.h>
  #include <stdint.h>
  int main() {
      uint64_t value;
      asm volatile (\"wrgsbase %0\" : : \"r\"(value));
      printf(\"WRGSBASE instruction is available.\\n\");
      return 0;
  }")
  # Try to compile and run the test program
  try_run (TEST_WRGSBASE_RESULT
    TEST_WRGSBASE_COMPILED
    ${CMAKE_BINARY_DIR}/test_wrgsbase
    SOURCES ${TEST_WRGSBASE_SOURCE}
    CMAKE_FLAGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
  )
  #message("${TEST_WRGSBASE_COMPILED}, ${TEST_WRGSBASE_RESULT}")
  if (NOT TEST_WRGSBASE_RESULT EQUAL 0)
    add_definitions (-DWASM_DISABLE_WRITE_GS_BASE=1)
    message ("     Write linear memory base addr to x86 GS register disabled")
  endif ()
endif ()
if (WAMR_CONFIGUABLE_BOUNDS_CHECKS EQUAL 1)
  add_definitions (-DWASM_CONFIGURABLE_BOUNDS_CHECKS=1)
  message ("     Configurable bounds checks enabled")
endif ()
