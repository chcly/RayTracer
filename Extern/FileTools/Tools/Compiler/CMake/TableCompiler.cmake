#------------------------------------------------------------------------------
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
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
#------------------------------------------------------------------------------
set(FT_EXECUTABLE TableCompiler)

# ----------------------------------------------------------------------------#
#                                                                             #
# Get a list of absolute paths                                                #
#                                                                             #
# ----------------------------------------------------------------------------#
macro(FT_ABSOLUTE_SRC TARGET)
    foreach (it ${ARGN})
        get_filename_component(N ${it} ABSOLUTE)
        set(${TARGET} ${${TARGET}} ${N})
    endforeach(it)
endmacro(FT_ABSOLUTE_SRC)

# ----------------------------------------------------------------------------#
#                                                                             #
# Get a list of names paths                                                   #
#                                                                             #
# ----------------------------------------------------------------------------#
macro(FT_BASE_SRC TARGET)
    foreach (it ${ARGN})
        get_filename_component(N ${it} NAME)
        set(${TARGET} ${${TARGET}} ${N})
    endforeach(it)
endmacro(FT_BASE_SRC)

# ----------------------------------------------------------------------------#
#                                                                             #
#   Usage: ${FT_EXECUTABLE} ${OUTFILE} {ARGN}                                #
#                                                                             #
# ----------------------------------------------------------------------------#
macro(ADD_TABLE TARGET)
    set(SRC_FILES )
    set(BASE_FILES )
    set(OUTFILE )

    ft_absolute_src(SRC_FILES ${ARGN})
    ft_base_src(BASE_FILES ${ARGN})
    get_filename_component(__TargetName__ ${TARGET} NAME)
   
    set(OUTFILE ${CMAKE_CURRENT_BINARY_DIR}/${__TargetName__}.inl)

    # Add the output directory to the current include 
    # list since it needs to be included in-order to use it
    include_directories(${CMAKE_CURRENT_BINARY_DIR})

    add_custom_command(
	    OUTPUT ${OUTFILE}
	    COMMAND ${FT_EXECUTABLE} ${__TargetName__} ${OUTFILE} ${SRC_FILES}
	    DEPENDS ${FT_EXECUTABLE} ${SRC_FILES}
	    )

    set(${TARGET} ${OUTFILE})

endmacro(ADD_TABLE)

# ----------------------------------------------------------------------------#
#                                                                             #
#   Usage: ${FT_EXECUTABLE} ${OUTFILE} {ARGN}                                #
#          Writes a validation target                                         #
#                                                                             #
# ----------------------------------------------------------------------------#
macro(ADD_TABLE_VALIDATOR TARGET)
    
    set(SRC_FILES )
    set(BASE_FILES )
    set(OUTFILE )

    get_filename_component(Result ${ARGN} EXT)
    get_filename_component(FileName ${ARGN} NAME)
    string(REPLACE ${Result} "" BaseName ${FileName})

    set(OutTable      ${CMAKE_CURRENT_BINARY_DIR}/${BaseName}.inl)
    set(OutValidator  ${CMAKE_CURRENT_BINARY_DIR}/${BaseName}Validator.cpp)

    # Add the output directory to the current include 
    # list since it needs to be included in-order to use it
    include_directories(${CMAKE_CURRENT_BINARY_DIR})

    add_custom_command(
	    OUTPUT ${OutTable} ${OutValidator}
	    COMMAND ${FT_EXECUTABLE} ${BaseName} ${OutTable} ${ARGN}
	    DEPENDS ${FT_EXECUTABLE} ${ARGN}
	    COMMENT ""
	    )


    add_executable(${BaseName}Validator ${OutValidator})

    add_custom_command(
        TARGET ${BaseName}Validator
        POST_BUILD
	    COMMAND ${BaseName}Validator
	    COMMENT "Validating ${TARNAME}"
	    )

    set(${TARGET} ${OutTable})
endmacro(ADD_TABLE_VALIDATOR)

