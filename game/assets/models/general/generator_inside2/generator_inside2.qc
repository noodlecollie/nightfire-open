$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "generator_inside2.mdl"

$root "main"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "generator"
{
	studio "generator_inside2_reference_ref"
	studio "generator_inside2_broken_referen_ref"
}



$hbox 0 "main" -7.820000 -0.040000 -37.380000 21.400000 59.390000 36.250000
$hbox 0 "main_switch" -7.920000 -1.320000 0.000000 7.920000 1.320000 3.690000

$sequence "idle1" "idle1_ani"
$sequence "turn_off" "turn_off_ani"
