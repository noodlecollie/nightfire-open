add_library(${TARGETNAME_LIB_MINIZ} STATIC
	include/miniz/miniz.h
	src/miniz.c
)

target_include_directories(${TARGETNAME_LIB_MINIZ}
	PUBLIC include
	PRIVATE src
)
