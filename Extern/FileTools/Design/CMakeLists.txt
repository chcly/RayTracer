include(FindDoxygen)

if (DOXYGEN_FOUND)
	set(TargetName Documentation)

	set(Input_CFG	 ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.txt.in)
	set(Output_CFG	 ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	set(Layout_CFG	 ${CMAKE_CURRENT_SOURCE_DIR}/Layout.xml)
	set(Output_MAIN	 ${CMAKE_SOURCE_DIR}/docs/index.html)

	set_source_files_properties(${Output_MAIN} PROPERTIES GENERATED TRUE)

	set(Target_SRC
		header.html
		footer.html
		dox.css
		navtree.css
		style.css
		${Input_CFG}
		${Output_CFG}
		${Layout_CFG}		
		${Output_MAIN}
	)
	set(Input_SRC
		Source/MainPage.inl
		Source/License.inl
	)


	source_group("Source"    FILES ${Input_SRC})
	source_group("Resource"  FILES ${Target_SRC})



	configure_file(${Input_CFG} ${Output_CFG})

	add_custom_target(${TargetName}
		COMMAND ${DOXYGEN_EXECUTABLE} ${Output_CFG}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs 
		COMMENT "Building Documentation" 
		SOURCES ${Target_SRC} ${Input_SRC}
		)
	add_custom_command(
		TARGET ${TargetName}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/navtree.css"
		 "${CMAKE_SOURCE_DIR}/docs/navtree.css"
	)


	set_target_properties(${TargetName} PROPERTIES FOLDER "CMakePredefinedTargets")

else()
	message("DOXYGEN not found")
endif()
