# Unfortunately, these were determined manually
# based on what Depends.exe found that the freetype
# DLL was wanting.
find_package(Freetype REQUIRED)
find_package(BZip2 REQUIRED)
find_package(PNG REQUIRED)
find_package(unofficial-brotli CONFIG REQUIRED)

include(compiler_settings)

set(SOURCES_UI
	src/rmlui/FileInterfaceImpl.h
	src/rmlui/FileInterfaceImpl.cpp
	src/rmlui/FontEngineInterfaceImpl.h
	src/rmlui/FontEngineInterfaceImpl.cpp
	src/rmlui/RenderInterfaceImpl.h
	src/rmlui/RenderInterfaceImpl.cpp
	src/rmlui/RmlUiBackend.h
	src/rmlui/RmlUiBackend.cpp
	src/rmlui/SystemInterfaceImpl.h
	src/rmlui/SystemInterfaceImpl.cpp
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

target_compile_options(${TARGETNAME_LIB_UI} PRIVATE
	# MSVC
	$<$<STREQUAL:"${HOST_COMPILER}","MSVC">:/GR->

	# Not MSVC
	$<$<AND:$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>,$<COMPILE_LANGUAGE:CXX>>:-fno-rtti>
)

set_common_library_compiler_settings(${TARGETNAME_LIB_UI})

target_include_directories(${TARGETNAME_LIB_UI} PRIVATE
	src
)

# Adapted from https://stackoverflow.com/a/75065206/2054335
# TODO: Do we need to copy .so files on Linux? Perhaps we do -
# it's probably better to make sure that the game uses libs
# that we've compiled and know work (assuming that's what
# the rpath enforces).
# TODO: Convert the engine target to do something like
# this for SDL2.
install(TARGETS ${TARGETNAME_LIB_UI}
	COMPONENT ${TARGETNAME_LIB_UI}
	RUNTIME_DEPENDENCIES
	PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
	POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
	DIRECTORIES
	$<TARGET_FILE_DIR:PNG::PNG>
	$<TARGET_FILE_DIR:BZip2::BZip2>
	$<TARGET_FILE_DIR:freetype>
	$<TARGET_FILE_DIR:unofficial::brotli::brotlidec>
	ARCHIVE DESTINATION ${INSTALL_ROOT}
	LIBRARY DESTINATION ${INSTALL_ROOT}
	RUNTIME DESTINATION ${INSTALL_ROOT}
)
