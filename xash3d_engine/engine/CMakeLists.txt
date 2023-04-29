# NFTODO: This is currently set up using the following assumptions
# while we don't have all the options migrated yet:
# - C linkage (no is_cxx_link)
# - No ENGINE_TESTS
# - Only Windows or Linux supported
# - No XASH_CUSTOM_SWAP
# - No FBDEV_SW

include(compiler_settings)

if(NF_DEDICATED_SERVER)
	set(CLIENT_SUPPORT NO)
else()
	set(CLIENT_SUPPORT YES)
endif()

# NFTODO: Predicate on USE_SELECT option as well
if(NF_DEDICATED_SERVER AND NOT TARGET_PLATFORM STREQUAL "WINDOWS")
	set(ENABLE_USE_SELECT YES)
else()
	set(ENABLE_USE_SELECT NO)
endif()

if(CLIENT_SUPPORT)
	find_package(SDL2 REQUIRED)
endif()

set(ENGINE_SOURCES
	common/imagelib/imagelib.h
	common/imagelib/img_bmp.h
	common/imagelib/img_bmp.c
	common/imagelib/img_dds.h
	common/imagelib/img_dds.c
	common/imagelib/img_main.c
	common/imagelib/img_png.h
	common/imagelib/img_png.c
	common/imagelib/img_quant.c
	common/imagelib/img_tga.h
	common/imagelib/img_tga.c
	common/imagelib/img_utils.c
	common/imagelib/img_wad.c
	common/soundlib/libmpg/dct36.c
	common/soundlib/libmpg/dct64.c
	common/soundlib/libmpg/fmt123.h
	common/soundlib/libmpg/format.c
	common/soundlib/libmpg/frame.h
	common/soundlib/libmpg/frame.c
	common/soundlib/libmpg/getbits.h
	common/soundlib/libmpg/huffman.h
	common/soundlib/libmpg/index.h
	common/soundlib/libmpg/index.c
	common/soundlib/libmpg/layer3.c
	common/soundlib/libmpg/libmpg.h
	common/soundlib/libmpg/libmpg.c
	common/soundlib/libmpg/mpeghead.h
	common/soundlib/libmpg/mpg123.h
	common/soundlib/libmpg/mpg123.c
	common/soundlib/libmpg/parse.c
	common/soundlib/libmpg/reader.h
	common/soundlib/libmpg/reader.c
	common/soundlib/libmpg/sample.h
	common/soundlib/libmpg/synth.h
	common/soundlib/libmpg/synth.c
	common/soundlib/libmpg/tabinit.c
	common/soundlib/snd_main.c
	common/soundlib/snd_mp3.c
	common/soundlib/snd_utils.c
	common/soundlib/snd_wav.c
	common/soundlib/soundlib.h
	common/bsp/generic/viscompress.h
	common/bsp/generic/viscompress.c
	common/base_cmd.h
	common/base_cmd.c
	common/cfgscript.c
	common/cmd.c
	common/common.h
	common/common.c
	common/com_strings.h
	common/con_utils.c
	common/crashhandler.c
	common/custom.c
	common/cvar.h
	common/cvar.c
	common/dedicated.c
	common/filesystem_engine.c
	common/gamma.c
	common/host.c
	common/host_state.c
	common/hpak.h
	common/hpak.c
	common/identification.c
	common/infostring.c
	common/ipv6text.h
	common/ipv6text.c
	common/launcher.c
	common/lib_common.c
	common/library.h
	common/masterlist.c
	common/mod_bmodel.c
	common/model.c
	common/model_dump.h
	common/model_dump.c
	common/mod_local.h
	common/mod_sprite.c
	common/mod_studio.c
	common/net_buffer.h
	common/net_buffer.c
	common/net_chan.c
	common/netchan.h
	common/net_encode.h
	common/net_encode.c
	common/net_ws.h
	common/net_ws.c
	common/pm_debug.c
	common/pm_local.h
	common/pm_surface.c
	common/pm_trace.c
	common/protocol.h
	common/sequence.c
	common/sys_con.c
	common/system.c
	common/system.h
	common/tests.h
	common/textureproperties.h
	common/textureproperties.c
	common/whereami.h
	common/whereami.c
	common/world.h
	common/world.c
	common/zone.c
	server/server.h
	server/sv_client.c
	server/sv_cmds.c
	server/sv_custom.c
	server/sv_filter.c
	server/sv_frame.c
	server/sv_game.c
	server/sv_init.c
	server/sv_log.c
	server/sv_main.c
	server/sv_move.c
	server/sv_phys.c
	server/sv_pmove.c
	server/sv_save.c
	server/sv_world.c
)

if(CLIENT_SUPPORT)
	list(APPEND ENGINE_SOURCES
		client/avi/avi_stub.c
		client/avi/avi_win.c
		client/avi/avi.h
		client/vgui/vgui_draw.h
		client/vgui/vgui_draw.c
		client/cl_cmds.c
		client/cl_custom.c
		client/cl_debug.c
		client/cl_demo.c
		client/cl_efrag.c
		client/cl_efx.c
		client/cl_events.c
		client/cl_font.c
		client/cl_frame.c
		client/cl_game.c
		client/cl_gameui.c
		client/client.h
		client/cl_main.c
		client/cl_mobile.c
		client/cl_netgraph.c
		client/cl_parse.c
		client/cl_pmove.c
		client/cl_qparse.c
		client/cl_remap.c
		client/cl_render.c
		client/cl_scrn.c
		client/cl_securedstub.c
		client/cl_surfaceprop_effects.h
		client/cl_surfaceprop_effects.c
		client/cl_tent.h
		client/cl_tent.c
		client/cl_video.c
		client/cl_view.c
		client/console.c
		client/in_joy.c
		client/input.h
		client/input.c
		client/in_touch.c
		client/keys.c
		client/mod_dbghulls.c
		client/ref_common.h
		client/ref_common.c
		client/s_dsp.c
		client/s_load.c
		client/s_main.c
		client/s_mix.c
		client/s_mouth.c
		client/sound.h
		client/s_stream.c
		client/s_utils.c
		client/s_vox.c
		client/titles.c
		client/vid_common.h
		client/vid_common.c
		client/voice.h
		client/voice.c
		client/vox.h
	)
endif()

if(TARGET_PLATFORM STREQUAL "WINDOWS")
	list(APPEND ENGINE_SOURCES
		platform/win32/con_win.c
		platform/win32/lib_custom_win.c
		platform/win32/lib_win.h
		platform/win32/lib_win.c
		platform/win32/net.h
		platform/win32/sys_win.c
	)
else()
	list(APPEND ENGINE_SOURCES
		platform/posix/lib_posix.c
		platform/posix/net.h
		platform/posix/sys_posix.c
		platform/linux/in_evdev.c
		platform/linux/s_alsa.c
		platform/linux/sys_linux.c
		platform/linux/vid_fbdev.c
	)
endif()

if(SDL2_FOUND)
	list(APPEND ENGINE_SOURCES
		platform/sdl/events.h
		platform/sdl/events.c
		platform/sdl/in_sdl.c
		platform/sdl/s_sdl.c
		platform/sdl/sys_sdl.c
		platform/sdl/vid_sdl.c
	)
endif()

if(NF_SINGLE_ENGINE_BINARY)
	add_executable(${TARGETNAME_LIB_ENGINE}
		${ENGINE_SOURCES}
	)

	set_target_properties(${TARGETNAME_LIB_ENGINE} PROPERTIES OUTPUT_NAME ${GAME_EXECUTABLE})

	target_link_options(${TARGETNAME_LIB_ENGINE} PRIVATE
		# Use WinMain() instead of main()
		$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:/ENTRY:WinMainCRTStartup>
		$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:/SUBSYSTEM:WINDOWS>
	)
else()
	add_library(${TARGETNAME_LIB_ENGINE}
		${ENGINE_SOURCES}
	)
endif()

target_link_libraries(${TARGETNAME_LIB_ENGINE} PUBLIC
	${TARGETNAME_INCLUDES_ENGINE}
	${TARGETNAME_LIB_FILESYSTEM_STDIO}
)

target_link_libraries(${TARGETNAME_LIB_ENGINE} PRIVATE
	$<$<BOOL:${CLIENT_SUPPORT}>:SDL2::SDL2>
	$<$<BOOL:${CLIENT_SUPPORT}>:Opus::opus>
	${TARGETNAME_LIB_PUBLIC}
)

target_compile_definitions(${TARGETNAME_LIB_ENGINE} PRIVATE
	$<$<BOOL:${NF_DEDICATED_SERVER}>:XASH_DEDICATED=1>
	#XASH_ENGINE_TESTS=0 # NFTODO: Predicate on tests build option (not present if disabled)
	#XASH_CUSTOM_SWAP=0 # Don't need to support this
	$<$<BOOL:${NF_SINGLE_ENGINE_BINARY}>:SINGLE_BINARY=1>
	$<$<BOOL:${NF_SINGLE_ENGINE_BINARY}>:XASH_GAMEDIR="afterburner">
	$<$<BOOL:${ENABLE_USE_SELECT}>:XASH_USE_SELECT=1>
	$<$<BOOL:${NF_LOW_MEMORY_SUPPORT}>:XASH_LOW_MEMORY=1>
	#SUPPORT_BSP2_FORMAT=0 # NFTODO: Don't need to support this, but if it's defined then BSP loading screws up as struct layout is different! Needs to be fixed
	$<$<BOOL:${TARGET_IS_64BIT}>:XASH_64BIT=1>
	AFTERBURNER_ENGINE=1 # NFTODO: Rename this to NIGHTFIRE_OPEN_ENGINE
	DBGHELP=$<BOOL:${WIN32}>
	PSAPI_VERSION=$<BOOL:${WIN32}>
	$<$<BOOL:${SDL2_FOUND}>:XASH_SDL=2>
	"ALLOCA_H=<${ALLOCA_HEADER}>"
)

target_link_libraries(${TARGETNAME_LIB_ENGINE} PRIVATE
	# Windows
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:USER32>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:SHELL32>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:GDI32>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:ADVAPI32>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:DBGHELP>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:PSAPI>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:WS2_32>

	# Linux
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:m>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:rt>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:pthread>
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:asound>

	$<$<AND:$<STREQUAL:"${TARGET_PLATFORM}","LINUX">,$<STREQUAL:"${LIB_MODE}","SHARED">>:dl>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_ENGINE})

target_include_directories(${TARGETNAME_LIB_ENGINE} PRIVATE
	client
	client/vgui
	server
)

# NFTODO: Enable tests

if(CLIENT_SUPPORT)
	add_subdirectory(ref)
endif()

install(TARGETS ${TARGETNAME_LIB_ENGINE}
	ARCHIVE DESTINATION ${INSTALL_ROOT}
	LIBRARY DESTINATION ${INSTALL_ROOT}
	RUNTIME DESTINATION ${INSTALL_ROOT}
)

if(TARGET_PLATFORM STREQUAL "WINDOWS" AND CLIENT_SUPPORT)
	add_custom_command(
		TARGET ${TARGETNAME_LIB_ENGINE} POST_BUILD
		MAIN_DEPENDENCY SDL2::SDL2
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"$<TARGET_FILE:SDL2::SDL2>"
		"$<TARGET_FILE_DIR:${TARGETNAME_LIB_ENGINE}>"
	)

	install(IMPORTED_RUNTIME_ARTIFACTS SDL2::SDL2
		RUNTIME DESTINATION ${INSTALL_ROOT}
		LIBRARY DESTINATION ${INSTALL_ROOT}
	)
endif()
