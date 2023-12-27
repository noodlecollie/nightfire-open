include(compiler_settings)

add_library(${TARGETNAME_LIB_COMPILETOOLS} STATIC
	include/CompileTools/engine/bspfile.h
	include/CompileTools/engine/color24.h
	include/CompileTools/engine/wadfile.h
	include/CompileTools/basefs.h
	include/CompileTools/bspfile.h
	include/CompileTools/cmdlib.h
	include/CompileTools/compatibility_mode.h
	include/CompileTools/conprint.h
	include/CompileTools/crashhandler.h
	include/CompileTools/file_system.h
	include/CompileTools/loglevel.h
	include/CompileTools/polylib.h
	include/CompileTools/scriptlib.h
	include/CompileTools/shaders.h
	include/CompileTools/tga.h
	include/CompileTools/threads.h
	include/CompileTools/vastring.h
	include/CompileTools/wadfileoperations.h
	include/CompileTools/zone.h
	src/basefs.cpp
	src/bspfile.cpp
	src/cmdlib.cpp
	src/conprint.cpp
	src/crashhandler.cpp
	src/file_system.cpp
	src/polylib.cpp
	src/scriptlib.cpp
	src/shaders.cpp
	src/threads.cpp
	src/vastring.cpp
	src/virtualfs.cpp
	src/wadfileoperations.cpp
	src/zone.cpp
)

target_include_directories(${TARGETNAME_LIB_COMPILETOOLS}
	PUBLIC
	include

	PRIVATE
	src
)

target_link_libraries(${TARGETNAME_LIB_COMPILETOOLS} PUBLIC
	${TARGETNAME_LIB_BUILDPLATFORM}
	${TARGETNAME_LIB_PLATFORMLIB}
	${TARGETNAME_LIB_MATHLIB}
	${TARGETNAME_LIB_CRTLIB}

	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:DBGHELP>

	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:pthread>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:dl>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_COMPILETOOLS})
