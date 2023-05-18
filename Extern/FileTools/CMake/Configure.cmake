# -----------------------------------------------------------------------------
#   Copyright (c) Charles Carley.
#
#   This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
#   Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
# ------------------------------------------------------------------------------
include(StaticRuntime)
include(ExternalTarget)

set_static_runtime()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(FileTools_BUILD_TEST           "Build the unit test program." OFF)
option(FileTools_AUTO_RUN_TEST        "Automatically run the test program." OFF)
option(FileTools_BUILD_COMPILER       "Build the table compiler." ON)
option(FileTools_BUILD_DECOMPILER     "Build the table decompiler." ON)
option(FileTools_BUILD_RECOMPILE_TEST "Build the Decompile/Recompile test." OFF)
option(FileTools_BLEND_FILE           "Build the Blend file loader." OFF)
option(FileTools_BLEND_TEST           "Builds a test .blend program." OFF)
option(FileTools_BUILD_DOCS           "Builds documentation." OFF)
option(FileTools_USE_ZLIB             "Build with zlib." OFF)


set(BUILD_GMOCK   OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(GTEST_DIR     ${FileTools_SOURCE_DIR}/Test/googletest)
set(GTEST_INCLUDE ${FileTools_SOURCE_DIR}/Test/googletest/googletest/include)
set(GTEST_LIBRARY gtest_main)


set(FileTools_ZLIB_INCLUDE  ${FileTools_SOURCE_DIR}/File/zlib)
set(FileTools_ZLIB_LIBRARY  zlib)
set(FileTools_ZLIB_DEFINITIONS "-DFT_USE_ZLIB=1")



set(Extern ${FileTools_SOURCE_DIR}/External)

DefineExternalTarget(Data2Array    Extern "${Extern}/Data2Array")
DefineExternalTarget(Utils         Extern "${Extern}")
