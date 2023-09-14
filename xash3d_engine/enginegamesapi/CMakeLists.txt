# NFTODO: Some of these will only be used by the renderers,
# so could be moved to a private renderer-only interface
add_library(${TARGETNAME_LIB_ENGINEGAMESAPI} INTERFACE
	include/EngineGamesAPI/alias.h
	include/EngineGamesAPI/beamdef.h
	include/EngineGamesAPI/bspfile.h
	include/EngineGamesAPI/cdll_int.h
	include/EngineGamesAPI/cl_entity.h
	include/EngineGamesAPI/com_image.h
	include/EngineGamesAPI/com_model.h
	include/EngineGamesAPI/com_strings.h
	include/EngineGamesAPI/con_nprint.h
	include/EngineGamesAPI/const.h
	include/EngineGamesAPI/cursor_type.h
	include/EngineGamesAPI/custom.h
	include/EngineGamesAPI/customentity.h
	include/EngineGamesAPI/cvardef.h
	include/EngineGamesAPI/demo_api.h
	include/EngineGamesAPI/dlight.h
	include/EngineGamesAPI/dll_types.h
	include/EngineGamesAPI/edict.h
	include/EngineGamesAPI/eiface.h
	include/EngineGamesAPI/engine_limits.h
	include/EngineGamesAPI/engine_version.h
	include/EngineGamesAPI/enginefeatures.h
	include/EngineGamesAPI/entity_state.h
	include/EngineGamesAPI/entity_types.h
	include/EngineGamesAPI/event_api.h
	include/EngineGamesAPI/event_args.h
	include/EngineGamesAPI/event_flags.h
	include/EngineGamesAPI/gameinfo.h
	include/EngineGamesAPI/hltv.h
	include/EngineGamesAPI/key_modifiers.h
	include/EngineGamesAPI/keydefs.h
	include/EngineGamesAPI/lightstyle.h
	include/EngineGamesAPI/link.h
	include/EngineGamesAPI/log_strings.h
	include/EngineGamesAPI/mempool.h
	include/EngineGamesAPI/menu_int.h
	include/EngineGamesAPI/mobility_int.h
	include/EngineGamesAPI/mod_local_defs.h
	include/EngineGamesAPI/netadr.h
	include/EngineGamesAPI/particledef.h
	include/EngineGamesAPI/physint.h
	include/EngineGamesAPI/pm_defs.h
	include/EngineGamesAPI/pm_info.h
	include/EngineGamesAPI/pm_movevars.h
	include/EngineGamesAPI/pmtrace.h
	include/EngineGamesAPI/progdefs.h
	include/EngineGamesAPI/protocol.h
	include/EngineGamesAPI/r_efx.h
	include/EngineGamesAPI/r_studioint.h
	include/EngineGamesAPI/ref_api.h
	include/EngineGamesAPI/ref_params.h
	include/EngineGamesAPI/render_api.h
	include/EngineGamesAPI/screenfade.h
	include/EngineGamesAPI/shake.h
	include/EngineGamesAPI/sprite.h
	include/EngineGamesAPI/studio_event.h
	include/EngineGamesAPI/studio.h
	include/EngineGamesAPI/surfaceprops.h
	include/EngineGamesAPI/trace.h
	include/EngineGamesAPI/triangleapi.h
	include/EngineGamesAPI/typedefs.h
	include/EngineGamesAPI/usercmd.h
	include/EngineGamesAPI/vgui_api.h
	include/EngineGamesAPI/wadfile.h
	include/EngineGamesAPI/weaponinfo.h
	include/EngineGamesAPI/wrect.h
)

target_compile_definitions(${TARGETNAME_LIB_ENGINEGAMESAPI} INTERFACE
	BUILDVAR_VERSION_STRING="${VCS_TAG_ID}"
)

target_include_directories(${TARGETNAME_LIB_ENGINEGAMESAPI} INTERFACE
	include
)

target_link_libraries(${TARGETNAME_LIB_ENGINEGAMESAPI} INTERFACE
	${TARGETNAME_LIB_PLATFORMDEFS}
)
