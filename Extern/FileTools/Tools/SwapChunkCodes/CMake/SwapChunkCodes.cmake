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
set(FT_FILE_CORRUPT SwapChunkCodes)


# This will call the SwapChunkCodes program for each 
# input file and generate corrupt files from the input.

macro(corrupt_file TARGET)

    include_directories(${CMAKE_CURRENT_BINARY_DIR})

    set(SwapOptions
         0 
         1
         2
         3
         4
         5
         6
         7
         8
    )

    set(OUTFILES )
    foreach(InputFile ${ARGN})
        
        get_filename_component(BaseName ${InputFile} NAME_WE) 
        get_filename_component(FullName ${InputFile} NAME) 
        get_filename_component(Ext ${InputFile} EXT) 
        get_filename_component(AbsInput ${InputFile} ABSOLUTE)
        
        
        string(REPLACE 
            ${CMAKE_CURRENT_SOURCE_DIR} 
            ${CMAKE_CURRENT_BINARY_DIR}
            AbsOutput 
            ${AbsInput})

        foreach(opt ${SwapOptions})
            set(OutName  "${BaseName}_Junked${opt}.blend")

            string(REPLACE
                ${FullName}   
                ${OutName}
                CurFile
                ${AbsOutput}
            )

            add_custom_command(
	            OUTPUT ${CurFile}
	            COMMAND ${FT_FILE_CORRUPT} -s ${opt} -o "${CurFile}" "${AbsInput}"
	            DEPENDS ${FT_FILE_CORRUPT} ${AbsInput}
                COMMENT "Corrupting ${FullName} as ${OutName}"
	        )
            list(APPEND OUTFILES ${CurFile})

        endforeach()


    endforeach()
    set(${TARGET} ${OUTFILES})

endmacro(corrupt_file)