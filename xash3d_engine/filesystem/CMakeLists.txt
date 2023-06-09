include(compiler_settings)

add_library(${TARGETNAME_LIB_FILESYSTEM_STDIO} SHARED
	dir.c
	filesystem_internal.h
	filesystem.c
	filesystem.h
	fscallback.h
	pak.c
	VFileSystem009.h
	VFileSystem009.cpp
	wad.c
	zip.c
)

target_link_libraries(${TARGETNAME_LIB_FILESYSTEM_STDIO} PUBLIC
	${TARGETNAME_INCLUDES_FILESYSTEM}
	${TARGETNAME_LIB_PUBLIC}
)

set_common_library_compiler_settings(${TARGETNAME_LIB_FILESYSTEM_STDIO})

target_compile_definitions(${TARGETNAME_LIB_FILESYSTEM_STDIO} PRIVATE
	"ALLOCA_H=<${ALLOCA_HEADER}>"
)

target_compile_options(${TARGETNAME_LIB_FILESYSTEM_STDIO} PRIVATE
	$<$<STREQUAL:"${HOST_COMPILER}","MSVC">:/GR->
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-fno-rtti>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-fno-exceptions>
)

if(NF_ENABLE_TESTS)
	add_executable(${TARGETNAME_LIB_FILESYSTEM_TESTS}
		tests/caseinsensitive.c
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
