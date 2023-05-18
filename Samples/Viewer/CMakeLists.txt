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
set(TargetName Viewer)
set(TargetGroup )

include (GroupSet)

set_group(SampleFiles
    Test:*.bascii
    Test:*.blend
)

set(TargetName_SOURCE 
    Main.cpp
    ${SampleFiles}
)

include_directories(
    ${RayTracer_INCLUDE}
    ${Utils_INCLUDE}
    ${Math_INCLUDE}
    ${Image_INCLUDE}
    ${FileTools_INCLUDE}
    ${BlendFile_INCLUDE}
    ${SDL_INCLUDE}
    ${Cuda_INCLUDE}
)
add_executable(
    ${TargetName} 
    ${TargetName_SOURCE}
)


if (USING_CUDA)
    add_definitions(-DUSING_CUDA)
endif()

if (RayTracer_OPT_GEN_INTRINSIC)
    add_definitions(-DRT_USE_SIMD)
endif()


target_link_libraries(
    ${TargetName} 
    ${RayTracer_LIBRARY}
)

if (TargetFolders)
    set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
endif()

if (MSVC)
    # Relative to the root directory where make is called
    set_target_properties(
        ${TargetName} 
        PROPERTIES 
        VS_DEBUGGER_WORKING_DIRECTORY 
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
else()
    add_custom_command(TARGET ${TargetName}
          POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SampleFiles} ${CMAKE_CURRENT_BINARY_DIR})
endif()
