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
set(Threads_SRC
    skCriticalSection.cpp
    skMutex.cpp
    skProcessQueue.cpp
    skSemaphore.cpp
    skTimedCallback.cpp
    skThread.cpp
    skThreadUtils.cpp
)

set(Threads_HDR
    skCriticalSection.h
    skMutex.h
    skProcessQueue.h
    skSemaphore.h
    skTimedCallback.h
    skThread.h
    skThreadUtils.h
)


if (WIN32 AND NOT USING_EMSCRIPTEN)

    
	set(Threads_SRC_PLAT_EX 
        Posix/skPosixCriticalSection.cpp
        Posix/skPosixMutex.cpp
        Posix/skPosixSemaphore.cpp
        Posix/skPosixThread.cpp
        Posix/skPosixUtils.cpp
	)

	set(Threads_HDR_PLAT_EX
        Posix/skPosixCriticalSection.h
        Posix/skPosixMutex.h
        Posix/skPosixSemaphore.h
        Posix/skPosixThread.h
        Posix/skPosixUtils.h
        Windows/stubs/errno.h
        Windows/stubs/pthread.h
        Windows/stubs/unistd.h
        Windows/stubs/semaphore.h
	)
    set_source_files_properties(${Threads_SRC_PLAT_EX} PROPERTIES VS_TOOL_OVERRIDE "None")
    set_source_files_properties(${Threads_HDR_PLAT_EX} PROPERTIES VS_TOOL_OVERRIDE "None")
    set_source_files_properties(${Threads_SRC_PLAT_EX} PROPERTIES HEADER_FILE_ONLY ON)
    set_source_files_properties(${Threads_HDR_PLAT_EX} PROPERTIES HEADER_FILE_ONLY ON)

    source_group(Source\\Stubs FILES ${Threads_SRC_PLAT_EX})
    source_group(Header\\Stubs FILES ${Threads_HDR_PLAT_EX})

    set(Threads_SRC_PLAT 
        Windows/skWindowsCriticalSection.cpp
        Windows/skWindowsMutex.cpp
        Windows/skWindowsSemaphore.cpp
        Windows/skWindowsThread.cpp
        ${Threads_SRC_PLAT_EX}
	)

	set(Threads_HDR_PLAT 
        Windows/skWindowsCriticalSection.h
        Windows/skWindowsMutex.h
        Windows/skWindowsSemaphore.h
        Windows/skWindowsThread.h
        ${Threads_HDR_PLAT_EX}
	)

    set(CrossCOMPILE FALSE)
    if (CrossCOMPILE)
        include_directories(Windows/stubs)
    endif()

else()

	find_package(Threads REQUIRED)

	set(Threads_SRC_PLAT 
        Posix/skPosixCriticalSection.cpp
        Posix/skPosixMutex.cpp
        Posix/skPosixSemaphore.cpp
        Posix/skPosixThread.cpp
        Posix/skPosixUtils.cpp
	)

	set(Threads_HDR_PLAT 
        Posix/skPosixCriticalSection.h
        Posix/skPosixMutex.h
        Posix/skPosixSemaphore.h
        Posix/skPosixThread.h
        Posix/skPosixUtils.h
	)


    set(Threads_PLAT_LIBS pthread)


endif()

include_directories(../ ${Utils_INCLUDE})


if(MSVC OR USING_EMSCRIPTEN)
    source_group(Source FILES ${Threads_SRC})
    source_group(Header FILES ${Threads_HDR})
    source_group(Source\\Platform FILES ${Threads_SRC_PLAT})
    source_group(Header\\Platform FILES ${Threads_HDR_PLAT})
endif()



add_library(
    ${TargetName} 
    ${Threads_SRC}
    ${Threads_HDR}
    ${Threads_SRC_PLAT}
    ${Threads_HDR_PLAT}
)

target_link_libraries(${TargetName} ${Utils_LIBRARY} ${Threads_PLAT_LIBS})
if (TargetFolders)
    set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
endif()
