include(compiler_settings)

add_library(${TARGETNAME_LIB_COMMONUTILS} STATIC
	include/PublicLib/crclib.h
	include/PublicLib/crtlib.h
	include/PublicLib/miniz.h
	include/PublicLib/xash3d_mathlib.h
	src/build.c
	src/buildenums.h
	src/crclib.c
	src/crtlib.c
	src/matrixlib.c
	src/xash3d_mathlib.c
)

target_include_directories(${TARGETNAME_LIB_COMMONUTILS}
	PUBLIC include
	PRIVATE src
)

target_link_libraries(${TARGETNAME_LIB_COMMONUTILS}
	PUBLIC
	${TARGETNAME_INCLUDES_SDK}
	${TARGETNAME_LIB_PLATFORMLIB}
	${TARGETNAME_LIB_BUILDDEFS}

	PRIVATE
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:m>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_COMMONUTILS})
