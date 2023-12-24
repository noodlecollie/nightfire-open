set(GENERATED_HEADER_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated_headers")

configure_file(configure/engine_version.h.in ${GENERATED_HEADER_DIR}/EnginePublicAPI/engine_version.h)

add_library(${TARGETNAME_LIB_ENGINEPUBLICAPI} INTERFACE
	${GENERATED_HEADER_DIR}/EnginePublicAPI/engine_version.h
	include/EnginePublicAPI/beamdef.h
	include/EnginePublicAPI/bspfile.h
	include/EnginePublicAPI/cdll_int.h
	include/EnginePublicAPI/cl_entity.h
	include/EnginePublicAPI/com_model.h
	include/EnginePublicAPI/com_strings.h
	include/EnginePublicAPI/con_nprint.h
	include/EnginePublicAPI/const.h
	include/EnginePublicAPI/custom.h
	include/EnginePublicAPI/customentity.h
	include/EnginePublicAPI/cvardef.h
	include/EnginePublicAPI/demo_api.h
	include/EnginePublicAPI/dlight.h
	include/EnginePublicAPI/edict.h
	include/EnginePublicAPI/eiface.h
	include/EnginePublicAPI/engine_limits.h
	include/EnginePublicAPI/entity_state.h
	include/EnginePublicAPI/entity_types.h
	include/EnginePublicAPI/event_api.h
	include/EnginePublicAPI/event_args.h
	include/EnginePublicAPI/event_flags.h
	include/EnginePublicAPI/hltv.h
	include/EnginePublicAPI/keydefs.h
	include/EnginePublicAPI/link.h
	include/EnginePublicAPI/mobility_int.h
	include/EnginePublicAPI/netadr.h
	include/EnginePublicAPI/particledef.h
	include/EnginePublicAPI/physint.h
	include/EnginePublicAPI/pm_defs.h
	include/EnginePublicAPI/pm_info.h
	include/EnginePublicAPI/pmtrace.h
	include/EnginePublicAPI/progdefs.h
	include/EnginePublicAPI/r_efx.h
	include/EnginePublicAPI/r_studioint.h
	include/EnginePublicAPI/ref_params.h
	include/EnginePublicAPI/screenfade.h
	include/EnginePublicAPI/shake.h
	include/EnginePublicAPI/studio_event.h
	include/EnginePublicAPI/studio.h
	include/EnginePublicAPI/surfaceprops.h
	include/EnginePublicAPI/trace.h
	include/EnginePublicAPI/triangleapi.h
	include/EnginePublicAPI/typedefs.h
	include/EnginePublicAPI/usercmd.h
	include/EnginePublicAPI/weaponinfo.h
	include/EnginePublicAPI/wrect.h
)

target_include_directories(${TARGETNAME_LIB_ENGINEPUBLICAPI} INTERFACE
	${GENERATED_HEADER_DIR}
	include
)

target_link_libraries(${TARGETNAME_LIB_ENGINEPUBLICAPI} INTERFACE
	${TARGETNAME_LIB_BUILDPLATFORM}
	${TARGETNAME_LIB_MATHLIB}
)
