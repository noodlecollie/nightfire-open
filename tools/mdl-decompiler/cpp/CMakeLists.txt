add_executable(${TARGETNAME_EXE_MDL14_DECOMPILER}
	src/BufferedFileReader.h
	src/BufferedFileReader.cpp
	src/Exceptions.h
	src/Main.cpp
)

target_compile_options(${TARGETNAME_EXE_MDL14_DECOMPILER} PRIVATE
	${COMMON_COMPILE_OPTIONS}
)

install(TARGETS ${TARGETNAME_EXE_MDL14_DECOMPILER}
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
