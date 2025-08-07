add_library(${TARGETNAME_LIB_ARGS} INTERFACE
	args/args.hxx
)

target_include_directories(${TARGETNAME_LIB_ARGS} INTERFACE .)
