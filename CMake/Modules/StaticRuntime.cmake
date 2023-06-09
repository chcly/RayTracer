## Needs CMake v3.15
macro(set_static_runtime)
    if(MSVC)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
		#message(STATUS "CMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}")
    endif()
endmacro()

macro(set_dynamic_runtime)
    if(MSVC)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
		#message(STATUS "CMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}")
    endif()
endmacro()

macro(set_static_runtime_cuda)
    if(MSVC)
    	set(CMAKE_MSVC_RUNTIME_LIBRARY_DEFAULT "MultiThreadedDebug")
    endif()
endmacro()

macro(msvc_disable_crt_warnings)
    if(MSVC)
        add_definitions("-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE")
    endif()
endmacro()
