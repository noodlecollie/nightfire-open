$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "ash_tray.mdl"

$root "ash_tray"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "main"
{
	studio "ash_tray_ref"
}



$hbox 0 "ash_tray" -7.050000 -6.700000 0.000000 7.050000 6.700000 34.620000

$sequence "idle1" "idle1_ani"
$sequence "shot" "shot_ani"
