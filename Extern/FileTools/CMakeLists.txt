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

cmake_minimum_required(VERSION 3.0)
project(FileTools)

# Set the module path to include scripts
list(APPEND CMAKE_MODULE_PATH  
    ${FileTools_SOURCE_DIR}/CMake 
    ${FileTools_SOURCE_DIR}/CMake/Modules)
include(Configure)

if (FileTools_ExternalTarget)

    set(TargetFolders ${FileTools_TargetFolders})
    set(TargetName    ${FileTools_TargetName})
    set(TargetGroup   ${FileTools_TargetGroup})

else()
    subdirs(CMake)
    subdirs(External)
    set(TargetFolders TRUE)
    set(TargetName FileTools)
    set(TargetGroup )

    set(FileTools_INCLUDE  ${FileTools_SOURCE_DIR}/File)
    set(FileTools_LIBRARY  ${TargetName})
    set(BlendFile_INCLUDE ${FileTools_SOURCE_DIR}/FileFormats/Blend)
    set(BlendFile_LIBRARY BlendFile)

    if (FileTools_BUILD_DOCS)
        add_subdirectory(Design)
    endif()

    if(FileTools_USE_ZLIB)
        add_subdirectory(File/zlib)
    endif()


endif()

add_subdirectory(File)

if (FileTools_BLEND_FILE)
    unset(TargetGroup )
    add_subdirectory(FileFormats/Blend)
    add_subdirectory(Tools/SwapChunkCodes)
endif()


if (FileTools_BUILD_COMPILER)
    add_subdirectory(Tools/Compiler)
endif()

if (FileTools_BUILD_DECOMPILER)
    add_subdirectory(Tools/Decompiler)
endif()

if (FileTools_BUILD_TEST)
    set(BUILD_GMOCK   OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(TargetGroup "Units")
    add_subdirectory(${FileTools_SOURCE_DIR}/Test/googletest)
    
    set_target_properties(gtest_main PROPERTIES FOLDER "Units")
    set_target_properties(gtest      PROPERTIES FOLDER "Units")

    add_subdirectory(Test)

    if (FileTools_BUILD_COMPILER AND FileTools_BUILD_DECOMPILER AND FileTools_BUILD_RECOMPILE_TEST)
        add_subdirectory(Tools/CompileTest)
    endif()

    if (FileTools_BLEND_TEST)
        add_subdirectory(FileFormats/Blend/Test)
    endif()

    if (NOT FileTools_ExternalTarget)
        if(FileTools_AUTO_RUN_TEST)
            set(Utils_AUTO_RUN_TESTS TRUE)
        endif()
        add_subdirectory(External/Utils/Tests)
    endif()

endif()
