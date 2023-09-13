include(compiler_settings)

add_library(${TARGETNAME_LIB_REF_SOFT} SHARED
	src/adivtab.h
	src/r_aclip.c
	src/r_beams.c
	src/r_bsp.c
	src/r_context.c
	src/r_decals.c
	src/r_draw.c
	src/r_edge.c
	src/r_glblit.c
	src/r_image.c
	src/r_light.c
	src/r_local.h
	src/r_main.c
	src/r_math.c
	src/r_misc.c
	src/r_part.c
	src/r_polyse.c
	src/r_rast.c
	src/r_scan.c
	src/r_sprite.c
	src/r_studio.c
	src/r_surf.c
	src/r_trialias.c
	src/r_triapi.c
	src/r_vgui.c
)

target_compile_definitions(${TARGETNAME_LIB_REF_SOFT} PRIVATE
	REF_DLL
)

set_common_library_compiler_settings(${TARGETNAME_LIB_REF_SOFT})

target_include_directories(${TARGETNAME_LIB_REF_SOFT} PUBLIC
	.
)

target_link_libraries(${TARGETNAME_LIB_REF_SOFT} PRIVATE
	${TARGETNAME_INCLUDES_ENGINE}
	${TARGETNAME_LIB_CRCLIB}
	${TARGETNAME_LIB_FILESYSTEM_STDIO}
	${TARGETNAME_LIB_MATHLIB}
	${TARGETNAME_LIB_CRTLIB}

	# Linux
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:m>
)

target_compile_definitions(${TARGETNAME_LIB_REF_SOFT} PUBLIC
	XASH_REF_SOFT_ENABLED=1
)

# If this renderer is enabled, add it as an engine dependency
target_link_libraries(${TARGETNAME_LIB_ENGINE} PRIVATE
	${TARGETNAME_LIB_REF_SOFT}
)

install(TARGETS ${TARGETNAME_LIB_REF_SOFT}
	ARCHIVE DESTINATION ${INSTALL_ROOT}
	LIBRARY DESTINATION ${INSTALL_ROOT}
	RUNTIME DESTINATION ${INSTALL_ROOT}
)
