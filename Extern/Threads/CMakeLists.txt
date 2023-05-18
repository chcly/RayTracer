# -----------------------------------------------------------------------------
#
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
cmake_minimum_required(VERSION 3.0)
project(Threads)

# Set the module path to include scripts
list(APPEND CMAKE_MODULE_PATH  ${Threads_SOURCE_DIR}/CMake)
include(Configure)


if (Threads_ExternalTarget)
    set(TargetFolders ${Threads_TargetFolders})
    set(TargetName    ${Threads_TargetName})
    set(TargetGroup   ${Threads_TargetGroup})
else()
    subdirs(CMake)
    subdirs(Extern)

    set(TargetFolders FALSE)
    set(TargetName Threads)

    set(Threads_INCLUDE ${Threads_SOURCE_DIR})
    set(Threads_LIBRARY ${TargetName})

endif()

subdirs(Threads)

if (Threads_BUILD_TEST)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    subdirs(Test/googletest)
    subdirs(Test)
endif()
