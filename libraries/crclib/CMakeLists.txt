add_library(${TARGETNAME_LIB_CRCLIB} STATIC
	include/CRCLib/crclib.h
	src/crclib.c
)

target_include_directories(${TARGETNAME_LIB_CRCLIB}
	PUBLIC include
	PRIVATE src
)
