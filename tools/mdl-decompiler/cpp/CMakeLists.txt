include(compiler_settings)

add_executable(${TARGETNAME_EXE_MDL14_DECOMPILER}
	src/MDLv14/ComponentReader.h
	src/MDLv14/ComponentReader.cpp
	src/MDLv14/Components.h
	src/MDLv14/MDLFile.h
	src/MDLv14/MDLFile.cpp
	src/BufferedFile.h
	src/BufferedFile.cpp
	src/BufferedFileReader.h
	src/BufferedFileReader.cpp
	src/Exceptions.h
	src/Main.cpp
	src/Utils.h
)

target_include_directories(${TARGETNAME_EXE_MDL14_DECOMPILER} PRIVATE
	src
)

set_common_library_compiler_settings(${TARGETNAME_EXE_MDL14_DECOMPILER})

target_compile_definitions(${TARGETNAME_EXE_MDL14_DECOMPILER} PRIVATE
	EXECUTABLE_NAME="${TARGETNAME_EXE_MDL14_DECOMPILER}"
)

target_link_libraries(${TARGETNAME_EXE_MDL14_DECOMPILER} PRIVATE
	${TARGETNAME_LIB_ARGS}
)

install(TARGETS ${TARGETNAME_EXE_MDL14_DECOMPILER}
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
