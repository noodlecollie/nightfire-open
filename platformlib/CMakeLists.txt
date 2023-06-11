if(TARGET_PLATFORM STREQUAL "WINDOWS")
	set(SOURCES
		src/Win32/String.cpp
	)
elseif(TARGET_PLATFORM STREQUAL "LINUX")
	message(FATAL_ERROR "Add PlatformLib sources for Linux!")
else()
	message(FATAL_ERROR "Unsupported platform: ${TARGET_PLATFORM}")
endif()

add_library(${TARGETNAME_LIB_PLATFORMLIB} STATIC
	include/PlatformLib/LibExport.h
	include/PlatformLib/String.h
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
