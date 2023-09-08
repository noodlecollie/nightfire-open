include(compiler_settings)

add_library(${TARGETNAME_LIB_FILESYSTEM_STDIO} SHARED
	include/Filesystem/filesystem.h
	include/Filesystem/fscallback.h
	src/dir.c
	src/filesystem_internal.h
	src/filesystem.c
	src/pak.c
	src/wad.c
	src/zip.c
)

target_include_directories(${TARGETNAME_LIB_FILESYSTEM_STDIO}
	PUBLIC include
	PRIVATE src
)

target_link_libraries(${TARGETNAME_LIB_FILESYSTEM_STDIO} PUBLIC
	${TARGETNAME_INCLUDES_FILESYSTEM}
	${TARGETNAME_INCLUDES_SDK}
	${TARGETNAME_LIB_COMMONUTILS}
	${TARGETNAME_LIB_PLATFORMDEFS}
	${TARGETNAME_LIB_CRCLIB}
	${TARGETNAME_LIB_ENGINEDEFS}
)

set_common_library_compiler_settings(${TARGETNAME_LIB_FILESYSTEM_STDIO})

target_compile_definitions(${TARGETNAME_LIB_FILESYSTEM_STDIO} PRIVATE
	"ALLOCA_H=<${ALLOCA_HEADER}>"
)

target_compile_options(${TARGETNAME_LIB_FILESYSTEM_STDIO} PRIVATE
	# MSVC
	$<$<STREQUAL:"${HOST_COMPILER}","MSVC">:/GR->

	# Not MSVC
	$<$<AND:$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>,$<COMPILE_LANGUAGE:CXX>>:-fno-rtti>
	$<$<AND:$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>,$<COMPILE_LANGUAGE:CXX>>:-fno-exceptions>
)

if(NF_ENABLE_TESTS)
	add_executable(${TARGETNAME_LIB_FILESYSTEM_TESTS}
		src/tests/caseinsensitive.c
	)

	target_link_libraries(${TARGETNAME_LIB_FILESYSTEM_TESTS} PRIVATE
		${TARGETNAME_LIB_FILESYSTEM_STDIO}
	)
endif()

install(TARGETS ${TARGETNAME_LIB_FILESYSTEM_STDIO}
	ARCHIVE DESTINATION ${INSTALL_ROOT}
	LIBRARY DESTINATION ${INSTALL_ROOT}
	RUNTIME DESTINATION ${INSTALL_ROOT}
)
