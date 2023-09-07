add_library(${TARGETNAME_LIB_PLATFORMDEFS} INTERFACE
	include/PlatformDefs/decorators.h
	include/PlatformDefs/libnames.h
	include/PlatformDefs/platformid.h
)

target_include_directories(${TARGETNAME_LIB_PLATFORMDEFS} INTERFACE include)
