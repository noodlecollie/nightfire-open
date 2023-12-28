set(COMMON_COMPILE_DEFINITIONS
	VERSION_64BIT

	# Windows
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:WIN32>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:_CONSOLE>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:SYSTEM_WIN32>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:STDC_HEADERS>

	# Linux
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:STDC_HEADERS>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:SYSTEM_POSIX>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:HAVE_SYS_TIME_H>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:HAVE_UNISTD_H>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:HAVE_SYS_STAT_H>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:HAVE_FCNTL_H>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:HAVE_SYS_RESOURCE_H>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:_strdup=strdup>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:_strlwr=strlwr>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:_strupr=strupr>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:stricmp=strcasecmp>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:_unlink=unlink>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:_open=open>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:_read=read>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:_close=close>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:VERSION_LINUX>
)

set(COMMON_LINK_LIBRARIES
	${TARGETNAME_LIB_ENGINEPUBLICAPI}

	# Linux
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:pthread>
)

# This code will be replaced with the better compile tools SDK later,
# so no point spending loads of time fixing these warnings now.
set(COMMON_COMPILE_OPTIONS
	# MSVC
	$<$<STREQUAL:"${HOST_COMPILER}","MSVC">:/wd4311>
	$<$<STREQUAL:"${HOST_COMPILER}","MSVC">:/wd4312>

	# Not MSVC
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wint-to-pointer-cast>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Ofast>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-funsafe-math-optimizations>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-funsafe-loop-optimizations>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-ffast-math>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-fgraphite-identity>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-march=native>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-mtune=native>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-msse4>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-mavx>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-floop-interchange>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-mfpmath=sse>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-fno-strict-aliasing>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-deprecated-declarations>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-unused-parameter>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-unused-variable>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-unused-value>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-sign-compare>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-unused-but-set-variable>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-maybe-uninitialized>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-string-compare>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-parentheses>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-dangling-else>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-unused-result>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-unused-function>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-uninitialized>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-type-limits>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-unknown-pragmas>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-misleading-indentation>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-format-overflow>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-address>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-stringop-truncation>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-array-parameter>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-switch>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-class-memaccess>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-deprecated-copy>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-missing-braces>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-implicit-fallthrough>
)

set(COMMON_SOURCE_FILES
	../common/anorms.h
	../common/blockmem.cpp
	../common/blockmem.h
	../common/boundingbox.h
	../common/bspfile.cpp
	../common/bspfile.h
	../common/checksum.h
	../common/cmdlib.cpp
	../common/cmdlib.h
	../common/cmdlinecfg.cpp
	../common/cmdlinecfg.h
	../common/cppmemory.cpp
	../common/cppmemory.h
	../common/filelib.cpp
	../common/filelib.h
	../common/files.cpp
	../common/filesystem.cpp
	../common/hlassert.h
	../common/log.cpp
	../common/log.h
	../common/mathlib.cpp
	../common/mathlib.h
	../common/mathtypes.h
	../common/messages.cpp
	../common/messages.h
	../common/miptexwrapper.cpp
	../common/miptexwrapper.h
	../common/nfopenheader.cpp
	../common/nfopenheader.h
	../common/pngtexturepath.cpp
	../common/pngtexturepath.h
	../common/resourcelock.cpp
	../common/resourcelock.h
	../common/scriplib.cpp
	../common/scriplib.h
	../common/specialtextures.h
	../common/specialtextures_halflife.h
	../common/specialtextures_nfopen.h
	../common/stb_image.cpp
	../common/stb_image.h
	../common/stringlib.cpp
	../common/stringlib.h
	../common/studio.h
	../common/texturecollection.cpp
	../common/texturecollection.h
	../common/texturecollectionreader.cpp
	../common/texturecollectionreader.h
	../common/texturecollectionwriter.cpp
	../common/texturecollectionwriter.h
	../common/texturedirectorylisting.cpp
	../common/texturedirectorylisting.h
	../common/threads.cpp
	../common/threads.h
	../common/TimeCounter.h
	../common/win32fix.h
	../common/winding.cpp
	../common/winding.h
	../template/BaseMath.h
	../template/basictypes.h
	../template/EndianMath.h
	../template/ReferenceArray.h
	../template/ReferenceCounter.h
	../template/ReferenceObject.h
	../template/ReferencePtr.h
)

add_subdirectory(hlbsp)
add_subdirectory(hlcsg)
add_subdirectory(hlrad)
add_subdirectory(hlvis)
add_subdirectory(studiomdl)
