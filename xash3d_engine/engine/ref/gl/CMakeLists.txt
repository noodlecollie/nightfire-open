include(compiler_settings)

if(NF_OPENGL_STATIC_LINK)
	if(TARGET_PLATFORM STREQUAL "LINUX")
		find_library(SYSTEM_GL GL REQUIRED)
	else()
		message(FATAL_ERROR "Static OpenGL linking is only supported on Linux")
	endif()
endif()

add_library(${TARGETNAME_LIB_REF_OPENGL} SHARED
	gl_alias.c
	gl_backend.c
	gl_beams.c
	gl_context.c
	gl_cull.c
	gl_dbghulls.c
	gl_decals.c
	gl_draw.c
	gl_export.h
	gl_frustum.h
	gl_frustum.c
	gl_image.c
	gl_local.h
	gl_opengl.c
	gl_rlight.c
	gl_rmain.c
	gl_rmath.c
	gl_rmisc.c
	gl_rpart.c
	gl_rsurf.c
	gl_sprite.c
	gl_studio.c
	gl_triapi.c
	gl_vgui.c
	gl_warp.c
)

target_compile_definitions(${TARGETNAME_LIB_REF_OPENGL} PRIVATE
	REF_DLL
)

set_common_library_compiler_settings(${TARGETNAME_LIB_REF_OPENGL})

target_include_directories(${TARGETNAME_LIB_REF_OPENGL} PUBLIC
	.
)

target_link_libraries(${TARGETNAME_LIB_REF_OPENGL} PRIVATE
	${TARGETNAME_INCLUDES_ENGINE}
	${TARGETNAME_LIB_COMMONUTILS}
	${TARGETNAME_INCLUDES_SDK}

	$<$<BOOL:${NF_OPENGL_STATIC_LINK}>:GL>

	# Linux
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:m>
)

target_compile_definitions(${TARGETNAME_LIB_REF_OPENGL} PUBLIC
	XASH_REF_GL_ENABLED=1
	$<$<BOOL:${NF_OPENGL_STATIC_LINK}>:XASH_GL_STATIC=1>
)

# If this renderer is enabled, add it as an engine dependency
target_link_libraries(${TARGETNAME_LIB_ENGINE} PRIVATE
	${TARGETNAME_LIB_REF_OPENGL}
)

install(TARGETS ${TARGETNAME_LIB_REF_OPENGL}
	ARCHIVE DESTINATION ${INSTALL_ROOT}
	LIBRARY DESTINATION ${INSTALL_ROOT}
	RUNTIME DESTINATION ${INSTALL_ROOT}
)
