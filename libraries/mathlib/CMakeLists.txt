include(compiler_settings)

add_library(${TARGETNAME_LIB_MATHLIB} STATIC
	include/MathLib/mathdefs.h
	include/MathLib/mathlib.h
	include/MathLib/mathtypes.h
	include/MathLib/matrixlib.h
	include/MathLib/vec2.h
	include/MathLib/vec3.h
	src/mathlib.c
	src/matrixlib.c
)

target_include_directories(${TARGETNAME_LIB_MATHLIB}
	PUBLIC include
	PRIVATE src
)

target_link_libraries(${TARGETNAME_LIB_MATHLIB}
	PUBLIC
	${TARGETNAME_LIB_PLATFORMDEFS}
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:m>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_MATHLIB})
