$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "serving_tray.mdl"

$root "tray"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "serving_tray_ref"
}



$hbox 0 "tray" -17.250000 -8.880000 -1.580000 17.250000 8.880000 1.580000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 loop
