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
option(NF_SINGLE_ENGINE_BINARY "Build Xash3D engine as single binary (always enabled for dedicated server builds)" OFF)
option(NF_LOW_MEMORY_SUPPORT "Build with reduced memory footprint" OFF)
option(NF_SOFTWARE_RENDERER_ENABLED "Enables support for software rendering if building client" ON)
option(NF_OPENGL_RENDERER_ENABLED "Enables support for OpenGL rendering if building client" ON)
option(NF_OPENGL_STATIC_LINK "If OpenGL rendering is enabled, links statically against the OpenGL libraries" OFF)
option(NF_ENABLE_TESTS "Enables test code in targets" OFF)
option(NF_ENABLE_CUSTOM_SWAP "Enables custom memory swap code for specialist devices with no swap functionality" OFF)
option(NF_ENABLE_STDIN_INPUT "Enable console input from stdin (always enabled for dedicated server builds)" OFF)
option(NF_CLIENT_WEAPON_PREDICTION "Enables or disables prediction of weapons on the game client" ON)

include(project_definitions)

if(NF_ENABLE_CUSTOM_SWAP AND TARGET_PLATFORM STREQUAL "WINDOWS")
	message(FATAL_ERROR "Custom swap is not supported on Windows builds")
endif()

if(NF_ENABLE_STDIN_INPUT AND TARGET_PLATFORM STREQUAL "WINDOWS")
	message(WARNING "Using stdin is not supported on Windows builds, ignoring NF_ENABLE_STDIN_INPUT")
	set(NF_ENABLE_STDIN_INPUT OFF)
endif()

if(NF_DEDICATED_SERVER)
	set(NF_SINGLE_ENGINE_BINARY ON)

	if(NOT TARGET_PLATFORM STREQUAL "WINDOWS")
		set(NF_ENABLE_STDIN_INPUT ON)
	endif()
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)

set(CMAKE_POSITION_INDEPENDENT_CODE YES)

set(VCS_COMMIT_ID "notset")

find_package(Git QUIET)

if(Git_FOUND)
	execute_process(
		COMMAND "${GIT_EXECUTABLE}" rev-parse --short HEAD
		WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
		OUTPUT_VARIABLE VCS_COMMIT_ID
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
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

add_subdirectory(platformlib)
add_subdirectory(xash3d_engine)
add_subdirectory(game)
