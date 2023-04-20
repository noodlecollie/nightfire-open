cmake_minimum_required(VERSION 3.21)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# This cannot use HOST_PLATFORM because we don't want to rely on custom scripts
# just yet. This is an early check and should use OG CMake variables only.
if(WIN32)
	set(VCPKG_OVERLAY_TRIPLETS "${CMAKE_SOURCE_DIR}/vcpkg-overlays")
	set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

project(Nightfire-Open LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(NF_DEDICATED_SERVER "Build dedicated server (no client-specific code)" OFF)
option(NF_SOFTWARE_RENDERER_ENABLED "Enables support for software rendering if building client" ON)
option(NF_OPENGL_RENDERER_ENABLED "Enables support for OpenGL rendering if building client" ON)
option(NF_OPENGL_STATIC_LINK "If OpenGL rendering is enabled, links statically against the OpenGL libraries" OFF)

include(project_definitions)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)

set(CMAKE_POSITION_INDEPENDENT_CODE YES)

if(LIB_MODE STREQUAL "STATIC")
	# NFTODO: Support this
	message(FATAL_ERROR "NFTODO: Support static engine compilation")
endif()

# Thanks to the Opus scripts for demonstrating how alloca
# detection is actually supposed to work.
include(CheckIncludeFile)
include(CheckSymbolExists)

check_include_file(alloca.h ALLOCA_H_EXISTS)

if(ALLOCA_H_EXISTS)
	set(ALLOCA_HEADER "alloca.h")
	check_symbol_exists(alloca "alloca.h" USE_ALLOCA_SUPPORTED)
else()
	set(ALLOCA_HEADER "malloc.h")
	check_symbol_exists(alloca "malloc.h" USE_ALLOCA_SUPPORTED)
endif()

if(NOT USE_ALLOCA_SUPPORTED)
	message(FATAL_ERROR "Could not determine alloca support in either alloca.h or malloc.h")
endif()

add_subdirectory(xash3d_engine)
add_subdirectory(game)
