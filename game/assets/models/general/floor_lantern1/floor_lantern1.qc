$cdtexture "../../../../content/nfopen/textures"
$cd "."
$modelname "floor_lantern1.mdl"
$noembeddedtextures

$root "floor lantern1"

$flags 0
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "floor_lantern1_ref"
}



$hbox 0 "floor lantern1" -5.840000 -5.840000 0.000000 5.840000 5.840000 43.550000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
$sequence "hit" "hit_ani" fps 33
