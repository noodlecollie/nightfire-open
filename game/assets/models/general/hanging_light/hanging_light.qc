$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "hanging_light.mdl"

$root "HanginLamp"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "hanging_light_ref"
}



$hbox 0 "HanginLamp" -6.400000 -7.120000 -36.480000 6.160000 7.380000 13.050000

$sequence "idle1" "idle1_ani" ACT_IDLE 1
