$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "generator_inside.mdl"

$root "main"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "generator"
{
	studio "generator_inside_reference_ref"
	studio "generator_broken_reference_ref"
}



$hbox 0 "main" -43.820000 0.000000 -49.380000 0.000000 75.390000 24.250000
$hbox 0 "main_switch" -7.920000 -1.320000 0.000000 7.920000 1.320000 3.690000

$sequence "idle1" "idle1_ani"
$sequence "turn_off" "turn_off_ani"
