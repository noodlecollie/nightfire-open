set(GENERATED_HEADER_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated_headers")
configure_file(configure/VCS.h.in ${GENERATED_HEADER_DIR}/BuildPlatform/VCS.h)

add_library(${TARGETNAME_LIB_BUILDPLATFORM} STATIC
	${GENERATED_HEADER_DIR}/BuildPlatform/VCS.h
	include/BuildPlatform/Decorators.h
	include/BuildPlatform/LibNames.h
	include/BuildPlatform/PlatformID.h
	include/BuildPlatform/StaticAssert.h
	include/BuildPlatform/Typedefs.h
	include/BuildPlatform/Utils.h
	src/Arch.c
	src/BuildEnums.h
)

target_include_directories(${TARGETNAME_LIB_BUILDPLATFORM}
	PUBLIC
	${GENERATED_HEADER_DIR}
	include

	PRIVATE
	src
)
