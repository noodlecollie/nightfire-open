add_library(${TARGETNAME_LIB_CJSON} STATIC
	cJSON.h
	cJSON.c
	cJSON_Utils.h
	cJSON_Utils.c
)

target_include_directories(${TARGETNAME_LIB_CJSON} PUBLIC .)
target_compile_definitions(${TARGETNAME_LIB_CJSON} PUBLIC CJSON_HIDE_SYMBOLS)

if(NOT "${TARGET_PLATFORM}" STREQUAL "WINDOWS")
	target_link_libraries(${TARGETNAME_LIB_CJSON} PRIVATE m)
endif()
