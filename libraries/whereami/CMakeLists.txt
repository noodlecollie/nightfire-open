add_library(${TARGETNAME_LIB_WHEREAMI} STATIC
	include/WhereAmI/whereami.h
	src/whereami.c
)

target_include_directories(${TARGETNAME_LIB_WHEREAMI}
	PUBLIC include
	PRIVATE src
)

target_link_libraries(${TARGETNAME_LIB_WHEREAMI} PRIVATE
	${TARGETNAME_LIB_PLATFORMLIB}
)
