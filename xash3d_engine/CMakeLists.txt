############################################################
#       Common header sets used by multiple targets        #
############################################################

# NFTODO: These should really be managed more cleanly.
# Targets should not be reaching into the headers of other
# targets if they do not link against them. Decouple these
# when possible.

set(CMAKE_INSTALL_RPATH "\$ORIGIN")

add_subdirectory(thirdparty)
add_subdirectory(filesystem)
add_subdirectory(engine)
add_subdirectory(enginepublicapi)
add_subdirectory(engineinternalapi)

if(NOT NF_SINGLE_ENGINE_BINARY)
	add_subdirectory(game_launch)
endif()
