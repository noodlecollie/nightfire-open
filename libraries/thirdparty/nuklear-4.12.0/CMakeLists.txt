add_library(${TARGETNAME_LIB_NUKLEAR} INTERFACE
	include/nuklear/nuklear.h
)

target_include_directories(${TARGETNAME_LIB_NUKLEAR} INTERFACE include)
