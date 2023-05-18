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


macro(DefineExternalTarget NAME GROUP INCLUDE )
    set(${NAME}_ExternalTarget      TRUE)
    set(${NAME}_TargetFolders       TRUE)
    set(${NAME}_TargetName          ${NAME})
    set(${NAME}_TargetGroup         ${GROUP})
    set(${NAME}_FOLDER              ${GROUP})
    set(${NAME}_INCLUDE             ${INCLUDE})
    set(${NAME}_LIBRARY             ${${NAME}_TargetName})
endmacro()



macro(DefineExternalTargetEx NAME GROUP INCLUDE_PATH SOURCE_DIR BLD_TEST RUN_TEST)
    set(${NAME}_ExternalTarget      TRUE)
    set(${NAME}_TargetFolders       TRUE)
    set(${NAME}_TargetName          ${NAME})
    set(${NAME}_TargetGroup         ${GROUP})
    set(${NAME}_FOLDER              ${GROUP})
    set(${NAME}_INCLUDE             ${INCLUDE_PATH})
    set(${NAME}_DIRECTORY           ${SOURCE_DIR})
    set(${NAME}_LIBRARY             ${${NAME}_TargetName})
    set(${NAME}_BUILD_TEST          ${BLD_TEST})
    set(${NAME}_AUTO_RUN_TEST       ${RUN_TEST})
    set(${NAME}_AUTO_RUN_TESTS      ${RUN_TEST})
endmacro()
