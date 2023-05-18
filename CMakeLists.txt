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
cmake_minimum_required(VERSION 3.23)
project(RayTracer)

list(APPEND CMAKE_MODULE_PATH  
    ${RayTracer_SOURCE_DIR}/CMake 
    ${RayTracer_SOURCE_DIR}/Extern/FileTools/Tools/Compiler/CMake
    ${RayTracer_SOURCE_DIR}/CMake/Modules)
include(Configure)

if (RayTracer_ExternalTarget)

    set(TargetFolders ${RayTracer_TargetFolders})
    set(TargetName    ${RayTracer_TargetName})
    set(TargetGroup   ${RayTracer_TargetGroup})
else()
    add_subdirectory(CMake)

    set(TargetFolders TRUE)
    set(TargetName RayTracer)
    set(TargetGroup )

    set(RayTracer_INCLUDE  
        ${RayTracer_SOURCE_DIR}
    )
    set(RayTracer_LIBRARY  ${TargetName})

    add_subdirectory(Extern)
    add_subdirectory(Samples)
endif()

add_subdirectory(RenderSystem)
