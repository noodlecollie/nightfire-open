add_library(${TARGETNAME_LIB_ENGINEREFAPI} INTERFACE
	include/EngineRefAPI/alias.h
	include/EngineRefAPI/com_image.h
	include/EngineRefAPI/cursor_type.h
	include/EngineRefAPI/dll_types.h
	include/EngineRefAPI/enginefeatures.h
	include/EngineRefAPI/gameinfo.h
	include/EngineRefAPI/key_modifiers.h
	include/EngineRefAPI/lightstyle.h
	include/EngineRefAPI/log_strings.h
	include/EngineRefAPI/mempool.h
	include/EngineRefAPI/ref_api.h
	include/EngineRefAPI/vgui_api.h
)

target_include_directories(${TARGETNAME_LIB_ENGINEREFAPI} INTERFACE
	include
)

target_link_libraries(${TARGETNAME_LIB_ENGINEREFAPI} INTERFACE
	${TARGETNAME_LIB_PLATFORMDEFS}
)
