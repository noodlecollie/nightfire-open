add_library(${TARGETNAME_LIB_CRTLIB} STATIC
	include/CRTLib/bitdefs.h
	include/CRTLib/crtlib.h
	src/crtlib.c
)

target_include_directories(${TARGETNAME_LIB_CRTLIB}
	PUBLIC include
	PRIVATE src
)

target_link_libraries(${TARGETNAME_LIB_CRTLIB}
	PUBLIC
	${TARGETNAME_LIB_PLATFORMDEFS}

	PRIVATE
	${TARGETNAME_LIB_PLATFORMLIB}
)
