$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "scuba_pickup.mdl"

$root "default"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "scuba_pickup_ref_ref"
}



$hbox 0 "default" -10.630000 -9.440000 -14.020000 10.630000 9.440000 14.020000
$hbox 0 "tank" -12.020000 -14.450000 -16.560000 12.020000 14.450000 16.560000

$sequence "idle1" "idle1_ani" fps 20
