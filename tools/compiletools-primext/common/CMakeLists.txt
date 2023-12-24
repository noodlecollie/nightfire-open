include(compiler_settings)

add_library(${TARGETNAME_LIB_COMPILETOOLS} STATIC
	conprint.h
	conprint.cpp
	crashhandler.h
	crashhandler.cpp
	loglevel.h
	threads.h
	threads.cpp
	vastring.h
	vastring.cpp
	zone.h
	zone.cpp
)

target_link_libraries(${TARGETNAME_LIB_COMPILETOOLS} PUBLIC
	${TARGETNAME_LIB_BUILDPLATFORM}
	${TARGETNAME_LIB_PLATFORMLIB}
	${TARGETNAME_LIB_MATHLIB}
	${TARGETNAME_LIB_CRTLIB}
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:DBGHELP>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_COMPILETOOLS})
