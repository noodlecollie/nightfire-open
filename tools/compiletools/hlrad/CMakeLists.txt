add_executable(${TARGETNAME_EXE_NFRAD}
	${COMMON_SOURCE_FILES}
	compress.h
	compress.cpp
	leaf_lighting.cpp
	lerp.cpp
	lightmap.cpp
	list.h
	loadtextures.cpp
	mathutil.cpp
	meshdesc.h
	meshdesc.cpp
	meshtrace.h
	meshtrace.cpp
	nomatrix.cpp
	progmesh.cpp
	qrad.h
	qrad.cpp
	qradutil.cpp
	radtexture.h
	radtexture.cpp
	sparse.cpp
	studio.cpp
	trace.cpp
	transfers.cpp
	transparency.cpp
	vismatrix.cpp
	vismatrixutil.cpp
)

target_include_directories(${TARGETNAME_EXE_NFRAD} PRIVATE
	.
	../template
	../common
)

target_compile_definitions(${TARGETNAME_EXE_NFRAD} PRIVATE
	${COMMON_COMPILE_DEFINITIONS}
	HLRAD
)

target_link_libraries(${TARGETNAME_EXE_NFRAD} PRIVATE
	${COMMON_LINK_LIBRARIES}
)

target_compile_options(${TARGETNAME_EXE_NFRAD} PRIVATE
	${COMMON_COMPILE_OPTIONS}
)

install(TARGETS ${TARGETNAME_EXE_NFRAD}
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
