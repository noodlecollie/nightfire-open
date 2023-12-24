add_library(${TARGETNAME_LIB_ENGINEINTERNALAPI} INTERFACE
	include/EngineInternalAPI/alias.h
	include/EngineInternalAPI/com_image.h
	include/EngineInternalAPI/cursor_type.h
	include/EngineInternalAPI/dll_types.h
	include/EngineInternalAPI/enginefeatures.h
	include/EngineInternalAPI/gameinfo.h
	include/EngineInternalAPI/key_modifiers.h
	include/EngineInternalAPI/lightstyle.h
	include/EngineInternalAPI/log_strings.h
	include/EngineInternalAPI/mempool.h
	include/EngineInternalAPI/menu_int.h
	include/EngineInternalAPI/mod_local_defs.h
	include/EngineInternalAPI/pm_movevars.h
	include/EngineInternalAPI/protocol.h
	include/EngineInternalAPI/ref_api.h
	include/EngineInternalAPI/render_api.h
	include/EngineInternalAPI/sprite.h
	include/EngineInternalAPI/vgui_api.h
	include/EngineInternalAPI/wadfile.h
)

target_include_directories(${TARGETNAME_LIB_ENGINEINTERNALAPI} INTERFACE
	include
)

target_link_libraries(${TARGETNAME_LIB_ENGINEINTERNALAPI} INTERFACE
	${TARGETNAME_LIB_BUILDPLATFORM}
	${TARGETNAME_LIB_FILESYSTEM_STDIO}
)
