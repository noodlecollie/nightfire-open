cmake_minimum_required(VERSION 3.21)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(Nightfire-Open LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(NF_DEDICATED_SERVER "Build dedicated server (no client-specific code)" OFF)
option(NF_SOFTWARE_RENDERER_ENABLED "Enables support for software rendering if building client" ON)

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

add_subdirectory(xash3d_engine)
