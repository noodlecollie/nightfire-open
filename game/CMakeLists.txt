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

endif()
