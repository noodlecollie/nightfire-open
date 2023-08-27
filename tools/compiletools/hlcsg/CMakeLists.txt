add_executable(${TARGETNAME_EXE_NFCSG}
	${COMMON_SOURCE_FILES}
	ANSItoUTF8.cpp
	brush.cpp
	brushunion.cpp
	csg.h
	hullfile.cpp
	map.cpp
	properties.cpp
	qcsg.cpp
	textures.cpp
)

target_include_directories(${TARGETNAME_EXE_NFCSG} PRIVATE
	.
	../template
	../common
)

target_compile_definitions(${TARGETNAME_EXE_NFCSG} PRIVATE
	${COMMON_COMPILE_DEFINITIONS}
	HLCSG
	DOUBLEVEC_T
)

target_link_libraries(${TARGETNAME_EXE_NFCSG} PRIVATE
	${COMMON_LINK_LIBRARIES}
)

target_compile_options(${TARGETNAME_EXE_NFCSG} PRIVATE
	${COMMON_COMPILE_OPTIONS}
)

install(TARGETS ${TARGETNAME_EXE_NFCSG}
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
