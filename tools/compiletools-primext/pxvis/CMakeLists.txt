add_executable(${TARGETNAME_EXE_NFVIS}_new
	app_info.h
	flow.cpp
	qvis.h
	qvis.cpp
	soundpvs.cpp
	winding.cpp
)

target_include_directories(${TARGETNAME_EXE_NFVIS}_new PRIVATE
	.
)

# pxvis compile-time settings
target_compile_definitions(${TARGETNAME_EXE_NFVIS}_new PRIVATE
	HLVIS_SORT_PORTALS=1	# sort portals before flow
	#HLVIS_MERGE_PORTALS=1	# merge visibility between portals (disabled because causes lighting artifacts)
	HLVIS_ENABLE_CHECK_STACK=1 # enables CheckStack() call
)

if(NOT MSVC)
	target_compile_options(${TARGETNAME_EXE_NFVIS}_new PRIVATE -fno-exceptions) # GCC/Clang flag
endif()

if(ENABLE_MAP_COMPILERS_AVX2)
	if(MSVC)
		target_compile_options(${TARGETNAME_EXE_NFVIS}_new PRIVATE /arch:AVX2)
	else()
		message(WARNING "AVX2 support parameters not implemented for this compiler")
	endif()
endif()

target_link_libraries(${TARGETNAME_EXE_NFVIS}_new PRIVATE
	${TARGETNAME_LIB_COMPILETOOLS}
)

install(TARGETS ${TARGETNAME_EXE_NFVIS}_new
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
