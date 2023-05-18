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


if (BlendFile_ExternalTarget)
    set(TargetFolders ${BlendFile_TargetFolders})
    set(TargetName    ${BlendFile_TargetName})
    set(TargetGroup   ${BlendFile_TargetGroup})
else()
    set(TargetName BlendFile)
    unset(TargetGroup )
endif()

set(BlendFile_INCLUDE 
    ${CMAKE_CURRENT_SOURCE_DIR}  
    ${FileTools_INCLUDE} 
    ${Utils_INCLUDE})

set(BlendFile_LIBRARY 
    ${TargetName} 
    ${FileTools_LIBRARY} 
    ${Utils_LIBRARY})


list(APPEND CMAKE_MODULE_PATH  ${FileTools_SOURCE_DIR}/Tools/Compiler/CMake)

include(TableCompiler)
add_table(bfBlender Blender/292.h)



set(BlendFile_SRC
    ftBlend.h
    ftBlend.cpp
    Blender.h
    ${TrashedFiles}
)

include_directories(${BlendFile_INCLUDE})
add_library(
    ${TargetName}
    ${BlendFile_SRC} 
    ${bfBlender}
)

target_link_libraries(${TargetName} ${BlendFile_TOOLS_LIBRARY})

if (TargetFolders)
    set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
endif()
