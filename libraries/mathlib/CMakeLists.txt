add_library(${TARGETNAME_LIB_MATHLIB} STATIC
	include/MathLib/mathlib.h
	src/mathlib.c
)

target_include_directories(${TARGETNAME_LIB_MATHLIB}
	PUBLIC include
	PRIVATE src
)

target_link_libraries(${TARGETNAME_LIB_MATHLIB} PUBLIC
	${TARGETNAME_LIB_PLATFORMDEFS}
)
