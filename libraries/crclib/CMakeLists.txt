include(compiler_settings)

add_library(${TARGETNAME_LIB_CRCLIB} STATIC
	include/CRCLib/crclib.h
	src/crclib.c
)

target_include_directories(${TARGETNAME_LIB_CRCLIB}
	PUBLIC include
	PRIVATE src
)

set_common_library_compiler_settings(${TARGETNAME_LIB_CRCLIB})
