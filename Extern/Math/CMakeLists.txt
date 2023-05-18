# -----------------------------------------------------------------------------
#   Copyright (c) 2019 Charles Carley.
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
cmake_minimum_required(VERSION 3.0)
project(Math)
option(Math_SCALAR_DOUBLE "Define scalar type as double" OFF)

if (Math_ExternalTarget)
    set(TargetFolders ${Math_TargetFolders})
    set(TargetName    ${Math_TargetName})
    set(TargetGroup   ${Math_TargetGroup})
else()
    set(TargetFolders FALSE)
    set(TargetName Math)
    unset(TargetGroup )
endif()

set(Math_SRC
    skBoundingBox.cpp
    skBoundingSphere.cpp
    skBoundingBox2D.cpp
    skColor.cpp
    skEuler.cpp
    skMath.cpp
    skMatrix3.cpp
    skMatrix4.cpp
    skPlane.cpp
    skQuaternion.cpp
    skRandom.cpp
    skRational.cpp
    skRay.cpp
    skRectangle.cpp
    skTransform2D.cpp
    skVector2.cpp
    skVector3.cpp
    skVector4.cpp
)

set(Math_HDR
    skBoundingBox.h
    skBoundingSphere.h
    skBoundingBox2D.h
    skColor.h
    skEuler.h
    skFoot.h
    skMath.h
    skMatrix3.h
    skMatrix4.h
    skPlane.h
    skQuaternion.h
    skRandom.h
    skRational.h
    skRay.h
    skRectangle.h
    skScalar.h
    skScreenTransform.h
    skTransform2D.h
    skVector2.h
    skVector3.h
    skVector4.h
)

include_directories(../ .)
add_library(${TargetName} ${Math_SRC} ${Math_HDR})

if (Math_SCALAR_DOUBLE)
   add_definitions(-DSK_DOUBLE)
endif()

set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")

