# Classes and Prediction

Weapons can either be run 100% on the server, with events sent to the client to notify when certain actions happen, or can be simulated on the client using weapon prediction. In order for weapons to be predicted, the `CLIENT_WEAPONS` preprocessor define must exist (which it does by default), and the `cl_lw` convar must be set to 1.

When weapon prediction is enabled, weapon firing code is run on the client in order to make the weapon appear to act as though there is no server-client latency. The server, however, is still authoritative on whether shots hit or miss their targets. For more information on the concept of prediction, see [this page](https://developer.valvesoftware.com/wiki/Prediction##) on the Valve developer wiki.

When weapon prediction is enabled, `HUD_WeaponsPostThink()` is called from `HUD_PostRunCmd()`. This will in turn call `ItemPostFrame()` on the client's current weapon, and client-only code in the weapon class will be executed.

# Sounds
Weapon sounds that matter to players (eg. shots) should be emitted by the server, either at a static location or following the player. Weapon sounds that are less important (eg. draw/holster sounds) should be played only to the client. This can be achieved by using StudioMDL events - event `5004` emits a clientside sound from a point in the world, and the custom event `5005` emits a sound without having it located in the world.
