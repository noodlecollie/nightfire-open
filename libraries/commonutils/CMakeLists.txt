add_library(${TARGETNAME_LIB_COMMONUTILS} INTERFACE
	include/CommonUtils/linux_win32_compat.h
)

target_include_directories(${TARGETNAME_LIB_COMMONUTILS}
	INTERFACE include
)

target_link_libraries(${TARGETNAME_LIB_COMMONUTILS}
	INTERFACE
	${TARGETNAME_LIB_PLATFORMDEFS}
)
