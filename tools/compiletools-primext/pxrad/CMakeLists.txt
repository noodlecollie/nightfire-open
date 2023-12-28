# No options other than goldsrc are currently supported
set(COMPATIBILITY_TYPE "goldsrc")

add_executable(${TARGETNAME_EXE_NFRAD}_new
	alias.cpp
	$<$<OR:$<STREQUAL:"${COMPATIBILITY_TYPE}","p2">,$<STREQUAL:"${COMPATIBILITY_TYPE}","goldsrc">>:ambientcube.cpp>
	app_info.h
	dirtmap.cpp
	facepos.cpp
	lerp.cpp
	mat3x4_extra.h
	model_trace.h
	qrad.h
	randomfloat.h
	randomfloat.cpp
	$<$<STREQUAL:"${COMPATIBILITY_TYPE}","p2">:raytracer.h>
	$<$<STREQUAL:"${COMPATIBILITY_TYPE}","p2">:raytracer.cpp>
)

target_include_directories(${TARGETNAME_EXE_NFRAD}_new PRIVATE
	.
)

# pxrad compile-time settings
target_compile_definitions(${TARGETNAME_EXE_NFRAD}_new PRIVATE
	HLRAD_COMPRESS_TRANSFERS=1	# saves 80% memory
	HLRAD_RIGHTROUND=1			# round decimals to higher magnitudes
	HLRAD_TestLine_EDGE_FIX=1	# remove shadow artifacts in some cases
)

if(COMPATIBILITY_TYPE STREQUAL "p2")
	target_compile_definitions(${TARGETNAME_EXE_NFRAD}_new PRIVATE
		HLRAD_PSAVIOR=1
		HLRAD_REFLECTIVITY=1
		HLRAD_DELUXEMAPPING=1			# store deluxe vectors for bump-mapping
		#HLRAD_SHADOWMAPPING=0			# store shadow intensity for bump-mapping
		HLRAD_VERTEXLIGHTING=1 			# vertex lighting for static models
		HLRAD_LIGHTMAPMODELS=1			# calculate lightmaps on the models
		HLRAD_AMBIENTCUBES=1			# leaf ambient lighting
		HLRAD_COMPUTE_VISLIGHTMATRIX=1 	# compute vislight matrix
		HLRAD_EXTERNAL_TEXTURES=1		# allow to loading external textures
		#HLRAD_SHRINK_MEMORY=0			# TESTTEST
		HLRAD_RAYTRACE=1				# TESTTEST
	)
elseif(COMPATIBILITY_TYPE STREQUAL "p1")
	target_compile_definitions(${TARGETNAME_EXE_NFRAD}_new PRIVATE
		HLRAD_PARANOIA=1
		HLRAD_PARANOIA_BUMP=1			# compute Paranoia-style bump (three lightstyles)
		HLRAD_DELUXEMAPPING=1			# store deluxe vectors for bump-mapping
		HLRAD_VERTEXLIGHTING=1			# vertex lighting for static models
	)
elseif(COMPATIBILITY_TYPE STREQUAL "goldsrc")
	target_compile_definitions(${TARGETNAME_EXE_NFRAD}_new PRIVATE
		HLRAD_GOLDSRC=1
		HLRAD_REFLECTIVITY=1
		HLRAD_VERTEXLIGHTING=1			# vertex lighting for static models
		HLRAD_AMBIENTCUBES=1			# leaf ambient lighting
	)
endif()

if(NOT MSVC)
	target_compile_options(${TARGETNAME_EXE_NFRAD}_new PRIVATE
		-fno-exceptions
	)
endif()

if(ENABLE_MAP_COMPILERS_AVX2)
	if(MSVC)
		target_compile_options(${TARGETNAME_EXE_NFRAD}_new PRIVATE
			/arch:AVX2
		)
	else()
		message(WARNING "AVX2 support parameters not implemented for this compiler")
	endif()
endif()

target_link_libraries(${TARGETNAME_EXE_NFRAD}_new PRIVATE
	${TARGETNAME_LIB_COMPILETOOLS}
	${TARGETNAME_LIB_MINIUTL}
)

set_common_library_compiler_settings(${TARGETNAME_EXE_NFRAD}_new)

install(TARGETS ${TARGETNAME_EXE_NFRAD}_new
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
