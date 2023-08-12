$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "desk_lamp.mdl"

$root "desk_lamp"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "main"
{
	studio "desk_lamp_ref"
}



$hbox 0 "desk_lamp" -5.870000 -25.880000 0.000000 5.870000 7.020000 21.310000

$sequence "idle1" "idle1_ani"
$sequence "shot" "shot_ani"
