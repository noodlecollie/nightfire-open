include(compiler_settings)

add_library(${TARGETNAME_LIB_CRTLIB} STATIC
	include/CRTLib/bitdefs.h
	include/CRTLib/crtlib.h
	src/crtlib.c
)

target_include_directories(${TARGETNAME_LIB_CRTLIB}
	PUBLIC include
	PRIVATE src
)

target_link_libraries(${TARGETNAME_LIB_CRTLIB}
	PUBLIC
	${TARGETNAME_LIB_BUILDPLATFORM}

	PRIVATE
	${TARGETNAME_LIB_PLATFORMLIB}
)

set_common_library_compiler_settings(${TARGETNAME_LIB_CRTLIB})
