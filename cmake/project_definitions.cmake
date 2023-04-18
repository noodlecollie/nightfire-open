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

if(BUILD_SHARED_LIBS)
	set(LIB_MODE "SHARED")
else()
	set(LIB_MODE "STATIC")
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

############################################################
#                Canonical names and paths                 #
############################################################

set(INSTALL_ROOT nightfire-open)
set(GAME_FOLDER_NAME afterburner)

set(TARGETNAME_INCLUDES_SDK sdk_includes)
set(TARGETNAME_INCLUDES_FILESYSTEM filesystem_includes)
set(TARGETNAME_INCLUDES_ENGINE engine_includes)

set(TARGETNAME_LIB_PUBLIC public)
set(TARGETNAME_LIB_FILESYSTEM_STDIO filesystem_stdio)
set(TARGETNAME_LIB_REF_SOFT ref_soft)
set(TARGETNAME_LIB_REF_OPENGL ref_gl)
set(TARGETNAME_LIB_ENGINE xash)
set(TARGETNAME_LIB_MINIUTL miniutl)
set(TARGETNAME_LIB_CLIENT client)
set(TARGETNAME_LIB_SERVER server)

set(TARGETNAME_EXE_GAME_LAUNCHER bond)
