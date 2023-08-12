$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "nuke_missile.mdl"

$root "master"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "missile"
{
	studio "nuke_missile_reference_ref"
	studio "nuke_missile_open_reference_ref"
	studio "nuke_missile_empty_reference_ref"
	studio "nuke_missile_hanging_reference_ref"
}
$bodygroup "studio"
{
	studio "nuke_missile_rack_reference_ref"
}



$hbox 0 "missile" -23.450000 -168.480000 -139.930000 24.220000 170.520000 0.000000
$hbox 0 "warhead" -13.020000 -38.690000 -13.020000 13.020000 23.410000 13.020000
$hbox 0 "missile_door" 0.000000 -140.200000 -0.350000 27.980000 0.000000 13.590000
$hbox 0 "chains" -1.540000 -27.360000 -99.000000 2.860000 26.320000 3.300000
$hbox 0 "hook" -7.070000 -23.520000 -5.230000 7.070000 10.440000 5.230000
$hbox 0 "rack" -18.350000 -110.960000 -139.790000 19.060000 106.500000 0.000000

$sequence "idle1" "idle1_ani"
$sequence "idle_hanging" "idle_hanging_ani"
