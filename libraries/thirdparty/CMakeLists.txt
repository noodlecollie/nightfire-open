add_subdirectory(nuklear-4.12.0)

if(TARGET_PLATFORM STREQUAL "LINUX")
	add_subdirectory(safestringlib-master-20230813)
endif()
