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


configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/rtConfig.h.in  
    ${CMAKE_CURRENT_SOURCE_DIR}/rtConfig.h)


# ---------- Main (.h) ----------

set(TargetName_HDR
    rtClientObject.h
    rtBvObject.h
    rtCamera.h
    rtCommon.h
    rtCube.h
    rtImageTarget.h
    rtInteractiveCamera.h
    rtLight.h
    rtMaterial.h
    rtMesh.h
    rtObject.h
    rtPlane.h
    rtScene.h
    rtSphere.h
    rtRenderSystem.h
    rtTarget.h
    rtTickState.h
    rtTimeProfile.h
    rtConfig.h
)


# ---------- CPU (.h) ----------

set(TargetName_CPU_HDR
    Cpu/rtCpuRenderSystem.h
    Cpu/rtCpuKernel.h
    Cpu/rtCpuMath.h
    Cpu/rtTileManager.h
)

set(TargetName_DAT_HDR
    Data/rtAllocator.h
    Data/rtArray.h
    Data/rtBackendTypes.h
    Data/rtCameraTypes.h
    Data/rtLightTypes.h
    Data/rtMaterialTypes.h
    Data/rtMeshTypes.h
    Data/rtObjectTypes.h
    Data/rtShapeTypes.h
    Data/rtSceneType.h
)


# ---------- DBG (.h) ----------

set(TargetName_DBG_HDR

)


# ---------- Math (.h) ----------

set(TargetName_MTH_HDR
    Math/rtColor.h
    Math/rtVectorTypes.h
    Math/rtMath.h
)


# ---------- Loader (.h) ----------

set(TargetName_LDR_HDR
    Loader/rtLoader.h
    Loader/Ascii/rtAsciiLoader.h
    Loader/Blend/rtBlendLoader.h
)



# ---------- Viewer (.h) ----------

set(TargetName_VWR_HDR
    Viewer/rtViewerImpl.h
)


if (USING_CUDA)
    set(TargetName_CUDA_HDR
        Cuda/rtCudaUtils.h
        Cuda/rtCudaHostMath.h
        Cuda/rtCudaRenderSystem.h
        Cuda/rtCuMath.cuh
    )

    set(TargetName_CUDA_REF
        Cuda/rtCuMath.inl
    )

    set(TargetName_CUDA_SRC
        Cuda/rtCudaUtils.cpp
        Cuda/rtCudaRenderSystem.cpp
        Cuda/rtKernel.cu
        ${TargetName_CUDA_REF}
    )
endif()


# ---------- Main (.cpp) ----------

set(TargetName_SRC
    rtBvObject.cpp
    rtCamera.cpp
    rtCube.cpp
    rtImageTarget.cpp
    rtInteractiveCamera.cpp
    rtLight.cpp
    rtMaterial.cpp
    rtMesh.cpp
    rtObject.cpp
    rtPlane.cpp
    rtScene.cpp
    rtSphere.cpp
    rtRenderSystem.cpp
    rtTickState.cpp
    rtTimeProfile.cpp
)

# ---------- CPU (.cpp) ----------

set(TargetName_CPU_SRC
    Cpu/rtCpuRenderSystem.cpp
    Cpu/rtCpuKernel.cpp
    Cpu/rtCpuMath.cpp
    Cpu/rtTileManager.cpp
)

# ---------- DBG (.cpp) ----------

set(TargetName_DBG_SRC
)


# ---------- Math (.cpp) ----------

set(TargetName_MTH_SRC
)


# ---------- Loader (.cpp) ----------

set(TargetName_LDR_SRC
    Loader/rtLoader.cpp
    Loader/Ascii/rtAsciiLoader.cpp
    Loader/Blend/rtBlendLoader.cpp
)

# ---------- Viewer (.cpp) ----------

set(TargetName_VWR_SRC
    Viewer/rtViewerImpl.cpp
)

set(TargetName_DAT_SRC
    Data/rtAllocator.cpp
)


if (USING_CUDA)
    add_definitions(-DUSING_CUDA)
endif()

if (RayTracer_OPT_GEN_INTRINSIC)
    add_definitions(-DRT_USE_SIMD)
endif()


# add_definitions(-DRT_EXTRA_DEBUG)
include_directories(
    ../
    ${Utils_INCLUDE}
    ${Math_INCLUDE}
    ${Image_INCLUDE}
    ${FileTools_INCLUDE}
    ${BlendFile_INCLUDE}
    ${SDL_INCLUDE}
    ${Cuda_INCLUDE}
    ${Threads_INCLUDE}
    ${bAscii_INCLUDE}
    ${CMAKE_CURRENT_BINARY_DIR}
)


if (MSVC)
    source_group("Header" FILES ${TargetName_HDR})
    source_group("Source" FILES ${TargetName_SRC})

    source_group("Header\\Cpu" FILES ${TargetName_CPU_HDR})
    source_group("Source\\Cpu" FILES ${TargetName_CPU_SRC})

    source_group("Header\\Math" FILES ${TargetName_MTH_HDR})
    source_group("Source\\Math" FILES ${TargetName_MTH_SRC})

    source_group("Header\\Loader" FILES ${TargetName_LDR_HDR})
    source_group("Source\\Loader" FILES ${TargetName_LDR_SRC})

    source_group("Header\\Debug" FILES ${TargetName_DBG_HDR})
    source_group("Source\\Debug" FILES ${TargetName_DBG_SRC})

    source_group("Header\\Viewer" FILES ${TargetName_VWR_HDR})
    source_group("Source\\Viewer" FILES ${TargetName_VWR_SRC})

    source_group("Header\\Cuda" FILES ${TargetName_CUDA_HDR})
    source_group("Source\\Cuda" FILES ${TargetName_CUDA_SRC})


    source_group("Header\\Types" FILES ${TargetName_DAT_HDR})
    source_group("Source\\Types" FILES ${TargetName_DAT_SRC})

endif()


add_library(
    ${TargetName} 
    ${TargetName_HDR}      ${TargetName_SRC} 
    ${TargetName_DAT_HDR}  ${TargetName_DAT_SRC}
    ${TargetName_CPU_HDR}  ${TargetName_CPU_SRC}
    ${TargetName_MTH_HDR}  ${TargetName_MTH_SRC}
    ${TargetName_LDR_HDR}  ${TargetName_LDR_SRC}
    ${TargetName_DBG_HDR}  ${TargetName_DBG_SRC}
    ${TargetName_VWR_HDR}  ${TargetName_VWR_SRC}
    ${TargetName_CUDA_HDR} ${TargetName_CUDA_SRC}
    ${TargetName_ASM}
)


target_link_libraries(${TargetName} 
    ${Utils_LIBRARY}
    ${Math_LIBRARY}
    ${Image_LIBRARY}
    ${FileTools_LIBRARY}
    ${BlendFile_LIBRARY}
    ${SDL_LIBRARY}
    ${Cuda_LIBRARY}
    ${Threads_LIBRARY}
    ${bAscii_LIBRARY}
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(${TargetName}  OpenMP::OpenMP_CXX)
endif()


if (TargetFolders)
    set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
endif()

if (MSVC)

    set_target_properties(
        ${TargetName} 
        PROPERTIES 
        VS_DEBUGGER_WORKING_DIRECTORY 
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

endif()
