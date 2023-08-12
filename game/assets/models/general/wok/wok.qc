$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "wok.mdl"

$root "wok"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "wok_ref"
}



$hbox 0 "wok" -13.706930 -9.980000 0.000000 11.100000 8.760000 4.210000
$hbox 0 "woklid" -9.820000 -9.520000 -0.260000 8.470000 9.510000 7.340000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
