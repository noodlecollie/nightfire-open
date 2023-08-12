$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "radar_dishstand.mdl"

$root "default"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "radar_dishstand_ref_ref"
}



$hbox 0 "default" -45.620000 -29.610000 -58.930000 45.620000 29.610000 58.930000
$hbox 0 "default01" -45.620000 -2.970000 -25.210000 45.620000 2.970000 25.210000

$sequence "idle1" "idle1_ani" fps 20
