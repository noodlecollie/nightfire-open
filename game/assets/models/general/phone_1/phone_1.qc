$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "phone_1.mdl"

$root "phone_1"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "main"
{
	studio "phone_1_ref"
}



$hbox 0 "phone_1" -6.510000 -5.620000 -0.010000 6.390000 5.600000 3.560000
$hbox 0 "handle" -4.270000 -5.140000 0.000000 5.850000 0.000000 4.580000

$sequence "idle1" "idle1_ani"
$sequence "shot" "shot_ani"
