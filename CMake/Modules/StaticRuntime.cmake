

macro(set_static_runtime)
    if(MSVC)
        foreach(flag_var    CMAKE_CXX_FLAGS 
	                        CMAKE_CXX_FLAGS_DEBUG 
	                        CMAKE_CXX_FLAGS_RELEASE 
	                        CMAKE_CXX_FLAGS_MINSIZEREL 
	                        CMAKE_CXX_FLAGS_RELWITHDEBINFO
	                        CMAKE_C_FLAGS_DEBUG
	                        CMAKE_C_FLAGS_RELEASE
	                        CMAKE_C_FLAGS_MINSIZEREL 
	                        CMAKE_C_FLAGS_RELWITHDEBINFO
	                        )
		    if(${flag_var} MATCHES "/MD")
			    string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
		    endif(${flag_var} MATCHES "/MD")

            if(${flag_var} MATCHES "/MDd")
			    string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
		    endif(${flag_var} MATCHES "/MDd")
        endforeach(flag_var)
    endif(MSVC)
endmacro(set_static_runtime)

macro(set_static_runtime_cuda)
    if(MSVC)
		set(CMAKE_MSVC_RUNTIME_LIBRARY_DEFAULT "MultiThreadedDebug")
    endif(MSVC)
endmacro(set_static_runtime_cuda)

macro(msvc_disable_crt_warnings)
    if(MSVC)
        add_definitions("-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE")
    endif()
endmacro()
