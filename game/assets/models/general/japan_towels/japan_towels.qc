$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "japan_towels.mdl"

$root "Bone01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "main"
{
	studio "japan_towels_ref"
}



$hbox 1 "Bone01" -10.000000 -4.000000 -30.000000 10.000000 4.000000 0.000000

$sequence "idle" "idle_ani" loop
$sequence "swing" "swing_ani" loop
