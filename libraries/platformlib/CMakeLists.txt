include(compiler_settings)

if(TARGET_PLATFORM STREQUAL "WINDOWS")
	set(SOURCES
		src/Win32/File.cpp
		src/Win32/Net.cpp
		src/Win32/String.cpp
		src/Win32/System.cpp
		src/Win32/Time.cpp
	)
elseif(TARGET_PLATFORM STREQUAL "LINUX")
	set(SOURCES
		src/Linux/File.cpp
		src/Linux/Net.cpp
		src/Linux/String.cpp
		src/Linux/System.cpp
		src/Linux/Time.cpp
	)
else()
	message(FATAL_ERROR "Unsupported platform: ${TARGET_PLATFORM}")
endif()

add_library(${TARGETNAME_LIB_PLATFORMLIB} STATIC
	include/PlatformLib/File.h
	include/PlatformLib/Net.h
	include/PlatformLib/String.h
	include/PlatformLib/System.h
	include/PlatformLib/Time.h
	${SOURCES}
	src/whereami/whereami.h
	src/whereami/whereami.c
)

target_include_directories(${TARGETNAME_LIB_PLATFORMLIB}
	PUBLIC include
	PRIVATE src
)

target_compile_definitions(${TARGETNAME_LIB_PLATFORMLIB} PRIVATE
	PLATFORMLIB_BUILD_PRODUCER=1
)

target_link_libraries(${TARGETNAME_LIB_PLATFORMLIB} PRIVATE
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:${TARGETNAME_LIB_SAFESTRING}>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_PLATFORMLIB})
