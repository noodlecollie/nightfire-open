$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "c4_explosives.mdl"

$root "Object02"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "bomb"
{
	studio "c4_explosives_reference_ref"
	studio "c4_explosives_armed_reference_ref"
}



$hbox 0 "Object02" -5.590000 -4.100000 -0.060000 5.460000 7.970000 3.610000

$sequence "idle1" "idle1_ani"
$sequence "planted" "planted_ani"
