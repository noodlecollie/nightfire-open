include(compiler_settings)

add_library(${TARGETNAME_LIB_MEMPOOL} STATIC
	include/MemPool/MemPool.h
	src/MemPool.c
)

target_include_directories(${TARGETNAME_LIB_MEMPOOL}
	PUBLIC
	include

	PRIVATE
	src
)

target_link_libraries(${TARGETNAME_LIB_MEMPOOL}
	PUBLIC
	${TARGETNAME_LIB_BUILDPLATFORM}
)

set_common_library_compiler_settings(${TARGETNAME_LIB_MEMPOOL})
