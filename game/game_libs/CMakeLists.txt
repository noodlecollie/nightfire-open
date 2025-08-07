add_subdirectory(dlls)

if(NOT NF_DEDICATED_SERVER)
	add_subdirectory(cl_dlls)
	add_subdirectory(ui)
endif()
