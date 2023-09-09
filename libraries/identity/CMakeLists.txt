add_library(${TARGETNAME_LIB_IDENTITY} STATIC
	include/Identity/arch.h
	src/arch.c
	src/buildenums.h
)

target_include_directories(${TARGETNAME_LIB_IDENTITY}
	PUBLIC include
	PRIVATE src
)

target_link_libraries(${TARGETNAME_LIB_IDENTITY}
	PUBLIC
	${TARGETNAME_LIB_PLATFORMDEFS}

	PRIVATE
	${TARGETNAME_LIB_CRTLIB}
)

set_common_library_compiler_settings(${TARGETNAME_LIB_IDENTITY})
