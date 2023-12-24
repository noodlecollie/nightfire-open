include(compiler_settings)

add_library(${TARGETNAME_LIB_COMPILETOOLS} STATIC
	conprint.h
	conprint.cpp
	threads.h
	threads.cpp
	vastring.h
	vastring.cpp
)

target_link_libraries(${TARGETNAME_LIB_COMPILETOOLS} PUBLIC
	${TARGETNAME_LIB_PLATFORMDEFS}
	${TARGETNAME_LIB_PLATFORMLIB}
	${TARGETNAME_LIB_MATHLIB}
	${TARGETNAME_LIB_CRTLIB}
)

set_common_library_compiler_settings(${TARGETNAME_LIB_COMPILETOOLS})
