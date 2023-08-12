$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "fuel_line.mdl"

$root "fuel_lines_1"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "fuel_lines_up_ref"
	studio "fuel_lines_down_ref"
}



$hbox 0 "fuel_lines_1" -109.460000 -88.540000 -199.060000 0.000000 382.180000 642.580000
$hbox 0 "fuel_lines_2" -97.210000 -88.540000 0.000000 0.000000 421.200000 642.580000

$sequence "idle1" "idle1_ani"
