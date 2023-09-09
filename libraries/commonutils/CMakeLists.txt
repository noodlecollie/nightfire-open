include(compiler_settings)

add_library(${TARGETNAME_LIB_COMMONUTILS} STATIC
	include/CommonUtils/arch.h
	include/CommonUtils/linux_win32_compat.h
	src/arch.c
	src/buildenums.h
)

target_include_directories(${TARGETNAME_LIB_COMMONUTILS}
	PUBLIC include
	PRIVATE src
)

target_link_libraries(${TARGETNAME_LIB_COMMONUTILS}
	PUBLIC
	${TARGETNAME_LIB_PLATFORMLIB}
	${TARGETNAME_LIB_PLATFORMDEFS}

	PRIVATE
	${TARGETNAME_LIB_CRTLIB}
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:m>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_COMMONUTILS})
