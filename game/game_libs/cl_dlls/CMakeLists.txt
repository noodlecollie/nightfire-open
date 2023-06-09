set(SOURCES_PM_SHARED
	../pm_shared/pm_debug.h
	../pm_shared/pm_debug.c
	../pm_shared/pm_defs.h
	../pm_shared/pm_info.h
	../pm_shared/pm_materials.h
	../pm_shared/pm_math.c
	../pm_shared/pm_movevars.h
	../pm_shared/pm_shared.h
	../pm_shared/pm_shared.c
)

set(SOURCES_GAME_SHARED
	../game_shared/customGeometry/constructors/aabboxConstructor.h
	../game_shared/customGeometry/constructors/aabboxConstructor.cpp
	../game_shared/customGeometry/geometryItem.h
	../game_shared/customGeometry/geometryItem.cpp
	../game_shared/customGeometry/sharedDefs.h
	../game_shared/customGeometry/sharedDefs.cpp
	../game_shared/eventConstructor/eventConstructor.h
	../game_shared/eventConstructor/eventConstructor.cpp
	../game_shared/gameplay/inaccuracymodifiers.h
	../game_shared/gameplay/inaccuracymodifiers.cpp
	../game_shared/gameplay/radialdamage.h
	../game_shared/gameplay/radialdamage.cpp
	../game_shared/gameplay/weaponInaccuracyCalculator.h
	../game_shared/gameplay/weaponInaccuracyCalculator.cpp
	../game_shared/resources/BaseResourceCollection.h
	../game_shared/resources/FootstepSoundResources.h
	../game_shared/resources/FootstepSoundResources.cpp
	../game_shared/resources/ItemSoundResources.h
	../game_shared/resources/ItemSoundResources.cpp
	../game_shared/resources/IterableResourceCollection.h
	../game_shared/resources/MaterialBreakSoundResources.h
	../game_shared/resources/MaterialBreakSoundResources.cpp
	../game_shared/resources/PlayerMoveResources.h
	../game_shared/resources/PlayerMoveResources.cpp
	../game_shared/resources/PlayerSoundResources.h
	../game_shared/resources/PlayerSoundResources.cpp
	../game_shared/resources/ShellImpactSoundResources.h
	../game_shared/resources/ShellImpactSoundResources.cpp
	../game_shared/resources/SoundResources.h
	../game_shared/resources/SoundResources.cpp
	../game_shared/resources/SurfaceAttributes.h
	../game_shared/resources/SurfaceAttributes.cpp
	../game_shared/resources/SurfaceDecalResources.h
	../game_shared/resources/SurfaceDecalResources.cpp
	../game_shared/resources/SurfaceSoundResources.h
	../game_shared/resources/SurfaceSoundResources.cpp
	../game_shared/resources/TextureResources.h
	../game_shared/resources/TextureResources.cpp
	../game_shared/resources/WeaponSoundResources.h
	../game_shared/resources/WeaponSoundResources.cpp
	../game_shared/screenOverlays/screenOverlayIds.h
	../game_shared/sound/SoundInstance.h
	../game_shared/sound/SoundInstance.cpp
	../game_shared/util/cvarFuncs.h
	../game_shared/util/debugLogger.h
	../game_shared/util/debugLogger.cpp
	../game_shared/util/extramath.h
	../game_shared/util/weightedvaluelist.h
	../game_shared/weaponattributes/weaponatts_ammobasedattack.h
	../game_shared/weaponattributes/weaponatts_ammodef.h
	../game_shared/weaponattributes/weaponatts_baseattack.h
	../game_shared/weaponattributes/weaponatts_base.h
	../game_shared/weaponattributes/weaponatts_collection.h
	../game_shared/weaponattributes/weaponatts_collection.cpp
	../game_shared/weaponattributes/weaponatts_core.h
	../game_shared/weaponattributes/weaponatts_hitscanattack.h
	../game_shared/weaponattributes/weaponatts_meleeattack.h
	../game_shared/weaponattributes/weaponatts_playermodel.h
	../game_shared/weaponattributes/weaponatts_prediction.h
	../game_shared/weaponattributes/weaponatts_projectileattack.h
	../game_shared/weaponattributes/weaponatts_skillrecord.h
	../game_shared/weaponattributes/weaponatts_skillrecord.cpp
	../game_shared/weaponattributes/weaponatts_soundset.h
	../game_shared/weaponattributes/weaponatts_viewmodel.h
	../game_shared/weapons/ammodefs.h
	../game_shared/weapons/ammodefs.cpp
	../game_shared/weapons/crossbow.cpp
	../game_shared/weapons/crowbar.cpp
	../game_shared/weapons/egon.cpp
	../game_shared/weapons/gauss.cpp
	../game_shared/weapons/generichitscanweapon.h
	../game_shared/weapons/generichitscanweapon.cpp
	../game_shared/weapons/genericmeleeweapon.h
	../game_shared/weapons/genericmeleeweapon.cpp
	../game_shared/weapons/genericprojectileweapon.h
	../game_shared/weapons/genericprojectileweapon.cpp
	../game_shared/weapons/genericweapon.h
	../game_shared/weapons/genericweapon.cpp
	../game_shared/weapons/glock.cpp
	../game_shared/weapons/handgrenade.cpp
	../game_shared/weapons/hornetgun.cpp
	../game_shared/weapons/mp5.cpp
	../game_shared/weapons/python.cpp
	../game_shared/weapons/rpg.cpp
	../game_shared/weapons/satchel.cpp
	../game_shared/weapons/shotgun.cpp
	../game_shared/weapons/squeakgrenade.cpp
	../game_shared/weapons/tripmine.cpp
	../game_shared/weapons/weapon_fists_atts.h
	../game_shared/weapons/weapon_fists.h
	../game_shared/weapons/weapon_fists.cpp
	../game_shared/weapons/weapon_frinesi_atts.h
	../game_shared/weapons/weapon_frinesi.h
	../game_shared/weapons/weapon_frinesi.cpp
	../game_shared/weapons/weapon_grenadelauncher_atts.h
	../game_shared/weapons/weapon_grenadelauncher.h
	../game_shared/weapons/weapon_grenadelauncher.cpp
	../game_shared/weapons/weaponids.h
	../game_shared/weapons/weapon_l96a1.cpp
	../game_shared/weapons/weapon_l96a1_atts.h
	../game_shared/weapons/weapon_l96a1.h
	../game_shared/weapons/weapon_p99.cpp
	../game_shared/weapons/weapon_p99_atts.h
	../game_shared/weapons/weapon_p99.h
	../game_shared/weapons/weapon_pref_weights.h
	../game_shared/weapons/weapon_raptor_atts.h
	../game_shared/weapons/weapon_raptor.h
	../game_shared/weapons/weapon_raptor.cpp
	../game_shared/weapons/weaponregistry.h
	../game_shared/weapons/weaponregistry.cpp
	../game_shared/weapons/weapons.h
	../game_shared/weapons/weapon_silent_atts.h
	../game_shared/weapons/weapon_silent.h
	../game_shared/weapons/weapon_silent.cpp
	../game_shared/weapons/weaponslots.h
	../game_shared/weapons/weaponslots.cpp
	../game_shared/studio_utils_shared.h
	../game_shared/studio_utils_shared.cpp

	# NFTODO: Other game_shared files in the root of the directory
	# (apart from voice_gamemgr.cpp) don't appear to be used.
	# Remove them and see if anything breaks.
)

# NFTODO: Make a proper library out of this
set(SOURCES_GAME_MENU_SHARED
	../game_menu_shared/botprofiles/botprofileparser.h
	../game_menu_shared/botprofiles/botprofileparser.cpp
	../game_menu_shared/botprofiles/botprofiletable.h
	../game_menu_shared/botprofiles/botprofiletable.cpp
	../game_menu_shared/coreutil/coreutil.h
	../game_menu_shared/filesystemops/FileSystemOps.h
	../game_menu_shared/filesystemops/FileSystemOps.cpp
	../game_menu_shared/gameresources/GameResources.h
	../game_menu_shared/gameresources/GameResources.cpp
	../game_menu_shared/iterators/utlhashmapconstiterator.h
	../game_menu_shared/projectInterface/fixedSizeMessageBuffer.h
	../game_menu_shared/projectInterface/IFileLoader.h
	../game_menu_shared/projectInterface/ILogInterface.h
	../game_menu_shared/projectInterface/IProjectInterface.h
	../game_menu_shared/projectInterface/IProjectInterface.cpp
	../game_menu_shared/projectInterface/IRNG.h

	# NFTODO: Make a proper library out of these
	../game_menu_shared/rapidjson/error/en.h
	../game_menu_shared/rapidjson/error/error.h
	../game_menu_shared/rapidjson/internal/biginteger.h
	../game_menu_shared/rapidjson/internal/diyfp.h
	../game_menu_shared/rapidjson/internal/dtoa.h
	../game_menu_shared/rapidjson/internal/ieee754.h
	../game_menu_shared/rapidjson/internal/itoa.h
	../game_menu_shared/rapidjson/internal/meta.h
	../game_menu_shared/rapidjson/internal/pow10.h
	../game_menu_shared/rapidjson/internal/regex.h
	../game_menu_shared/rapidjson/internal/stack.h
	../game_menu_shared/rapidjson/internal/strfunc.h
	../game_menu_shared/rapidjson/internal/strtod.h
	../game_menu_shared/rapidjson/internal/swap.h
	../game_menu_shared/rapidjson/msinttypes/inttypes.h
	../game_menu_shared/rapidjson/msinttypes/stdint.h
	../game_menu_shared/rapidjson/allocators.h
	../game_menu_shared/rapidjson/document.h
	../game_menu_shared/rapidjson/encodedstream.h
	../game_menu_shared/rapidjson/encodings.h
	../game_menu_shared/rapidjson/filereadstream.h
	../game_menu_shared/rapidjson/filewritestream.h
	../game_menu_shared/rapidjson/fwd.h
	../game_menu_shared/rapidjson/istreamwrapper.h
	../game_menu_shared/rapidjson/memorybuffer.h
	../game_menu_shared/rapidjson/memorystream.h
	../game_menu_shared/rapidjson/ostreamwrapper.h
	../game_menu_shared/rapidjson/pointer.h
	../game_menu_shared/rapidjson/prettywriter.h
	../game_menu_shared/rapidjson/rapidjson.h
	../game_menu_shared/rapidjson/reader.h
	../game_menu_shared/rapidjson/schema.h
	../game_menu_shared/rapidjson/stream.h
	../game_menu_shared/rapidjson/stringbuffer.h
	../game_menu_shared/rapidjson/writer.h
	../game_menu_shared/rapidjson_helpers/rapidjson_helpers.h
	../game_menu_shared/rapidjson_helpers/rapidjson_helpers.cpp
)

set(SOURCES_CLIENT
	customGeometry/baseGeometryCollection.h
	customGeometry/baseGeometryCollection.cpp
	customGeometry/geometryCollectionManager.h
	customGeometry/geometryCollectionManager.cpp
	customGeometry/geometryRenderer.h
	customGeometry/geometryRenderer.cpp
	customGeometry/geometryStatics.h
	customGeometry/geometryStatics.cpp
	customGeometry/logger_client.h
	customGeometry/logger_client.cpp
	customGeometry/messageReader.h
	customGeometry/messageReader.cpp
	events/debugging/hitscanweapondebugging.h
	events/debugging/hitscanweapondebugging.cpp
	events/baseweaponeventplayer.h
	events/baseweaponeventplayer.cpp
	events/eventCommands.h
	events/eventCommands.cpp
	events/hitscanweaponeventplayer.h
	events/hitscanweaponeventplayer.cpp
	events/meleeweaponeventplayer.h
	events/meleeweaponeventplayer.cpp
	events/projectileweaponeventplayer.h
	events/projectileweaponeventplayer.cpp
	gameplay/crosshairCvars.h
	gameplay/crosshairCvars.cpp
	hl/hl_baseentity.cpp
	hl/hl_events.cpp
	hl/hl_objects.cpp
	hl/hl_weapons.cpp
	messages/baseMessageReader.h
	messages/baseMessageReader.cpp
	projectInterface_client/fileLoaders_client.h
	projectInterface_client/fileLoaders_client.cpp
	projectInterface_client/logInterface_client.h
	projectInterface_client/logInterface_client.cpp
	projectInterface_client/projectInterface_client.h
	projectInterface_client/projectInterface_client.cpp
	projectInterface_client/rng_client.h
	projectInterface_client/rng_client.cpp
	sound/ClientSoundInstance.h
	sound/ClientSoundInstance.cpp
	ui/core/uiVec2.h
	ui/crosshair/crosshairBase.h
	ui/crosshair/crosshairParameters.h
	ui/crosshair/crosshairParameters.cpp
	ui/crosshair/spreadVisualiser.h
	ui/crosshair/spreadVisualiser.cpp
	ui/drawable/BaseUIDrawable.h
	ui/screenOverlays/BaseScreenOverlay.h
	ui/screenOverlays/BaseScreenOverlay.cpp
	ui/screenOverlays/DebugCommands.h
	ui/screenOverlays/DebugCommands.cpp
	ui/screenOverlays/messageReader.h
	ui/screenOverlays/messageReader.cpp
	ui/screenOverlays/ScreenOverlayContainer.h
	ui/screenOverlays/ScreenOverlayContainer.cpp
	ui/screenOverlays/SniperScopeOverlay.h
	ui/screenOverlays/SniperScopeOverlay.cpp
	ammo_secondary.cpp
	ammo.h
	ammo.cpp
	ammohistory.h
	ammohistory.cpp
	battery.cpp
	camera.h
	cdll_int.cpp
	cl_dll.h
	cl_util.h
	com_weapons.h
	com_weapons.cpp
	death.cpp
	demo.h
	demo.cpp
	entity.cpp
	ev_common.cpp
	ev_hldm.h
	ev_hldm.cpp
	events.cpp
	eventscripts.h
	flashlight.cpp
	GameStudioModelRenderer.h
	GameStudioModelRenderer.cpp
	geiger.cpp
	health.cpp
	hud_crosshair.h
	hud_crosshair.cpp
	hud_iface.h
	hud_msg.cpp
	hud_redraw.cpp
	hud_spectator.h
	hud_spectator.cpp
	hud_update.cpp
	hud.h
	hud.cpp
	in_camera.cpp
	in_defs.h
	input_goldsource.cpp
	input_mouse.h
	input_mouse.cpp
	input_xash3d.cpp
	input.cpp
	kbutton.h
	menu.cpp
	message.cpp
	MOTD.cpp
	overview.h
	overview.cpp
	parsemsg.h
	parsemsg.cpp
	saytext.cpp
	scoreboard.cpp
	status_icons.cpp
	statusbar.cpp
	studio_util.h
	studio_util.cpp
	StudioModelRenderer.h
	StudioModelRenderer.cpp
	text_message.cpp
	tf_defs.h
	train.cpp
	tri.cpp
	util.cpp
	view.h
	view.cpp
)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

if(TARGET_IS_64BIT)
	set(CMAKE_SHARED_LIBRARY_SUFFIX "_amd64${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

add_library(${TARGETNAME_LIB_CLIENT} SHARED
	${SOURCES_PM_SHARED}
	${SOURCES_GAME_SHARED}
	${SOURCES_CLIENT}
	${SOURCES_GAME_MENU_SHARED}
)

target_link_libraries(${TARGETNAME_LIB_CLIENT} PRIVATE
	${TARGETNAME_LIB_MINIUTL}
	${TARGETNAME_LIB_PLATFORMLIB}
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:USER32>
)

# Since this library is looked up entirely at runtime,
# it is not depended upon by anything else in this repo,
# so all include paths are private.
target_include_directories(${TARGETNAME_LIB_CLIENT} PRIVATE
	.
	../common
	../engine
	../pm_shared
	../game_shared
	../shared_utils/cpp
	../public
	../game_shared/weapons
	../game_shared/weaponattributes
	../game_shared/util
	../game_shared/gameplay
	../game_menu_shared
	../miniutl
	../utils/false_vgui/include
	hl
	events
	projectInterface_client
)

target_compile_definitions(${TARGETNAME_LIB_CLIENT} PRIVATE
	CLIENT_DLL=1
	$<$<BOOL:${NF_CLIENT_WEAPON_PREDICTION}>:CLIENT_WEAPONS=1>

	# Windows
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:_WIN32=1>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:WIN32=1>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:NOMINMAX=1>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:_HAS_EXCEPTIONS=0>

	# Linux
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:_LINUX=1>
)

target_compile_options(${TARGETNAME_LIB_CLIENT} PRIVATE
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-fno-exceptions>

	# This is used for networking entity properties
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-invalid-offsetof>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_CLIENT})

install(TARGETS ${TARGETNAME_LIB_CLIENT}
	RUNTIME DESTINATION "${INSTALL_ROOT}/${GAME_FOLDER_NAME}/cl_dlls"
	LIBRARY DESTINATION "${INSTALL_ROOT}/${GAME_FOLDER_NAME}/cl_dlls"
)
