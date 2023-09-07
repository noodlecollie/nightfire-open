add_library(${TARGETNAME_LIB_PLATFORMDEFS} INTERFACE
	include/PlatformDefs/build.h
	include/PlatformDefs/decorators.h
	include/PlatformDefs/libnames.h
)

target_include_directories(${TARGETNAME_LIB_PLATFORMDEFS} INTERFACE include)
