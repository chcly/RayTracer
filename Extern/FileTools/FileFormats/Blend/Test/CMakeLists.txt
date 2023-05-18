#------------------------------------------------------------------------------
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
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
#------------------------------------------------------------------------------
set(TargetName BlendFileTest)
list(APPEND CMAKE_MODULE_PATH  ${FileTools_SOURCE_DIR}/Tools/SwapChunkCodes/CMake)

set(ExtraFileDir ${CMAKE_CURRENT_BINARY_DIR})
configure_file(ExtraFiles.in "${CMAKE_CURRENT_BINARY_DIR}/ExtraFiles.h")


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ 
    ${BlendFile_INCLUDE} 
    ${FileTools_INCLUDE}
    ${Utils_INCLUDE}
    ${GTEST_INCLUDE} 
    )
include(SwapChunkCodes)

set(FilesToCorrupt Test.blend)
corrupt_file(TrashedFiles ${FilesToCorrupt})



file(GLOB Blends *.blend)
set(Target_Source 
  main.cpp
  ExtraFiles.in
  ${Blends}
  ${TrashedFiles}
)



add_executable(${TargetName} ${Target_Source})

if (FileTools_AUTO_RUN_TEST)

   add_custom_command(TARGET 
        ${TargetName} POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND  $<TARGET_FILE:${TargetName}>
    )

endif()


if (MSVC)
    set_target_properties(
        ${TargetName} 
        PROPERTIES 
        VS_DEBUGGER_WORKING_DIRECTORY  
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

target_link_libraries(
    ${TargetName}
    ${BlendFile_LIBRARY}
    ${FileTools_LIBRARY}
    ${Utils_LIBRARY}
    ${GTEST_LIBRARY}
)

if (TargetFolders)
    set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
endif()
