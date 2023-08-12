$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "japanese_sandals.mdl"

$root "sandals1"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "sandals"
{
	studio "sandals1_ref"
	studio "sandals2_ref"
}



$hbox 0 "sandals1" -8.020000 -7.100000 0.000000 8.020000 7.100000 3.760000
$hbox 0 "sandals2" -7.990000 -9.180000 -1.360000 7.990000 9.180000 1.360000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
