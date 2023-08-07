if(NOT NF_INSTALL_CONTENT_ONLY)
	add_subdirectory(miniutl)
	add_subdirectory(game_libs)
endif()

add_subdirectory(content)
