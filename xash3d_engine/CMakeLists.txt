############################################################
#       Common header sets used by multiple targets        #
############################################################

# NFTODO: These should really be managed more cleanly.
# Targets should not be reaching into the headers of other
# targets if they do not link against them. Decouple these
# when possible.

add_library(${TARGETNAME_INCLUDES_ENGINE} INTERFACE)

target_include_directories(${TARGETNAME_INCLUDES_ENGINE} INTERFACE
	engine
	engine/common
	engine/common/imagelib
)

add_subdirectory(thirdparty)
add_subdirectory(filesystem)
add_subdirectory(engine)
add_subdirectory(enginegamesapi)
add_subdirectory(enginerefapi)

if(NOT NF_SINGLE_ENGINE_BINARY)
	add_subdirectory(game_launch)
endif()
