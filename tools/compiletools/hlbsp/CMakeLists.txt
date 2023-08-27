add_executable(${TARGETNAME_EXE_NFBSP}
	${COMMON_SOURCE_FILES}
	brink.cpp
	bsp5.h
	merge.cpp
	outside.cpp
	portals.cpp
	qbsp.cpp
	solidbsp.cpp
	surfaces.cpp
	tjunc.cpp
	writebsp.cpp
)

target_include_directories(${TARGETNAME_EXE_NFBSP} PRIVATE
	.
	../template
	../common
)

target_compile_definitions(${TARGETNAME_EXE_NFBSP} PRIVATE
	${COMMON_COMPILE_DEFINITIONS}
	HLBSP
	DOUBLEVEC_T
)

target_link_libraries(${TARGETNAME_EXE_NFBSP} PRIVATE
	${COMMON_LINK_LIBRARIES}
)

target_compile_options(${TARGETNAME_EXE_NFBSP} PRIVATE
	${COMMON_COMPILE_OPTIONS}
)

install(TARGETS ${TARGETNAME_EXE_NFBSP}
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
