add_library(${TARGETNAME_LIB_ENGINEDEFS} INTERFACE
	include/XashDefs/beamdef.h
	include/XashDefs/bspfile.h
	include/XashDefs/cl_entity.h
	include/XashDefs/com_image.h
	include/XashDefs/com_model.h
	include/XashDefs/const.h
	include/XashDefs/cvardef.h
	include/XashDefs/dlight.h
	include/XashDefs/engine_limits.h
	include/XashDefs/engine_version.h
	include/XashDefs/enginefeatures.h
	include/XashDefs/entity_state.h
	include/XashDefs/entity_types.h
	include/XashDefs/event_args.h
	include/XashDefs/event_flags.h
	include/XashDefs/hltv.h
	include/XashDefs/ivoicetweak.h
	include/XashDefs/kbutton.h
	include/XashDefs/lightstyle.h
	include/XashDefs/log_strings.h
	include/XashDefs/mempool.h
	include/XashDefs/net_api.h
	include/XashDefs/netadr.h
	include/XashDefs/particledef.h
	include/XashDefs/pm_info.h
	include/XashDefs/pmtrace.h
	include/XashDefs/qfont.h
	include/XashDefs/r_efx.h
	include/XashDefs/r_studioint.h
	include/XashDefs/ref_params.h
	include/XashDefs/render_api.h
	include/XashDefs/screenfade.h
	include/XashDefs/studio_event.h
	include/XashDefs/surfaceprops.h
	include/XashDefs/trace.h
	include/XashDefs/triangleapi.h
	include/XashDefs/typedefs.h
	include/XashDefs/usercmd.h
	include/XashDefs/wadfile.h
	include/XashDefs/weaponinfo.h
	include/XashDefs/wrect.h
)

target_compile_definitions(${TARGETNAME_LIB_ENGINEDEFS} INTERFACE
	BUILDVAR_VERSION_STRING="${VCS_TAG_ID}"
)

target_include_directories(${TARGETNAME_LIB_ENGINEDEFS} INTERFACE
	include
)

target_link_libraries(${TARGETNAME_LIB_ENGINEDEFS} INTERFACE
	${TARGETNAME_LIB_PLATFORMDEFS}
)
