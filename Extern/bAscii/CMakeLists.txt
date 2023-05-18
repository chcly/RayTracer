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
project(bAscii)

# Set the module path to include scripts
list(APPEND CMAKE_MODULE_PATH  ${bAscii_SOURCE_DIR}/CMake ${bAscii_SOURCE_DIR}/CMake/Modules)
include(Configure)

if (bAscii_ExternalTarget)

    set(TargetFolders ${bAscii_TargetFolders})
    set(TargetName    ${bAscii_TargetName})
    set(TargetGroup   ${bAscii_TargetGroup})
else()
    add_subdirectory(CMake)
    add_subdirectory(Extern)
    set(TargetFolders TRUE)
    set(TargetName bAscii)
    set(TargetGroup )

    set(bAscii_INCLUDE  ${bAscii_SOURCE_DIR})
    set(bAscii_LIBRARY  ${TargetName})
endif()

add_subdirectory(bAscii)

if (bAscii_BUILD_TEST)
    set(TargetGroup Units)
    if (NOT bAscii_ExternalTarget)
        add_subdirectory(${GTEST_DIR})
        if (TargetFolders)
            set_target_properties(gtest_main PROPERTIES FOLDER "${TargetGroup}")
            set_target_properties(gtest      PROPERTIES FOLDER "${TargetGroup}")
        endif()
    endif()
    add_subdirectory(Test)
endif()
