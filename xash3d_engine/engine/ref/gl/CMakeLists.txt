include(compiler_settings)

if(NF_OPENGL_STATIC_LINK)
	if(TARGET_PLATFORM STREQUAL "LINUX")
		find_library(SYSTEM_GL GL REQUIRED)
	else()
		message(FATAL_ERROR "Static OpenGL linking is only supported on Linux")
	endif()
endif()

add_library(${TARGETNAME_LIB_REF_OPENGL} SHARED
	src/gl_alias.c
	src/gl_backend.c
	src/gl_beams.c
	src/gl_context.c
	src/gl_cull.c
	src/gl_dbghulls.c
	src/gl_decals.c
	src/gl_draw.c
	src/gl_export.h
	src/gl_frustum.h
	src/gl_frustum.c
	src/gl_image.c
	src/gl_local.h
	src/gl_opengl.c
	src/gl_rlight.c
	src/gl_rmain.c
	src/gl_rmath.c
	src/gl_rmisc.c
	src/gl_rpart.c
	src/gl_rsurf.c
	src/gl_sprite.c
	src/gl_studio.c
	src/gl_triapi.c
	src/gl_ui.h
	src/gl_ui.c
	src/gl_vgui.c
	src/gl_warp.c
)

target_compile_definitions(${TARGETNAME_LIB_REF_OPENGL} PRIVATE
	REF_DLL
)

set_common_library_compiler_settings(${TARGETNAME_LIB_REF_OPENGL})

target_include_directories(${TARGETNAME_LIB_REF_OPENGL} PRIVATE
	src
)

target_link_libraries(${TARGETNAME_LIB_REF_OPENGL} PRIVATE
	${TARGETNAME_LIB_ENGINEPUBLICAPI}
	${TARGETNAME_LIB_ENGINEINTERNALAPI}
	${TARGETNAME_LIB_CRCLIB}
	${TARGETNAME_LIB_FILESYSTEM_STDIO}
	${TARGETNAME_LIB_MATHLIB}
	${TARGETNAME_LIB_CRTLIB}

	$<$<BOOL:${NF_OPENGL_STATIC_LINK}>:GL>

	# Linux
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:m>
)

target_compile_definitions(${TARGETNAME_LIB_REF_OPENGL} PUBLIC
	$<$<BOOL:${NF_OPENGL_STATIC_LINK}>:XASH_GL_STATIC=1>
)

install(TARGETS ${TARGETNAME_LIB_REF_OPENGL}
	ARCHIVE DESTINATION ${INSTALL_ROOT}
	LIBRARY DESTINATION ${INSTALL_ROOT}
	RUNTIME DESTINATION ${INSTALL_ROOT}
)
