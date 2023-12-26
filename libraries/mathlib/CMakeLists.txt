include(compiler_settings)

add_library(${TARGETNAME_LIB_MATHLIB} STATIC
	include/MathLib/angles.h
	include/MathLib/mat3x3.h
	include/MathLib/mat3x4.h
	include/MathLib/mat4x4.h
	include/MathLib/mathdefs.h
	include/MathLib/mathlib.h
	include/MathLib/plane.h
	include/MathLib/quaternion.h
	include/MathLib/utils.h
	include/MathLib/vec2.h
	include/MathLib/vec3.h
	include/MathLib/vec4.h
	src/angles.c
	src/mat3x3.c
	src/mat3x4.c
	src/mat4x4.c
	src/mathlib.c
	src/plane.c
	src/quaternion.c
	src/utils.c
	src/vec3.c
)

target_include_directories(${TARGETNAME_LIB_MATHLIB}
	PUBLIC include
	PRIVATE src
)

target_link_libraries(${TARGETNAME_LIB_MATHLIB}
	PUBLIC
	${TARGETNAME_LIB_BUILDPLATFORM}
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:m>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_MATHLIB})
