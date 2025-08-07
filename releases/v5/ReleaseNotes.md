This release contains:

* Three multiplayer maps:
	* DM Island
	* DM Japan
	* DM Power
* Three player models:
	* MI6 Stealth
	* MI6 Suit
	* MI6 Tux
* Eight weapons:
	* Fists
	* Frinesi (shotgun)
	* Militek (grenade launcher)
	* L96A1 (sniper)
	* P99 (pistol)
	* Raptor (handgun)
	* Storm (sub-machine gun)
	* Frag grenade

The main additions to this release are:

* Weapon behaviour and damage changes:
	* Tweaked Frinesi inaccuracy curve.
		* Crouching now provides slightly less accuracy.
		* The inaccuracy decay after firing has been extended.
		* The crosshair max radius has been increased to more accurately represent the spread.
	* Increased Raptor damage from 30 to 50.
	* Slightly increased Raptor accuracy.
	* Decreased L96A1 damage from 110 to 90.
	* Reduced Militek base damage from 100 to 75.
	* Increased Militek damage multiplier when directly hitting other players from 1.25x to 1.6x.
	* Reduced Militek timed grenade fuse from 4 seconds to 2 seconds.
	* Reduced frag grenade contact friction from 0.95 to 0.7. This means that grenades bounce more.
	* Reduced frag grenade fuse time from 4 seconds to 2 seconds.
	* Reduced frag grenade pin pull time from 1 second to 0.75 seconds.
	* Increased frag grenade refire delay from 0.5 seconds to 0.91 seconds.
* Tweaked player movement speeds:
	* Reduced player forward/backward speed from 280 to 250.
	* Reduced player strafe speed from 240 to 215.
* Increased bullet tracer speed, to improve the feel of firing shots. This is a graphical change only - shots register on the same frame as they are fired, as has always been the case.
* Increased health points gained from health kits from 15 to 50.
* Added support for client-side props.
	* All maps now contain their simple static props that used to be specified as `item_generic` entities in original Nightfire. These props are now handled entirely client-side, so that they do not take up entity slots on the server.
* Fixed the player sticking to surfaces with a magnetic-like feeling when noclipping.
* Fixed an issue with client-side weapon prediction where weapons would not play their deploy animations when first picked up. Weapon prediction is still disabled by default as it does not fully work yet.
* Lag compensation has been enabled - the fact that this was disabled was an error. Shots should feel much more accurate now.
* Stopped fallback model from being selectable as a multiplayer model.
* Updated all player model animations to those from original Nightfire.
* General internal improvements.
