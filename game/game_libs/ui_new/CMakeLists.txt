# Unfortunately, these were determined manually
# based on what Depends.exe found that the freetype
# DLL was wanting.
find_package(Freetype REQUIRED)
find_package(BZip2 REQUIRED)
find_package(PNG REQUIRED)
find_package(unofficial-brotli CONFIG REQUIRED)

include(compiler_settings)
include(nf_utils)

set(CMAKE_INSTALL_RPATH "\$ORIGIN")

set(SOURCES_UI
	src/components/ModalComponent.h
	src/components/ModalComponent.cpp
	src/framework/BaseComponent.h
	src/framework/BaseComponent.cpp
	src/framework/BaseCvarModel.h
	src/framework/BaseCvarModel.cpp
	src/framework/BaseMenu.h
	src/framework/BaseMenu.cpp
	src/framework/BaseTableModel.h
	src/framework/BaseTemplateBinding.h
	src/framework/CvarAccessor.h
	src/framework/CvarDataVar.h
	src/framework/DataVar.h
	src/framework/DocumentObserver.h
	src/framework/DocumentObserver.cpp
	src/framework/ElementFinder.h
	src/framework/ElementFinder.cpp
	src/framework/EventListenerObject.h
	src/framework/MenuDirectory.h
	src/framework/MenuDirectory.cpp
	src/framework/MenuPage.h
	src/framework/MenuPage.cpp
	src/framework/MenuStack.h
	src/framework/MenuStack.cpp
	src/menus/options/AvOptionsMenu.h
	src/menus/options/AvOptionsMenu.cpp
	src/menus/options/BaseOptionsMenu.h
	src/menus/options/BaseOptionsMenu.cpp
	src/menus/options/GameplayOptionsMenu.h
	src/menus/options/GameplayOptionsMenu.cpp
	src/menus/options/KeysOptionsMenu.h
	src/menus/options/KeysOptionsMenu.cpp
	src/menus/options/MouseOptionsMenu.h
	src/menus/options/MouseOptionsMenu.cpp
	src/menus/MainMenu.h
	src/menus/MainMenu.cpp
	src/menus/MainMenu.cpp
	src/menus/MultiplayerMenu.h
	src/menus/MultiplayerMenu.cpp
	src/models/KeyBindingModel.h
	src/models/KeyBindingModel.cpp
	src/rmlui/EventListenerImpl.h
	src/rmlui/EventListenerImpl.cpp
	src/rmlui/EventListenerInstancerImpl.h
	src/rmlui/EventListenerInstancerImpl.cpp
	src/rmlui/FileInterfaceImpl.h
	src/rmlui/FileInterfaceImpl.cpp
	src/rmlui/RenderInterfaceImpl.h
	src/rmlui/RenderInterfaceImpl.cpp
	src/rmlui/RmlUiBackend.h
	src/rmlui/RmlUiBackend.cpp
	src/rmlui/SystemInterfaceImpl.h
	src/rmlui/SystemInterfaceImpl.cpp
	src/rmlui/TextInputHandlerImpl.h
	src/rmlui/TextInputHandlerImpl.cpp
	src/rmlui/Utils.h
	src/rmlui/Utils.cpp
	src/templatebindings/MenuFrameDataBinding.h
	src/templatebindings/MenuFrameDataBinding.cpp
	src/templatebindings/OptionsTabBarDataBinding.h
	src/templatebindings/OptionsTabBarDataBinding.cpp
	src/utils/InFilePtr.h
	src/udll_int.h
	src/udll_int.cpp
	src/UIDebug.h
	src/UIDebug.cpp
)

add_library(${TARGETNAME_LIB_UI} SHARED
	${SOURCES_UI}
)

target_link_libraries(${TARGETNAME_LIB_UI} PRIVATE
	${TARGETNAME_LIB_PLATFORMLIB}
	${TARGETNAME_LIB_BUILDPLATFORM}
	${TARGETNAME_LIB_ENGINEPUBLICAPI}
	${TARGETNAME_LIB_ENGINEINTERNALAPI}
	${TARGETNAME_LIB_RMLUI}
	${TARGETNAME_LIB_CRTLIB}
)

set_common_library_compiler_settings(${TARGETNAME_LIB_UI})

target_include_directories(${TARGETNAME_LIB_UI} PRIVATE
	src
)

install(TARGETS ${TARGETNAME_LIB_UI}
	ARCHIVE DESTINATION ${INSTALL_ROOT}
	LIBRARY DESTINATION ${INSTALL_ROOT}
	RUNTIME DESTINATION ${INSTALL_ROOT}
)

install_dependencies_from_directory(
	TARGET
	${TARGETNAME_LIB_UI}
	DIRECTORIES
	$<TARGET_FILE_DIR:PNG::PNG>
	$<TARGET_FILE_DIR:BZip2::BZip2>
	$<TARGET_FILE_DIR:freetype>
	$<TARGET_FILE_DIR:unofficial::brotli::brotlidec>
)
