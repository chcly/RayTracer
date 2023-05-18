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
set_static_runtime()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
option(Threads_BUILD_TEST "Build the template test program." OFF)
option(Threads_AUTO_RUN_TEST "Run the test program after a successful build." OFF)



set(Utils_ExternalTarget TRUE)
set(Utils_TargetFolders  TRUE)
set(Utils_TargetName     Utils)
set(Utils_TargetGroup    Extern)
set(Utils_INCLUDE        ${Threads_SOURCE_DIR}/Extern)
set(Utils_LIBRARY        ${Utils_TargetName})

set(GTEST_INCLUDE ${Threads_SOURCE_DIR}/Test/googletest/googletest/include)
set(GTEST_LIBRARY gtest_main)
