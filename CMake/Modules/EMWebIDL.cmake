if (USING_EMSCRIPTEN_STD)
    find_file(EmWebIDL_BINDER NAMES webidl_binder.py PATHS "$ENV{EMSDK}/upstream/emscripten/tools")
    find_package (Python COMPONENTS Interpreter)

    if (NOT EmWebIDL_BINDER)
        if (EXISTS "$ENV{EMSDK}/upstream/emscripten/tools/webidl_binder.py")
            set(EmWebIDL_BINDER  "$ENV{EMSDK}/upstream/emscripten/tools/webidl_binder.py")
        else()
            message("webidl_binder.py was not found in the EMSDK environment '$ENV{EMSDK}/upstream/emscripten/tools' directory.")
        endif()
    endif()

    if (NOT Python_FOUND)
        message("A required python interpreter was not found")
    endif()

    if (EmWebIDL_BINDER AND Python_FOUND)

        macro(EmBindWebIDL OUT)
	        foreach( InputFile ${ARGN} )
        
                get_filename_component(InputFileBaseName ${InputFile} NAME_WE)


                list(APPEND ${OUT} "${CMAKE_CURRENT_BINARY_DIR}/${InputFileBaseName}Glue.js")
                list(APPEND ${OUT} "${CMAKE_CURRENT_BINARY_DIR}/${InputFileBaseName}Glue.cpp")


                add_custom_command(
                        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${InputFileBaseName}Glue.js
                        PRE_BUILD
                        COMMAND Python::Interpreter 
                            ${EmWebIDL_BINDER} ${InputFile} ${CMAKE_CURRENT_BINARY_DIR}/${InputFileBaseName}Glue
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )

                list(APPEND ${OUT} "${CMAKE_CURRENT_BINARY_DIR}/${InputFileBaseName}Glue.js")
                list(APPEND ${OUT} "${CMAKE_CURRENT_BINARY_DIR}/${InputFileBaseName}Glue.cpp")

	        endforeach()

        endmacro(EmBindWebIDL)

    endif()
else()
    if (USING_EMSCRIPTEN)
        message("The EmBindWebIDL macro should be used in an emcmake environment")
    endif()
endif()
