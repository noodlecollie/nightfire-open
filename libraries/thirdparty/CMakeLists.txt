add_subdirectory(rmlui-6.1)

if(TARGET_PLATFORM STREQUAL "LINUX")
	add_subdirectory(safestringlib-master-20230813)
endif()
