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
include(StaticRuntime)
include(ExternalTarget)
include(ConfigureCuda)
find_package (OpenMP)

if(CUDAToolkit_FOUND)
	enable_language(C CXX CUDA)
	set_static_runtime_cuda()
else()
	enable_language(C CXX)
endif()

set_static_runtime()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(RayTracer_BUILD_TEST     "Build the unit test program." ON)
option(RayTracer_AUTO_RUN_TEST  "Automatically run the test program." OFF)
option(RayTracer_USING_CUDA     "Build with CUDA support." OFF)
option(RayTracer_OPT_FAVOR_SPEED    "." ON)
option(RayTracer_OPT_GEN_INTRINSIC  "." ON)
option(RayTracer_EXTRA_DEBUG        "." ON)
option(RayTracer_SLOW_INTERSECTIONS "." OFF)
option(RayTracer_BUILD_DOCS         "." ON)
option(RayTracer_FULL_PROFILE       "." OFF)

if (NOT MSVC)
	unset(RayTracer_OPT_GEN_INTRINSIC)
	set(RayTracer_OPT_GEN_INTRINSIC FALSE)
endif()

set(BUILD_GMOCK   OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(GTEST_DIR     ${RayTracer_SOURCE_DIR}/Test/googletest)
set(GTEST_INCLUDE ${RayTracer_SOURCE_DIR}/Test/googletest/googletest/include)
set(GTEST_LIBRARY gtest_main)


set(RayTracer_INTRINSICS)

if (CUDAToolkit_FOUND AND RayTracer_USING_CUDA)
	set(USING_CUDA TRUE)
else()
	set(USING_CUDA FALSE)
endif()



if (USING_CUDA)
    add_definitions(-DUSING_CUDA)
endif()

if (RayTracer_OPT_GEN_INTRINSIC)

	if (MSVC)
		set(RayTracer_INTRINSICS "${RayTracer_INTRINSICS} /Qpar /fp:fast")
		set(RayTracer_INTRINSICS "${RayTracer_INTRINSICS} /arch:AVX2")

		if (RayTracer_OPT_FAVOR_SPEED)
			set(RayTracer_INTRINSICS "${RayTracer_INTRINSICS} /O2")
			set(RayTracer_INTRINSICS "${RayTracer_INTRINSICS} /Ot")
		else()
			set(RayTracer_INTRINSICS "${RayTracer_INTRINSICS} /O1")
			set(RayTracer_INTRINSICS "${RayTracer_INTRINSICS} /Os")
		endif()
	
		if (RayTracer_OPT_GEN_INTRINSIC)
			set(RayTracer_INTRINSICS "${RayTracer_INTRINSICS} /Oi")
		endif()

		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RayTracer_INTRINSICS}")
	else()

		set(RayTracer_INTRINSICS "${RayTracer_INTRINSICS} -O3 -Ofast")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RayTracer_INTRINSICS}")
	endif()
endif()

set(Extern_DIR ${RayTracer_SOURCE_DIR}/Extern)

DefineExternalTargetEx(Utils 
	Extern 
	${Extern_DIR} 
	${Extern_DIR}/Utils 
	${RayTracer_BUILD_TEST} 
	${RayTracer_AUTO_RUN_TEST}
)

DefineExternalTargetEx(bAscii 
	Extern 
	${Extern_DIR}/bAscii
	${Extern_DIR}/bAscii
	${RayTracer_BUILD_TEST} 
	${RayTracer_AUTO_RUN_TEST}
)

DefineExternalTargetEx(Math 
	Extern 
	${Extern_DIR} 
	${Extern_DIR}/Math 
	${RayTracer_BUILD_TEST} 
	${RayTracer_AUTO_RUN_TEST}
)

DefineExternalTargetEx(Image 
	Extern 
	${Extern_DIR}/Image 
	${Extern_DIR}/Image 
	${RayTracer_BUILD_TEST} 
	${RayTracer_AUTO_RUN_TEST}
)


set(FreeImage_SILENT TRUE)
DefineExternalTarget(FreeImage 
	Extern 
	${Extern_DIR}/FreeImage/Source
	${Extern_DIR}/FreeImage/Source
)


DefineExternalTargetEx(FileTools
	Extern 
	${Extern_DIR}/FileTools/File
	${Extern_DIR}/FileTools/File
	${RayTracer_BUILD_TEST} 
	${RayTracer_AUTO_RUN_TEST}
)

DefineExternalTargetEx(BlendFile
	Extern 
	${Extern_DIR}/FileTools/FileFormats/Blend
	${Extern_DIR}/FileTools/FileFormats/Blend
	${RayTracer_BUILD_TEST} 
	${RayTracer_AUTO_RUN_TEST}
)



DefineExternalTargetEx(TableCompiler
	Extern 
	${Extern_DIR}/FileTools/FileFormats/File
	${Extern_DIR}/FileTools/FileFormats/BlendFile
	${RayTracer_BUILD_TEST} 
	${RayTracer_AUTO_RUN_TEST}
)

set(SDL_INCLUDE ${Extern_DIR}/Extern/SDL)
set(SDL_LIBRARY SDL2-static SDL2main)
set(SDL_FOLDER Extern)


DefineExternalTargetEx(Threads 
	Extern 
	${Extern_DIR}/Threads
	${Extern_DIR}/Threads/
	FALSE 
	FALSE 
)
