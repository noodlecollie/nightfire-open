include(compiler_settings)

add_library(${TARGETNAME_LIB_REF_SOFT} SHARED
	adivtab.h
	r_aclip.c
	r_beams.c
	r_bsp.c
	r_context.c
	r_decals.c
	r_draw.c
	r_edge.c
	r_glblit.c
	r_image.c
	r_light.c
	r_local.h
	r_main.c
	r_math.c
	r_misc.c
	r_part.c
	r_polyse.c
	r_rast.c
	r_scan.c
	r_sprite.c
	r_studio.c
	r_surf.c
	r_trialias.c
	r_triapi.c
	r_vgui.c
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
	${TARGETNAME_LIB_PUBLIC}

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
