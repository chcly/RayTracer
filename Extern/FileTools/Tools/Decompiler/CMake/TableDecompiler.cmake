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
set(FT_DECOMPILER TableDecompiler)


# This will call the TableDecompiler for each 
# input binary and convert it back to a .h

macro(decompile_files TARGET FLAGS)
    set(SRC_FILES )
    set(BASE_FILES )
    set(OUTFILE )

    include_directories(${CMAKE_CURRENT_BINARY_DIR})

    set(OUTFILES )
    foreach(InputFile ${ARGN})
        
        get_filename_component(Result ${InputFile} EXT)
        get_filename_component(FileName ${InputFile} NAME)
        string(REPLACE ${Result} "" FileName ${FileName})

        string(REPLACE ${Result} ".h" OuputFile ${InputFile})
        string(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR} 
            ${CMAKE_CURRENT_BINARY_DIR} 
            Output 
            ${OuputFile})

        add_custom_command(
	        OUTPUT ${Output}
	        COMMAND ${FT_DECOMPILER} ${FLAGS} -n ${FileName} -s -i "${InputFile}" -o "${Output}"
	        DEPENDS ${FT_DECOMPILER} ${ARGN}
            COMMENT "${OuputFile}"
	    )



        list(APPEND OUTFILES ${Output})
    endforeach()
    set(${TARGET} ${OUTFILES})

endmacro(decompile_files)