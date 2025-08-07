include(compiler_settings)

add_executable(${TARGETNAME_EXE_MDL14_DECOMPILER}
	src/Common/Container.h
	src/Common/Math.h
	src/Common/StreamHelpers.h
	src/Common/Types.h
	src/Conversions/Activity.h
	src/Conversions/Activity.cpp
	src/Conversions/MotionFlags.h
	src/Conversions/MotionFlags.cpp
	src/MDLv14/AnimationDataHolder.h
	src/MDLv14/AnimationDataHolder.cpp
	src/MDLv14/ComponentReader.h
	src/MDLv14/ComponentReader.cpp
	src/MDLv14/ComponentReflection.h
	src/MDLv14/Components.h
	src/MDLv14/MDLFile.h
	src/MDLv14/MDLFile.cpp
	src/MDLv14/SkinDataHolder.h
	src/MDLv14/SkinDataHolder.cpp
	src/QCv10/CommandWriter.h
	src/QCv10/CommandWriter.cpp
	src/QCv10/QCCommandReflection.h
	src/QCv10/QCCommandReflection.h
	src/QCv10/QCCommands.h
	src/QCv10/QCECommandReflection.h
	src/QCv10/QCECommands.h
	src/QCv10/QCEFile.h
	src/QCv10/QCEFile.cpp
	src/QCv10/QCFile.h
	src/QCv10/QCFile.cpp
	src/QCv10/QCFilePopulator.h
	src/QCv10/QCFilePopulator.cpp
	src/SMDv10/ComponentReflection.h
	src/SMDv10/Components.h
	src/SMDv10/SMDAnimationPopulator.h
	src/SMDv10/SMDAnimationPopulator.cpp
	src/SMDv10/SMDFile.h
	src/SMDv10/SMDFile.cpp
	src/SMDv10/SMDFileWriter.h
	src/SMDv10/SMDFileWriter.cpp
	src/SMDv10/SMDReferencePopulator.h
	src/SMDv10/SMDReferencePopulator.cpp
	src/Traits/Reflection.h
	src/AnalysisOptions.h
	src/BufferedFile.h
	src/BufferedFile.cpp
	src/BufferedFileReader.h
	src/BufferedFileReader.cpp
	src/Exceptions.h
	src/Filesystem.h
	src/Filesystem.cpp
	src/Main.cpp
	src/SigintHandler.h
	src/SigintHandler.cpp
	src/Utils.h
)

target_include_directories(${TARGETNAME_EXE_MDL14_DECOMPILER} PRIVATE
	src
)

set_common_library_compiler_settings(${TARGETNAME_EXE_MDL14_DECOMPILER})

target_compile_definitions(${TARGETNAME_EXE_MDL14_DECOMPILER} PRIVATE
	EXECUTABLE_NAME="${TARGETNAME_EXE_MDL14_DECOMPILER}"
	_USE_MATH_DEFINES
)

target_link_libraries(${TARGETNAME_EXE_MDL14_DECOMPILER} PRIVATE
	${TARGETNAME_LIB_ARGS}
	${TARGETNAME_LIB_CPPFS}
)

install(TARGETS ${TARGETNAME_EXE_MDL14_DECOMPILER}
	RUNTIME DESTINATION ${TOOLS_INSTALL_ROOT}
)
