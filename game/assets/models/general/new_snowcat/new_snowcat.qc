$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "new_snowcat.mdl"

$root "default"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "new_snowcat_ref_ref"
}
$bodygroup "plow"
{
	blank
	studio "new_snowcat1_ref_ref"
}



$hbox 0 "default" -89.970000 -158.933200 -72.030000 89.510000 136.270000 56.000000
$hbox 0 "plow" -75.710000 -63.180000 -33.850000 75.750000 44.680000 16.220000

$sequence "idle1" "idle1_ani" fps 20
