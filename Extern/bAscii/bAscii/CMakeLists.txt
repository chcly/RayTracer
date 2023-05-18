include_directories(${Utils_INCLUDE})
add_executable(bAsciiKeywords MakeKeywords/Main.cpp)
set_target_properties(bAsciiKeywords PROPERTIES FOLDER "Tools")
target_link_libraries(bAsciiKeywords ${Utils_LIBRARY})

set(bAsciiKeywords_IN  "${CMAKE_CURRENT_SOURCE_DIR}/bAsciiKeywords.txt")
set(bAsciiKeywords_OUT "${CMAKE_CURRENT_SOURCE_DIR}/bAsciiKeywords.inl")

add_custom_command(
	OUTPUT  ${bAsciiKeywords_OUT}
	COMMAND bAsciiKeywords ${bAsciiKeywords_IN}
	DEPENDS bAsciiKeywords ${bAsciiKeywords_IN}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Converting ${bAsciiKeywords_IN}"
	)


set(Target_SRC 
	bAscii.cpp
	bAsciiCompiler.cpp
	bAsciiLinker.cpp
	bAsciiScanner.cpp
	bAsciiKeywords.cpp
	bAsciiBlock.cpp
	${bAsciiKeywords_IN}
	${bAsciiKeywords_OUT}
)

set(Target_HDR 
	bApi.h
	bAscii.h
	bAsciiMain.h
	bAsciiCommon.h
	bAsciiCompiler.h
	bAsciiLinker.h
	bAsciiOpCodes.h
	bAsciiScanner.h
	bAsciiKeywords.h
	bAsciiBlock.h
	bAsciiArgument.h
)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}
					${bAscii_INCLUDE}
					${Utils_INCLUDE}
					)

add_library(${TargetName}
            ${Target_SRC} 
	        ${Target_HDR})

set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
