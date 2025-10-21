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

Corresponding map editor release: https://github.com/noodlecollie/nightfire-open-editor/releases/tag/v2025.3-nfopen

The main additions to this release are:

* Overhauled multiplayer bot implementation.
	* Many thanks to the open-source [Botrix server plugin](https://www.famaf.unc.edu.ar/~godin/botrix/) for Source games, authored by [Boris Borisovich Godin](https://www.famaf.unc.edu.ar/~godin/about.html), which has been adapted for use with this mod.
	* Bots are now much more sophisticated and are fit for purpose as stand-ins for players.
	* The current implementation is a basic adaptation from the original server plugin, and has some limitations that will be addressed as development proceeds. Some examples include:
		* Bots using sniper rifles is not currently supported.
		* Bots do not target Ronin turrets as enemies.
		* Navigation in some map areas is not smooth.
* Added option in bot configuration menu to automatically fill the server to 60% with bots.
* Relayed bullet hit sounds to the attacker's game client when another player is hit, for better feedback when attacking enemies who are far away.
* Added an in-game acknowledgements menu.
	* This is currently labeled "View Readme" due to limitations with the current in-game menu system. The next release, which will completely replace this menu system with a much more flexible one, will resolve this shortcoming (and many others).
* Fixed in-engine computation of model bounds being broken, which sometimes resulted in models disappearing while they were still partially visible on-screen.
* Fixed some edge cases where player models would disappear where the game would have gibbed them if violence settings had been enabled.
* Fixed multiplayer model hit boxes being assigned completely incorrectly, resulting in headshot damage bonuses being granted on non-head hitboxes, and vice versa.
* Fixed development maps showing up in the multiplayer map selection menu.
* Fixed the crosshair changing colour on map change and becoming difficult to see.
* Fixed an edge case where no spawn location would be found if the loaded map had a single spawn point.
* Various other crash fixes and under-the-hood improvements.
