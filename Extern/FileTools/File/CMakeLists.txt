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

if (FileTools_ExternalTarget)
    set(TargetFolders ${FileTools_TargetFolders})
    set(TargetName    ${FileTools_TargetName})
    set(TargetGroup   ${FileTools_TargetGroup})
else()
    set(TargetName FileTools)
    unset(TargetGroup )
endif()


if (FileTools_USE_ZLIB)
    include_directories(${FileTools_ZLIB_INCLUDE})
    add_definitions(${FileTools_ZLIB_DEFINITIONS})
    link_libraries(${FileTools_ZLIB_LIBRARY})
endif()

set(FileTools_MaxStructMember       65536 CACHE STRING "Upper limit for the size in bytes of a single member.")
set(FileTools_MaxTableSize          5000 CACHE STRING "Maximum number of structures in a table.")
set(FileTools_MaxCharArray          64 CACHE STRING "Maximum character array length.")
set(FileTools_MaxMember             256 CACHE STRING "Maximum number of members in a struct or class.")
set(FileTools_DefaultAllocationSize 2048 CACHE STRING "Table default allocation size.")
set(FileTools_MaxArrayDim           3 CACHE STRING "Maximum N dimensional array.")
set(FileTools_TypeLengthValidate    TRUE CACHE BOOL "Write a validation file.")
set(FileTools_SwapEndian            FALSE CACHE BOOL "Swap from native endian when writing the table. (Test only)")
set(FileTools_MaxChunkArrayLength   65536 CACHE STRING "Defines the maximum number of data blocks that can be assigned to a chunk.")

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/ftConfig.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/ftConfig.h
)

set(File_SRC
    ftAtomic.cpp
    ftChunk.cpp
    ftCompiler.cpp
    ftEndianUtils.cpp
    ftFile.cpp
    ftLogger.cpp
    ftMember.cpp
    ftScanDNA.cpp
    ftScanner.cpp
    ftStreams.cpp
    ftStruct.cpp
    ftTable.cpp
    ftToken.cpp
    ftTypes.cpp
)

set(File_HDR
    ftAtomic.h
    ftChunk.h
    ftCompiler.h
    ftConfig.h
    ftConfig.h.in
    ftEndianUtils.h
    ftFile.h
    ftHashTypes.h
    ftLogger.h
    ftMember.h
    ftScanDNA.h
    ftScanner.h
    ftStreams.h
    ftStruct.h
    ftTable.h
    ftToken.h
    ftTypes.h
)

if (FileTools_USE_ZLIB)
    include_directories(${FileTools_ZLIB_INCLUDE})
    add_definitions(${FileTools_ZLIB_DEFINITIONS})
    link_libraries(${FileTools_ZLIB_LIBRARY})
endif()

include_directories(${Utils_INCLUDE})

add_library(${TargetName} 
    ${File_SRC} 
    ${File_HDR}
)

target_link_libraries(${TargetName} 
    ${Utils_LIBRARY}
)


if (TargetFolders)
    set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
endif()

