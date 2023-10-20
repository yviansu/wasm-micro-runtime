# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set (IWASM_GC_DIR ${CMAKE_CURRENT_LIST_DIR})

add_definitions (-DWASM_ENABLE_GC=1)

if (WAMR_TEST_GC EQUAL 1)
  add_definitions (-DGC_MANUALLY=1 -DGC_IN_EVERY_ALLOCATION=1)
endif ()

include_directories (${IWASM_GC_DIR})

file (GLOB_RECURSE source_all ${IWASM_GC_DIR}/*.c)

set (IWASM_GC_SOURCE ${source_all})

if (WAMR_BUILD_STRINGREF EQUAL 1)
  set (IWASM_STRINGREF_DIR ${CMAKE_CURRENT_LIST_DIR}/stringref)

  add_definitions (-DWASM_ENABLE_STRINGREF=1)

  include_directories (${IWASM_STRINGREF_DIR})

  file (GLOB_RECURSE source_all ${IWASM_STRINGREF_DIR}/*.c)

  if (NOT DEFINED WAMR_STRINGREF_IMPL_SOURCE)
      set (IWASM_STRINGREF_SOURCE ${source_all})
  else ()
      set (IWASM_STRINGREF_SOURCE ${WAMR_STRINGREF_IMPL_SOURCE})
  endif ()
endif ()
