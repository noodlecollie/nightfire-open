include(compiler_settings)
include(nf_utils)

if(NF_DEDICATED_SERVER)
	set(CLIENT_SUPPORT NO)
else()
	set(CLIENT_SUPPORT YES)
endif()

if(CLIENT_SUPPORT)
	find_package(SDL2 REQUIRED)
endif()

set(ENGINE_SOURCES
	src/common/imagelib/imagelib.h
	src/common/imagelib/img_bmp.h
	src/common/imagelib/img_bmp.c
	src/common/imagelib/img_dds.h
	src/common/imagelib/img_dds.c
	src/common/imagelib/img_main.c
	src/common/imagelib/img_png.h
	src/common/imagelib/img_png.c
	src/common/imagelib/img_quant.c
	src/common/imagelib/img_tga.h
	src/common/imagelib/img_tga.c
	src/common/imagelib/img_utils.c
	src/common/imagelib/img_wad.c
	src/common/soundlib/libmpg/dct36.c
	src/common/soundlib/libmpg/dct64.c
	src/common/soundlib/libmpg/fmt123.h
	src/common/soundlib/libmpg/format.c
	src/common/soundlib/libmpg/frame.h
	src/common/soundlib/libmpg/frame.c
	src/common/soundlib/libmpg/getbits.h
	src/common/soundlib/libmpg/huffman.h
	src/common/soundlib/libmpg/index.h
	src/common/soundlib/libmpg/index.c
	src/common/soundlib/libmpg/layer3.c
	src/common/soundlib/libmpg/libmpg.h
	src/common/soundlib/libmpg/libmpg.c
	src/common/soundlib/libmpg/mpeghead.h
	src/common/soundlib/libmpg/mpg123.h
	src/common/soundlib/libmpg/mpg123.c
	src/common/soundlib/libmpg/parse.c
	src/common/soundlib/libmpg/reader.h
	src/common/soundlib/libmpg/reader.c
	src/common/soundlib/libmpg/sample.h
	src/common/soundlib/libmpg/synth.h
	src/common/soundlib/libmpg/synth.c
	src/common/soundlib/libmpg/tabinit.c
	src/common/soundlib/snd_main.c
	src/common/soundlib/snd_mp3.c
	src/common/soundlib/snd_utils.c
	src/common/soundlib/snd_wav.c
	src/common/soundlib/soundlib.h
	src/common/bsp/generic/viscompress.h
	src/common/bsp/generic/viscompress.c
	src/common/backends.h
	src/common/base_cmd.h
	src/common/base_cmd.c
	src/common/buildnum.h
	src/common/buildnum.c
	src/common/cfgscript.c
	src/common/cmd.c
	src/common/common.h
	src/common/common.c
	src/common/con_utils.c
	src/common/crashhandler.c
	src/common/custom.c
	src/common/cvar.h
	src/common/cvar.c
	$<$<BOOL:${NF_DEDICATED_SERVER}>:src/common/dedicated.c>
	src/common/engine_builddefs.h
	src/common/engine_mempool.h
	src/common/engine_mempool.c
	src/common/filesystem_engine.c
	src/common/fscallback.h
	src/common/gamma.c
	src/common/host.c
	src/common/host_state.c
	src/common/hpak.h
	src/common/hpak.c
	src/common/identification.c
	src/common/infostring.c
	src/common/ipv6text.h
	src/common/ipv6text.c
	src/common/kbutton.h
	$<$<BOOL:${NF_SINGLE_ENGINE_BINARY}>:src/common/launcher.c>
	src/common/lib_common.c
	src/common/library.h
	src/common/masterlist.c
	src/common/mod_bmodel.c
	src/common/model.c
	src/common/net_api.h
	src/common/model_dump.h
	src/common/model_dump.c
	src/common/mod_local.h
	src/common/mod_sprite.c
	src/common/mod_studio.c
	src/common/net_buffer.h
	src/common/net_buffer.c
	src/common/net_byteswap.h
	src/common/net_chan.c
	src/common/netchan.h
	src/common/net_encode.h
	src/common/net_encode.c
	src/common/net_ws.h
	src/common/net_ws.c
	src/common/pm_debug.c
	src/common/pm_local.h
	src/common/pm_surface.c
	src/common/pm_trace.c
	src/common/qfont.h
	src/common/sequence.h
	src/common/sequence.c
	src/common/setpair.h
	src/common/staticbuildchecks.h
	src/common/sys_con.c
	src/common/system.c
	src/common/system.h
	src/common/tests.h
	src/common/textureproperties.h
	src/common/textureproperties.c
	src/common/world.h
	src/common/world.c
	src/server/server.h
	src/server/sv_client.c
	src/server/sv_cmds.c
	src/server/sv_custom.c
	src/server/sv_filter.c
	src/server/sv_frame.c
	src/server/sv_game.c
	src/server/sv_init.c
	src/server/sv_log.c
	src/server/sv_main.c
	src/server/sv_move.c
	src/server/sv_phys.c
	src/server/sv_pmove.c
	src/server/sv_save.c
	src/server/sv_world.c
)

if(CLIENT_SUPPORT)
	list(APPEND ENGINE_SOURCES
		src/client/avi/avi_stub.c
		src/client/avi/avi_win.c
		src/client/avi/avi.h
		src/client/vgui/vgui_draw.h
		src/client/vgui/vgui_draw.c
		src/client/cdll_exp.h
		src/client/cl_bytenormals.h
		src/client/cl_bytenormals.c
		src/client/cl_cliententities.h
		src/client/cl_cliententities.c
		src/client/cl_cmds.c
		src/client/cl_custom.c
		src/client/cl_debug.c
		src/client/cl_demo.c
		src/client/cl_efrag.c
		src/client/cl_efx.c
		src/client/cl_events.c
		src/client/cl_font.c
		src/client/cl_frame.c
		src/client/cl_game.c
		src/client/cl_gameui.c
		src/client/client.h
		src/client/cl_main.c
		src/client/cl_mobile.c
		src/client/cl_netgraph.c
		src/client/cl_parse.c
		src/client/cl_pmove.c
		src/client/cl_qparse.c
		src/client/cl_remap.c
		src/client/cl_render.c
		src/client/cl_scrn.c
		src/client/cl_securedstub.c
		src/client/cl_surfaceprop_effects.h
		src/client/cl_surfaceprop_effects.c
		src/client/cl_tent.h
		src/client/cl_tent.c
		src/client/cl_uifs.h
		src/client/cl_uifs.c
		src/client/cl_uigl.h
		src/client/cl_uigl.c
		src/client/cl_video.c
		src/client/cl_view.c
		src/client/console.c
		src/client/in_joy.c
		src/client/input.h
		src/client/input.c
		src/client/ivoicetweak.h
		src/client/in_touch.c
		src/client/keys.c
		src/client/mod_dbghulls.c
		src/client/ref_common.h
		src/client/ref_common.c
		src/client/s_dsp.c
		src/client/s_load.c
		src/client/s_main.c
		src/client/s_mix.c
		src/client/s_mouth.c
		src/client/sound.h
		src/client/s_stream.c
		src/client/s_utils.c
		src/client/s_vox.c
		src/client/titles.c
		src/client/vid_common.h
		src/client/vid_common.c
		src/client/voice.h
		src/client/voice.c
		src/client/vox.h
	)
endif()

if(TARGET_PLATFORM STREQUAL "WINDOWS")
	list(APPEND ENGINE_SOURCES
		src/platform/win32/con_win.c
		src/platform/win32/lib_custom_win.c
		src/platform/win32/lib_win.h
		src/platform/win32/lib_win.c
		src/platform/win32/net.h
		src/platform/win32/sys_win.c
	)
else()
	list(APPEND ENGINE_SOURCES
		src/platform/posix/lib_posix.c
		src/platform/posix/net.h
		src/platform/posix/sys_posix.c
		src/platform/linux/in_evdev.c
		src/platform/linux/s_alsa.c
		src/platform/linux/sys_linux.c
		src/platform/linux/vid_fbdev.c
	)
endif()

if(SDL2_FOUND)
	list(APPEND ENGINE_SOURCES
		src/platform/sdl/events.h
		src/platform/sdl/events.c
		src/platform/sdl/in_sdl.c
		src/platform/sdl/s_sdl.c
		src/platform/sdl/sys_sdl.c
		src/platform/sdl/vid_sdl.c
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
	add_library(${TARGETNAME_LIB_ENGINE} SHARED
		${ENGINE_SOURCES}
	)
endif()

target_compile_definitions(${TARGETNAME_LIB_ENGINE} PRIVATE
	$<$<BOOL:${NF_DEDICATED_SERVER}>:BUILDFLAG_IS_DEDICATED_SERVER>
	$<$<BOOL:${NF_ENABLE_TESTS}>:BUILDFLAG_BUILD_ENGINE_TESTS=1>
	$<$<BOOL:${NF_ENABLE_STDIN_INPUT}>:BUILDFLAG_USE_SELECT=1>
	NIGHTFIRE_OPEN_ENGINE=1
	XASH_EXECUTABLE_NAME="${GAME_EXECUTABLE}"
	DBGHELP=$<BOOL:${WIN32}>
	PSAPI_VERSION=$<BOOL:${WIN32}>
	$<$<BOOL:${SDL2_FOUND}>:XASH_SDL=2>
	"ALLOCA_H=<${ALLOCA_HEADER}>"
)

target_link_libraries(${TARGETNAME_LIB_ENGINE}
	PUBLIC
	${TARGETNAME_LIB_FILESYSTEM_STDIO}

	PRIVATE
	${TARGETNAME_LIB_CRCLIB}
	${TARGETNAME_LIB_MINIZ}
	${TARGETNAME_LIB_MATHLIB}
	${TARGETNAME_LIB_CRTLIB}
	${TARGETNAME_LIB_PLATFORMLIB}
	${TARGETNAME_LIB_ENGINEPUBLICAPI}
	${TARGETNAME_LIB_ENGINEINTERNALAPI}
	${TARGETNAME_LIB_MEMPOOL}

	$<$<BOOL:${CLIENT_SUPPORT}>:${TARGETNAME_LIB_REF_OPENGL}>
	$<$<AND:$<BOOL:${CLIENT_SUPPORT}>,$<BOOL:${NF_EXPERIMENTAL_RENDERER}>>:${TARGETNAME_LIB_REF_OPENGL_EXPERIMENTAL}>
	$<$<BOOL:${CLIENT_SUPPORT}>:SDL2::SDL2>
	$<$<BOOL:${CLIENT_SUPPORT}>:Opus::opus>

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
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:dl>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_ENGINE})

target_include_directories(${TARGETNAME_LIB_ENGINE} PRIVATE
	src
)

if(CLIENT_SUPPORT)
	add_subdirectory(ref)
endif()

install(TARGETS ${TARGETNAME_LIB_ENGINE}
	ARCHIVE DESTINATION ${INSTALL_ROOT}
	LIBRARY DESTINATION ${INSTALL_ROOT}
	RUNTIME DESTINATION ${INSTALL_ROOT}
)

install_dependencies_from_directory(TARGET ${TARGETNAME_LIB_ENGINE} DIRECTORIES $<TARGET_FILE_DIR:SDL2::SDL2>)
