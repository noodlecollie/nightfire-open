include(compiler_settings)

if(TARGET_PLATFORM STREQUAL "WINDOWS")
	set(SOURCES
		src/Win32/File.cpp
		src/Win32/Net.cpp
		src/Win32/String.cpp
		src/Win32/System.cpp
	)
elseif(TARGET_PLATFORM STREQUAL "LINUX")
	set(SOURCES
		src/Linux/File.cpp
		src/Linux/Net.cpp
		src/Linux/String.cpp
		src/Linux/System.cpp
	)
else()
	message(FATAL_ERROR "Unsupported platform: ${TARGET_PLATFORM}")
endif()

add_library(${TARGETNAME_LIB_PLATFORMLIB} STATIC
	include/PlatformLib/File.h
	include/PlatformLib/Net.h
	include/PlatformLib/String.h
	include/PlatformLib/System.h
	${SOURCES}
)

target_include_directories(${TARGETNAME_LIB_PLATFORMLIB}
	PUBLIC include
	PRIVATE src
)

target_compile_definitions(${TARGETNAME_LIB_PLATFORMLIB} PRIVATE
	PLATFORMLIB_BUILD_PRODUCER=1
	$<$<BOOL:${TARGET_IS_64BIT}>:IS_64BIT=1>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_PLATFORMLIB})
