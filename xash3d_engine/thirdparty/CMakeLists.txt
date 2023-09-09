add_subdirectory(miniz-3.0.2)

if(NOT NF_DEDICATED_SERVER)
	add_subdirectory(opus)
endif()
