if(NOT "${NF_INSTALL_MODE}" STREQUAL "CONTENT")
	add_subdirectory(miniutl)
	add_subdirectory(game_libs)
endif()

if(NOT "${NF_INSTALL_MODE}" STREQUAL "BINARIES")
	add_subdirectory(content)
endif()
