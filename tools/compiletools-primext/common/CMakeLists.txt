include(compiler_settings)

add_library(${TARGETNAME_LIB_COMPILETOOLS} STATIC
	engine/bspfile.h
	engine/color24.h
	engine/wadfile.h
	basefs.h
	basefs.cpp
	bspfile.h
	cmdlib.h
	cmdlib.cpp
	compatibility_mode.h
	conprint.h
	conprint.cpp
	crashhandler.h
	crashhandler.cpp
	file_system.h
	file_system.cpp
	loglevel.h
	threads.h
	threads.cpp
	vastring.h
	vastring.cpp
	virtualfs.cpp
	wadfileoperations.h
	wadfileoperations.cpp
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
