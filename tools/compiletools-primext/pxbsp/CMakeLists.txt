add_executable(${TARGETNAME_EXE_NFBSP}_new
	app_info.h
	bsp5.h
	detail.cpp
	merge.cpp
	outside.cpp
	partition.cpp
	plane.h
	portals.cpp
	solidbsp.cpp
	surfaces.cpp
	tjunc.cpp
)

target_include_directories(${TARGETNAME_EXE_NFBSP}_new PRIVATE
	.
)

if(NOT MSVC)
	target_compile_options(${TARGETNAME_EXE_NFBSP}_new PRIVATE -fno-exceptions)
endif()

if(ENABLE_MAP_COMPILERS_AVX2)
	if(MSVC)
		target_compile_options(${TARGETNAME_EXE_NFBSP}_new PRIVATE
			/arch:AVX2
		)
	else()
		message(WARNING "AVX2 support parameters not implemented for this compiler")
	endif()
endif()

target_link_libraries(${TARGETNAME_EXE_NFBSP}_new PRIVATE
	${TARGETNAME_LIB_COMPILETOOLS}
	# ${TARGETNAME_LIB_MINIUTL}
)
