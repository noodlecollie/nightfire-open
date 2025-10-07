include(compiler_settings)

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

set(SOURCES_UI
	controls/Action.h
	controls/Action.cpp
	controls/BackgroundBitmap.h
	controls/BackgroundBitmap.cpp
	controls/BaseClientWindow.h
	controls/BaseClientWindow.cpp
	controls/BaseItem.h
	controls/BaseItem.cpp
	controls/BaseWindow.h
	controls/BaseWindow.cpp
	controls/Bitmap.h
	controls/Bitmap.cpp
	controls/CheckBox.h
	controls/CheckBox.cpp
	controls/DeveloperStudioSceneView.h
	controls/DeveloperStudioSceneView.cpp
	controls/Editable.h
	controls/Editable.cpp
	controls/Field.h
	controls/Field.cpp
	controls/Framework.h
	controls/Framework.cpp
	controls/ItemsHolder.h
	controls/ItemsHolder.cpp
	controls/MessageBox.h
	controls/MessageBox.cpp
	controls/PicButton.h
	controls/PicButton.cpp
	controls/PlayerModelView.h
	controls/PlayerModelView.cpp
	controls/ProgressBar.h
	controls/ProgressBar.cpp
	controls/ScrollView.h
	controls/ScrollView.cpp
	controls/Slider.h
	controls/Slider.cpp
	controls/SpinControl.h
	controls/SpinControl.cpp
	controls/StudioSceneView.h
	controls/StudioSceneView.cpp
	controls/Switch.h
	controls/Switch.cpp
	controls/Table.h
	controls/Table.cpp
	controls/TabView.h
	controls/TabView.cpp
	controls/YesNoMessageBox.h
	controls/YesNoMessageBox.cpp
	font/BaseFontBackend.cpp
	font/BaseFontBackend.h
	font/BitmapFont.cpp
	font/BitmapFont.h
	font/FontManager.cpp
	font/FontManager.h
	font/FontRenderer.h
	font/FreeTypeFont.cpp
	font/FreeTypeFont.h
	font/StbFont.cpp
	font/StbFont.h
	font/stb_truetype.h
	font/WinAPIFont.cpp
	font/WinAPIFont.h
	menus/botsetup/BotProfileListModel.h
	menus/botsetup/BotProfileListModel.cpp
	menus/botsetup/BotSetup.h
	menus/botsetup/BotSetup.cpp
	menus/botsetup/InGameBotListModel.h
	menus/botsetup/InGameBotListModel.cpp
	menus/dynamic/ScriptMenu.cpp
	menus/Acknowledgements.cpp
	menus/AdvancedControls.cpp
	menus/Audio.cpp
	menus/Configuration.cpp
	menus/ConnectionProgress.cpp
	menus/ConnectionWarning.cpp
	menus/Controls.cpp
	menus/CreateGame.cpp
	menus/Credits.cpp
	menus/CustomGame.cpp
	menus/FileDialog.cpp
	menus/GameOptions.cpp
	menus/Gamepad.cpp
	menus/InputDevices.cpp
	menus/LoadGame.cpp
	menus/Main.cpp
	menus/ModelViewer.cpp
	menus/Multiplayer.cpp
	menus/NewGame.cpp
	menus/PlayerIntroduceDialog.h
	menus/PlayerIntroduceDialog.cpp
	menus/PlayerSetup.cpp
	menus/SaveLoad.cpp
	menus/ServerBrowser.cpp
	menus/TouchButtons.cpp
	menus/Touch.cpp
	menus/TouchEdit.cpp
	menus/TouchOptions.cpp
	menus/Video.cpp
	menus/VideoModes.cpp
	menus/VideoOptions.cpp
	menus/Zoo.cpp
	model/BaseArrayModel.h
	model/BaseModel.h
	model/FilterableStringVectorModel.h
	model/FilterableStringVectorModel.cpp
	model/StringArrayModel.h
	model/StringVectorModel.h
	model/StudioSceneModel.h
	model/StudioSceneModel.cpp
	projectInterface_mainui/fileLoaders_mainui.h
	projectInterface_mainui/fileLoaders_mainui.cpp
	projectInterface_mainui/logInterface_mainui.h
	projectInterface_mainui/logInterface_mainui.cpp
	projectInterface_mainui/projectInterface_mainui.h
	projectInterface_mainui/projectInterface_mainui.cpp
	projectInterface_mainui/rng_mainui.h
	projectInterface_mainui/rng_mainui.cpp
	BaseMenu.h
	BaseMenu.cpp
	BMPUtils.h
	Btns.cpp
	BtnsBMPTable.h
	CFGScript.h
	CFGScript.cpp
	Color.h
	Coord.h
	enginecallback_menu.h
	EngineCallback.cpp
	EventSystem.h
	EventSystem.cpp
	extdll_menu.h
	FileDialogStaticData.cpp
	Image.h
	menufont.h
	MenuStrings.h
	MenuStrings.cpp
	Primitive.h
	Scissor.h
	Scissor.cpp
	udll_int.cpp
	unicode_strtools.h
	unicode_strtools.cpp
	Utils.h
	Utils.cpp
	WindowSystem.h
	WindowSystem.cpp
)

add_library(${TARGETNAME_LIB_UI} SHARED
	${SOURCES_UI}
	${SOURCES_GAME_MENU_SHARED}
)

target_compile_definitions(${TARGETNAME_LIB_UI} PRIVATE
	MAINUI_USE_CUSTOM_FONT_RENDER=1

	# NFTODO: Enable freetype2 and fontconfig for Linux, then
	# we can use MAINUI_USE_FREETYPE=1 for Linux here too.
	# Only enable if not using STB libraries and not building
	# with LOW_MEMORY.
	$<$<NOT:$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">>:MAINUI_USE_STB=1>
)

target_compile_options(${TARGETNAME_LIB_UI} PRIVATE
	# MSVC
	$<$<STREQUAL:"${HOST_COMPILER}","MSVC">:/GR->

	# Not MSVC
	$<$<AND:$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>,$<COMPILE_LANGUAGE:CXX>>:-fno-rtti>
)

target_link_libraries(${TARGETNAME_LIB_UI} PRIVATE
	${TARGETNAME_LIB_MINIUTL}
	${TARGETNAME_LIB_PLATFORMLIB}
	${TARGETNAME_LIB_BUILDPLATFORM}
	${TARGETNAME_LIB_ENGINEPUBLICAPI}
	${TARGETNAME_LIB_ENGINEINTERNALAPI}

	# Windows
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:USER32>
	$<$<STREQUAL:"${TARGET_PLATFORM}","WINDOWS">:GDI32>

	# Linux
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:rt>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_UI})

target_include_directories(${TARGETNAME_LIB_UI} PRIVATE
	.
	miniutl
	font
	controls
	menus
	model
	projectInterface_mainui
	../game_menu_shared
)

install(TARGETS ${TARGETNAME_LIB_UI}
	ARCHIVE DESTINATION ${INSTALL_ROOT}
	LIBRARY DESTINATION ${INSTALL_ROOT}
	RUNTIME DESTINATION ${INSTALL_ROOT}
)
