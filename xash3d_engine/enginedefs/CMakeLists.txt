add_library(${TARGETNAME_LIB_ENGINEDEFS} INTERFACE
	include/XashDefs/engine_limits.h
	include/XashDefs/mempool.h
)

target_compile_definitions(${TARGETNAME_LIB_ENGINEDEFS} INTERFACE
	BUILDVAR_VERSION_STRING="${VCS_TAG_ID}"
)

target_include_directories(${TARGETNAME_LIB_ENGINEDEFS} INTERFACE
	include
)

target_link_libraries(${TARGETNAME_LIB_ENGINEDEFS} INTERFACE
	${TARGETNAME_LIB_PLATFORMDEFS}
)
