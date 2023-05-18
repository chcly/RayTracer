# -----------------------------------------------------------------------------
#
#   Copyright (c) 2019 Charles Carley.
#
#   Contributor(s): none yet
#
# ------------------------------------------------------------------------------
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

if (TableCompiler_ExternalTarget)
    set(TargetFolders ${TableCompiler_TargetFolders})
    set(TargetName    ${TableCompiler_TargetName})
    set(TargetGroup   ${TableCompiler_TargetGroup})
else()
    set(TargetFolders FALSE)
    set(TargetName TableCompiler)
    unset(TargetGroup )
endif()




include_directories(${FileTools_INCLUDE} ${Utils_INCLUDE})
link_libraries(${FileTools_LIBRARY} ${Utils_LIBRARY})

add_executable(${TargetName}  
    TableCompiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TableCompiler.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Readme.md
    )


if (MSVC)
    set_target_properties(
        ${TargetName} 
        PROPERTIES 
        VS_DEBUGGER_WORKING_DIRECTORY  
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()



if (TargetFolders)
    set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
endif()