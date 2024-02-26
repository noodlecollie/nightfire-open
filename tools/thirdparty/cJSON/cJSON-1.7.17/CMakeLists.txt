add_library(cJSON STATIC
	cJSON.h
	cJSON.c
	cJSON_Utils.h
	cJSON_Utils.c
)

target_include_directories(cJSON PUBLIC .)

if(NOT "${TARGET_PLATFORM}" STREQUAL "WINDOWS")
	target_link_libraries(cJSON PRIVATE m)
endif()
