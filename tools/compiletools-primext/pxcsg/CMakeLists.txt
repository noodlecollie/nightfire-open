add_executable(${TARGETNAME_EXE_NFCSG}_new
	app_info.h
	aselib.h
	aselib.cpp
	brush.cpp
	csg.h
	csg4.cpp
	map.cpp
	patch.cpp
	plane.h
	qcsg.cpp
	textures.cpp
	utils.cpp
	vecutils.h
)

target_include_directories(${TARGETNAME_EXE_NFCSG}_new PRIVATE
	.
)

if(NOT "${HOST_COMPILER}" STREQUAL "MSVC")
	target_compile_options(${TARGETNAME_EXE_NFCSG}_new PRIVATE
		-fno-exceptions
	)
endif()

if(ENABLE_MAP_COMPILERS_AVX2)
	if("${HOST_COMPILER}" STREQUAL "MSVC")
		target_compile_options(${TARGETNAME_EXE_NFCSG}_new PRIVATE
			/arch:AVX2
		)
	else()
		message(WARNING "AVX2 support parameters not implemented for this compiler")
	endif()
endif()

target_link_libraries(${TARGETNAME_EXE_NFCSG}_new PRIVATE
	${TARGETNAME_LIB_COMPILETOOLS}
	${TARGETNAME_LIB_MINIUTL}
)

set_common_library_compiler_settings(${TARGETNAME_EXE_NFCSG}_new)

install(TARGETS ${TARGETNAME_EXE_NFCSG}_new
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
