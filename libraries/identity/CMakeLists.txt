include(compiler_settings)

set(GENERATED_HEADER_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated_headers")

configure_file(configure/vcs.h.in ${GENERATED_HEADER_DIR}/Identity/vcs.h)

add_library(${TARGETNAME_LIB_IDENTITY} STATIC
	${GENERATED_HEADER_DIR}/Identity/vcs.h
	include/Identity/arch.h
	src/arch.c
	src/buildenums.h
)

target_include_directories(${TARGETNAME_LIB_IDENTITY}
	PUBLIC
	${GENERATED_HEADER_DIR}
	include

	PRIVATE
	src
)

target_link_libraries(${TARGETNAME_LIB_IDENTITY}
	PUBLIC
	${TARGETNAME_LIB_PLATFORMDEFS}

	PRIVATE
	${TARGETNAME_LIB_CRTLIB}
)

set_common_library_compiler_settings(${TARGETNAME_LIB_IDENTITY})
