This release contains:

* Four multiplayer maps:
	* DM Island
	* DM Japan
	* DM Power
	* DM Casino
* All multiplayer models from the original game
* Nine weapons:
	* Fists
	* Frinesi (shotgun)
	* Militek (grenade launcher)
	* L96A1 (sniper)
	* P99 (pistol)
	* Raptor (handgun)
	* Storm (sub-machine gun)
	* Frag grenade
	* Phoenix Ronin (deployable turret)

Corresponding map editor release: https://github.com/noodlecollie/nightfire-open-editor/releases/tag/v2025.2-nfopen

The main additions to this release are:

* Added Phoenix Ronin as a weapon.
	* Primary attack throws the Ronin. Secondary attack places it on the ground in front of the player.
	* A thrown Ronin can be deployed using a subsequent primary attack, or detonated using a subsequent secondary attack.
	* A thrown Ronin can target enemies in 360 degrees around itself. A placed Ronin targets enemies in a view cone in front of itself.
	* Invoking `+use` on a deployed Ronin will re-pack it, allowing the turret to be picked back up.
	* A killed Ronin disables itself, begins sparking, and eventually explodes.
	* Further improvements to the weapon's abilities are planned for a future release.
* Added all multiplayer models from original Nightfire, including different pain and death noises depending on gender.
* Added model viewer to engine. This can be accessed by launching with `+menu_modelviewer` on the command line, or by executing `menu_modelviewer` in the console.
* The option for online multiplayer matches has been removed for now. LAN multiplayer is still available.
	* Depending on the Xash master servers was not desirable, since the game should ideally have its own multiplayer infrastructure.
	* In the game's current state, it is not worth maintaining a master server that will go largely unused.
	* In future, online multiplayer may be re-enabled once infrastructure is in place.
* Allowed masked transparent textures to be used on models.
* Fixed screenshots not saving properly. These are now placed in the `scrshot` directory under the game's root.
* Tuned the entity lighting range for weapon muzzle flashes.
* Removed Half Life weapons. These were previously accessible using the `give` command, or `impulse 101`.
* Updated static models to support custom lighting origins, so that they no longer render very bright or dark.
* Added proper RPATH on Linux, so that launching the game executable via a script file is no longer required.
* The Nightfire Open fork of TrenchBroom has been updated to include the latest application functionality. See: [v2025.2-nfopen](https://github.com/noodlecollie/nightfire-open-editor/releases/tag/v2025.2-nfopen)
* Other internal codebase improvements.
