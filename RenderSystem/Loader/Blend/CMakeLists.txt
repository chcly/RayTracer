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
set(TargetName_HDR
    rtBvhNode.h
    rtBvObject.h
    rtCamera.h
    rtCommon.h
    rtCube.h
    rtImageTarget.h
    rtInteractiveCamera.h
    rtLight.h
    rtLoader.h
    rtMaterial.h
    rtMesh.h
    rtTransformState.h
    rtOctree.h
    rtPlane.h
    rtScene.h
    rtSceneBvh.h
    rtSphere.h
    rtRenderSystem.h
    rtTarget.h
    rtTickState.h
    rtTile.h
    rtTileManager.h


    Ascii/rtAsciiLoader.h
    Blend/rtBlendLoader.h

    Debug/rtDebugBVH.h
    Debug/rtDebugOctree.h

    Math/skBoundingBox.h
    Math/skBoundingSphere.h
    Math/skRay.h
    Math/skPlane.h
    Math/rtBoundingSphere.h
    Math/rtColor.h
    Math/rtVector2.h
    Math/rtScalar.h
    Math/rtVector3.h
    Math/rtMath.h
    Math/rtRay.h

    Utils/skArrayBinaryTree.h
    Viewer/rtViewer.h
    Viewer/rtViewerImpl.h
)

if (USING_CUDA)
    add_definitions(-DUSING_CUDA)
    set(Target_CUDA
        Cuda/rtCudaUtils.h
        Cuda/rtCudaUtils.cpp
        Cuda/rtCuArray.cu
        Cuda/rtCuArray.cuh
        Cuda/rtCuRender.cu
    )
endif()

set(TargetName_SRC
    rtBvhNode.cpp
    rtBvObject.cpp
    rtCamera.cpp
    rtCube.cpp
    rtImageTarget.cpp
    rtInteractiveCamera.cpp
    rtLight.cpp
    rtLoader.cpp
    rtMaterial.cpp
    rtMesh.cpp
    rtTransformState.cpp
    rtOctree.cpp
    rtPlane.cpp
    rtScene.cpp
    rtSceneBvh.cpp
    rtSphere.cpp
    rtRenderSystem.cpp
    rtTarget.cpp
    rtTickState.cpp
    rtTile.cpp
    rtTileManager.cpp

    Debug/rtDebugBVH.cpp
    Debug/rtDebugOctree.cpp

    Ascii/rtAsciiLoader.cpp
    Blend/rtBlendLoader.cpp

    Viewer/rtViewer.cpp
    Viewer/rtViewerImpl.cpp
)


# add_definitions(-DRT_EXTRA_DEBUG)
include_directories(
    .
    ${Utils_INCLUDE}
    ${Math_INCLUDE}
    ${Image_INCLUDE}
    ${FileTools_INCLUDE}
    ${BlendFile_INCLUDE}
    ${SDL_INCLUDE}
    ${Cuda_INCLUDE}
    ${Threads_INCLUDE}
    ${bAscii_INCLUDE}
)

add_library(
    ${TargetName} 
    ${TargetName_SRC}
    ${TargetName_HDR}
    ${Target_CUDA}
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
