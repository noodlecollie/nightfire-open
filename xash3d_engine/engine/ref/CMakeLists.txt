if(NF_SOFTWARE_RENDERER_ENABLED)
	add_subdirectory(soft)
endif()

if(NF_OPENGL_RENDERER_ENABLED)
	add_subdirectory(gl)
endif()
