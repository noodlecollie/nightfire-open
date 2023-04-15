include(compiler_settings)

add_library(${TARGETNAME_LIB_PUBLIC} STATIC
	build.h
	build.c
	buildenums.h
	crclib.h
	crclib.c
	crtlib.h
	crtlib.c
	matrixlib.c
	miniz.h
	pstdint.h
	xash3d_mathlib.h
	xash3d_mathlib.c
)

target_link_libraries(${TARGETNAME_LIB_PUBLIC}
	PUBLIC
	${TARGETNAME_INCLUDES_SDK}

	PRIVATE
	$<$<STREQUAL:${TARGET_PLATFORM},"LINUX">: m >
)

set_common_library_compiler_settings(${TARGETNAME_LIB_PUBLIC})
