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
