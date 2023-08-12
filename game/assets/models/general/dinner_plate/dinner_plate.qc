$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "dinner_plate.mdl"

$root "plate"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "dinner_plate_ref"
}



$hbox 0 "plate" -8.780000 -7.770000 -0.940000 8.780000 7.770000 0.930000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 loop
