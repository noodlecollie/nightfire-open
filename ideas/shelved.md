The following features or ideas have been shelved for now, due to the reasons described.

# Server-Side Gait and Blend Processing

Currently, hit detection does not take into account the pose of the player's arms or legs. This is because the calculation of these poses (the player's "gait") is performed entirely client-side for visual purposes, and relies on variables that are not taken into account when posing bones for models on the server.

The engine itself does allow the model blending interface for posing bones to be overridden by defining a Server_GetBlendingInterface() function in the server DLL. If the engine can look up this function, it uses it instead of the standard bone setup procedure. This does, however, mean that you have to do everything yourself in this function, resulting in a lot of duplicated code.

This function was used to attempt to allow gait processing in the server DLL. The standard bone setup is fairly straightforward, but when gait and blend processing are added things start to get a bit complicated. Specifically, the process requires at least the following data to be maintained for the current and previous frames:

* The player origin
* The player angles
* 4x blend values
* 2x sequence blend values
* 4x controller values
* The current server time

A lot of these values are maintained and networked using the 	`latchedvars_t` structure, but this is all done at the engine level rather than the game libs level, and it's not clear how to replicate the information exactly in the game libraries (or at least, it would be significantly complicated). Therefore, this feature has been shelved for now.

The ideal way to solve the blending/gait issue would be for the values to be calculated on the server, and networked/predicted for the client. However, this would require significant changes to the engine code.

# Importing Nightfire BSP Files

Sub-models in Half-Life BSPs (eg. brush entities like doors) are partially integrated into the map's main BSP tree, whereas Nightfire sub-models are not. This makes it impossible to load Nightfire BSPs without making significant changes to the BSP structure.

The technical reason for this is that the engine expects the surfaces used by the sub-model to be present somewhere in the main BSP tree. It uses this sub-tree as the primary hull (hull 0) when performing collision detection with the sub-model, and expects hulls 0-3 to be defined additionally with clip nodes.

The original plan for converting a Nightfire BSP was to generate clip node trees manually and point sub-models to these trees. However, the engine uses the clip node index for hull 0 as an index into the main BSP tree; since the clip nodes are generated in addition to the main BSP tree, the index of the first clip node is out of range of the BSP tree nodes and causes an engine crash.

Instead of loading Nightfire BSPs directly, we just decompile and recompile them into a format compatible with the GoldSrc engine.
