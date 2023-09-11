include(compiler_settings)

add_library(${TARGETNAME_LIB_MINIUTL} STATIC
	bitstring.h
	bitstring.cpp
	fmtstr.h
	generichash.h
	generichash.cpp
	ipv6text.h
	ipv6text.c
	minbase_endian.h
	miniutl.h
	strtools.h
	strtools.cpp
	utlbuffer.h
	utlbuffer.cpp
	utlhashmap.h
	utllinkedlist.h
	utlmap.h
	utlmemory.h
	utlmemory.cpp
	utlpriorityqueue.h
	utlrbtree.h
	utlstring.h
	utlstring.cpp
	utlvector.h
	winlite.h
)

target_include_directories(${TARGETNAME_LIB_MINIUTL} PUBLIC
	.
)

target_link_libraries(${TARGETNAME_LIB_MINIUTL} PRIVATE
	${TARGETNAME_LIB_PLATFORMLIB}
)

set_common_library_compiler_settings(${TARGETNAME_LIB_MINIUTL})
