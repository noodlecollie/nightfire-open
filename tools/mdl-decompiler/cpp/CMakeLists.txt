add_executable(${TARGETNAME_EXE_MDL14_DECOMPILER}
	src/MDLV14Components/BoundingBox.h
	src/MDLV14Components/BoundingBox.cpp
	src/MDLV14Components/EyePosition.h
	src/MDLV14Components/EyePosition.cpp
	src/MDLV14Components/HeaderV14.h
	src/MDLV14Components/HeaderV14.cpp
	src/MDLV14Components/ISerialisableComponent.h
	src/MDLV14Components/MDLFileV14.h
	src/MDLV14Components/MDLFileV14.cpp
	src/BufferedFile.h
	src/BufferedFile.cpp
	src/BufferedFileReader.h
	src/BufferedFileReader.cpp
	src/Exceptions.h
	src/Main.cpp
)

target_include_directories(${TARGETNAME_EXE_MDL14_DECOMPILER} PRIVATE
	src
)

target_compile_options(${TARGETNAME_EXE_MDL14_DECOMPILER} PRIVATE
	${COMMON_COMPILE_OPTIONS}
)

install(TARGETS ${TARGETNAME_EXE_MDL14_DECOMPILER}
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
