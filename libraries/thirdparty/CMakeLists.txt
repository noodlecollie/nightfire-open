add_subdirectory(miniz-3.0.2)

if(TARGET_PLATFORM STREQUAL "LINUX")
	add_subdirectory(safestringlib-master-20230813)
endif()
