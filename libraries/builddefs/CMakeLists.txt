add_library(${TARGETNAME_LIB_BUILDDEFS} INTERFACE
	include/BuildDefs/build.h
	include/BuildDefs/decorators.h
)

target_include_directories(${TARGETNAME_LIB_BUILDDEFS} INTERFACE include)
