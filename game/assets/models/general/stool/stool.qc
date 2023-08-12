$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "stool.mdl"

$root "stool"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "stools"
{
	studio "stool_ref"
	studio "stool2_ref"
}



$hbox 0 "stool" -7.520000 -7.500000 -17.340000 7.540000 7.560000 17.340000
$hbox 0 "stool2" -6.900000 -6.880000 -7.630000 6.700000 6.520000 8.480000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
