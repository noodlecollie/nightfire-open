$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "keyboard_2.mdl"

$root "master"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "keyboard"
{
	studio "keyboard_2_ref"
	studio "keyboard_2_ref"
	studio "keyboard_2_broken_ref"
}



$hbox 0 "Object01" -25.560000 -3.180000 -1.280000 1.410000 12.720000 0.890000
$hbox 0 "keyboard" -11.930000 -5.360000 -0.880000 11.580000 4.600000 0.520000

$sequence "idle1" "idle1_ani"
$sequence "shot" "shot_ani"
