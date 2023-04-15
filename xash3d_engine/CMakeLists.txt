############################################################
#       Common header sets used by multiple targets        #
############################################################

# NFTODO: These should really be managed more cleanly.
# Targets should not be reaching into the headers of other
# targets if they do not link against them. Decouple these
# when possible.

add_library(${TARGETNAME_INCLUDES_SDK} INTERFACE)

target_include_directories(${TARGETNAME_INCLUDES_SDK} INTERFACE
	public
	common
	pm_shared
	engine
)

add_library(${TARGETNAME_INCLUDES_FILESYSTEM} INTERFACE)

target_include_directories(${TARGETNAME_INCLUDES_FILESYSTEM} INTERFACE
	filesystem
)

add_library(${TARGETNAME_INCLUDES_ENGINE} INTERFACE)

target_include_directories(${TARGETNAME_INCLUDES_ENGINE} INTERFACE
	engine
	engine/common
	engine/common/imagelib
)

target_link_libraries(${TARGETNAME_INCLUDES_ENGINE} INTERFACE
	${TARGETNAME_INCLUDES_FILESYSTEM}
)

add_subdirectory(thirdparty)
add_subdirectory(public)
add_subdirectory(filesystem)
add_subdirectory(engine)
add_subdirectory(game_launch)
