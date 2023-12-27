add_executable(${TARGETNAME_EXE_NFBSP}_new
	app_info.h
	bsp5.h
	detail.cpp
	merge.cpp
	outside.cpp
	partition.cpp
	plane.h
	portals.cpp
	qbsp.cpp
	solidbsp.cpp
	surfaces.cpp
	tjunc.cpp
	tree.cpp
	writebsp.cpp
)

target_include_directories(${TARGETNAME_EXE_NFBSP}_new PRIVATE
	.
)

if(NOT "${HOST_COMPILER}" STREQUAL "MSVC")
	target_compile_options(${TARGETNAME_EXE_NFBSP}_new PRIVATE
		-fno-exceptions
	)
endif()

if(ENABLE_MAP_COMPILERS_AVX2)
	if("${HOST_COMPILER}" STREQUAL "MSVC")
		target_compile_options(${TARGETNAME_EXE_NFBSP}_new PRIVATE
			/arch:AVX2
		)
	else()
		message(WARNING "AVX2 support parameters not implemented for this compiler")
	endif()
endif()

target_link_libraries(${TARGETNAME_EXE_NFBSP}_new PRIVATE
	${TARGETNAME_LIB_COMPILETOOLS}
)

set_common_library_compiler_settings(${TARGETNAME_EXE_NFBSP}_new)

install(TARGETS ${TARGETNAME_EXE_NFBSP}_new
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
