add_executable(${TARGETNAME_EXE_STUDIOMDL}
	monsters/activity.h
	monsters/activitymap.h
	bmpread.c
	bonetags.h
	bonetags.c
	cmdlib.h
	cmdlib.c
	dirent_win32.h
	fcaseopen.h
	fcaseopen.c
	mathlib.h
	mathlib.c
	pngread.c
	resource.h
	scriplib.h
	scriplib.c
	stb_image.h
	stb_image.c
	studio_event.h
	studio.h
	studiomdl.h
	studiomdl.c
	tags.h
	tags.c
	trilib.h
	trilib.c
	tristrip.c
	win32fix.h
	write.c
)

target_include_directories(${TARGETNAME_EXE_STUDIOMDL} PRIVATE
	.
	monsters
)

target_compile_options(${TARGETNAME_EXE_STUDIOMDL} PRIVATE
	${COMMON_COMPILE_OPTIONS}

	# MSVC
	$<$<STREQUAL:"${HOST_COMPILER}","MSVC">:/wd4313>
	$<$<STREQUAL:"${HOST_COMPILER}","MSVC">:/wd4477>

	# Not MSVC
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-int-to-pointer-cast>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-format-overflow>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-unused-result>
	$<$<NOT:$<STREQUAL:"${HOST_COMPILER}","MSVC">>:-Wno-attribute-warning>
)

target_compile_definitions(${TARGETNAME_EXE_STUDIOMDL} PRIVATE
	${COMMON_COMPILE_DEFINITIONS}
	STBI_NO_STDIO
	NOMINMAX
)

target_link_libraries(${TARGETNAME_EXE_STUDIOMDL} PRIVATE
	${COMMON_LINK_LIBRARIES}

	# Linux
	$<$<STREQUAL:"${TARGET_PLATFORM}","LINUX">:m>
)

install(TARGETS ${TARGETNAME_EXE_STUDIOMDL}
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
