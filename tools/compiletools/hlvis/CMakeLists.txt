add_executable(${TARGETNAME_EXE_NFVIS}
	${COMMON_SOURCE_FILES}
	ambient.cpp
	flow.cpp
	vis.h
	vis.cpp
	zones.h
	zones.cpp
)

target_include_directories(${TARGETNAME_EXE_NFVIS} PRIVATE
	.
	../template
	../common
)

target_compile_definitions(${TARGETNAME_EXE_NFVIS} PRIVATE
	${COMMON_COMPILE_DEFINITIONS}
	HLVIS
)

target_link_libraries(${TARGETNAME_EXE_NFVIS} PRIVATE
	${COMMON_LINK_LIBRARIES}
)

target_compile_options(${TARGETNAME_EXE_NFVIS} PRIVATE
	${COMMON_COMPILE_OPTIONS}
)

install(TARGETS ${TARGETNAME_EXE_NFVIS}
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
