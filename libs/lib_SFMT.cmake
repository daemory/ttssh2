﻿
include(${CMAKE_CURRENT_LIST_DIR}/script_support.cmake)

set(SFMT_ROOT ${CMAKE_CURRENT_LIST_DIR}/SFMT_${TOOLSET})
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(SFMT_ROOT "${SFMT_ROOT}_x64")
endif()

set(SFMT_INCLUDE_DIRS "${SFMT_ROOT}/include")
set(SFMT_LIBRARY_DIRS "${SFMT_ROOT}/lib")

if(MINGW)
  set(SFMT_LIB ${SFMT_LIBRARY_DIRS}/libsfmt.a)
else()
  set(SFMT_LIB ${SFMT_LIBRARY_DIRS}/sfmt.lib)
#  set(SFMT_LIB ${SFMT_LIBRARY_DIRS}/sfmtd.lib)
endif()
