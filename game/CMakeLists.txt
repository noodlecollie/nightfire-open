add_subdirectory(game_libs)

if(NOT "${NF_FULL_CONTENT_PATH}" STREQUAL "")
	install(DIRECTORY "${NF_FULL_CONTENT_PATH}/content/${GAME_FOLDER_NAME}" DESTINATION ${INSTALL_ROOT})

	find_package(Python3)

	if(Python3_FOUND)
		add_custom_target(check-content-hash
			COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/check-content-hash.py" "${NF_FULL_CONTENT_PATH}"
		)

		add_custom_target(update-content-hash
			COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/update-content-hash.py" "${NF_FULL_CONTENT_PATH}"
		)

		install(CODE "execute_process(COMMAND \"${Python3_EXECUTABLE}\" \"${CMAKE_CURRENT_SOURCE_DIR}/update-content-hash.py\" \"${NF_FULL_CONTENT_PATH}\" WORKING_DIRECTORY \"${CMAKE_CURRENT_SOURCE_DIR}\")")
	endif()

	# For now, we assume the install prefix is not an absolute path
	set(GAME_DIR "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}/${INSTALL_ROOT}/${GAME_FOLDER_NAME}")

	if(WIN32)
		cmake_path(NATIVE_PATH Python3_EXECUTABLE NORMALIZE Python3_EXECUTABLE_WINPATH)
		cmake_path(NATIVE_PATH CMAKE_CURRENT_SOURCE_DIR NORMALIZE CMAKE_CURRENT_SOURCE_DIR_WINPATH)
		cmake_path(NATIVE_PATH NF_FULL_CONTENT_PATH NORMALIZE NF_FULL_CONTENT_PATH_WINPATH)
		cmake_path(NATIVE_PATH GAME_DIR NORMALIZE GAME_DIR_WINPATH)

		configure_file(
			"${CMAKE_CURRENT_SOURCE_DIR}/run-content-copyback-windows.bat.in"
			"${CMAKE_CURRENT_SOURCE_DIR}/run-content-copyback-windows.bat"
			@ONLY
			NEWLINE_STYLE WIN32
		)
	else()
		configure_file(
			"${CMAKE_CURRENT_SOURCE_DIR}/run-content-copyback-linux.sh.in"
			"${CMAKE_CURRENT_SOURCE_DIR}/run-content-copyback-linux.sh"
			@ONLY
			NEWLINE_STYLE UNIX
		)
	endif()
endif()
