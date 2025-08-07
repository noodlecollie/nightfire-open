############################################################
#  Convenience variables for use in generator expressions  #
############################################################

# Assigning known strings makes using generator expressions
# a bit easier, particularly for multi-valued variables,
# but also to avoid having to stack $<$<NOT:$<BOOL:${VAR}>>:...>
# if you want to invert a variable.
# Eg. if you want to set an option only for Windows, use:
#   $<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:my_windows_option>

if(WIN32)
	set(TARGET_PLATFORM "WINDOWS")
else()
	set(TARGET_PLATFORM "LINUX")
endif()

if(CMAKE_HOST_WIN32)
	set(HOST_PLATFORM "WINDOWS")
else()
	set(HOST_PLATFORM "LINUX")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(HOST_COMPILER "CLANG")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(HOST_COMPILER "GCC")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(HOST_COMPILER "MSVC")
else()
	message(WARNING "Unsupported compiler - configuring/building project may not succeed")
	set(HOST_COMPILER "UNKNOWN")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(TARGET_IS_64BIT YES)
else()
	set(TARGET_IS_64BIT NO)
endif()

############################################################
#                Canonical names and paths                 #
############################################################

set(INSTALL_ROOT nightfire-open)
set(TOOLS_INSTALL_ROOT ${INSTALL_ROOT}/tools)
set(GAME_FOLDER_NAME nfopen)
set(GAME_EXECUTABLE bond)

set(TARGETNAME_LIB_BUILDPLATFORM buildplatform)
set(TARGETNAME_LIB_FILESYSTEM_STDIO filesystem_stdio)
set(TARGETNAME_LIB_FILESYSTEM_TESTS filesystem_test_caseinsensitive)
set(TARGETNAME_LIB_REF_SOFT ref_soft)
set(TARGETNAME_LIB_REF_OPENGL ref_gl)
set(TARGETNAME_LIB_REF_OPENGL_EXPERIMENTAL ref_gl_experimental)
set(TARGETNAME_LIB_ENGINE engine)
set(TARGETNAME_LIB_ENGINEPUBLICAPI engine_publicapi)
set(TARGETNAME_LIB_ENGINEINTERNALAPI engine_internalapi)
set(TARGETNAME_LIB_MINIUTL miniutl)
set(TARGETNAME_LIB_CLIENT client)
set(TARGETNAME_LIB_SERVER server)
set(TARGETNAME_LIB_UI menu)
set(TARGETNAME_LIB_PLATFORMLIB platformlib)
set(TARGETNAME_LIB_CRCLIB crclib)
set(TARGETNAME_LIB_CRTLIB crtlib)
set(TARGETNAME_LIB_MINIZ miniz)
set(TARGETNAME_LIB_MATHLIB mathlib)
set(TARGETNAME_LIB_ARGS argparse)
set(TARGETNAME_LIB_CPPFS cppfs)
set(TARGETNAME_LIB_MEMPOOL mempoollib)
set(TARGETNAME_LIB_COMPILETOOLS compiletoolslib)

# Only supported on Linux
set(TARGETNAME_LIB_SAFESTRING safestring_static)

set(TARGETNAME_EXE_GAME_LAUNCHER game_launch)
set(TARGETNAME_EXE_NFCSG nfcsg)
set(TARGETNAME_EXE_NFBSP nfbsp)
set(TARGETNAME_EXE_NFVIS nfvis)
set(TARGETNAME_EXE_NFRAD nfrad)
set(TARGETNAME_EXE_STUDIOMDL studiomdl)
set(TARGETNAME_EXE_MDL14_DECOMPILER mdlv14decompiler)
