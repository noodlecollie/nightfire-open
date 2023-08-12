$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "helicopter_austria_01.mdl"

$root "Dummy01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "helicopter_austria_01_reference_ref"
}
$bodygroup "blades"
{
	studio "helicopter_austria_01_snowblades_ref"
	studio "helicopter_austria_01_blades_ref"
}

$attachment "" "Object08" 0.000000 -215.000000 -40.000000
$attachment "" "Object08" 0.000000 -247.000000 -45.000000


$hbox 0 "Dummy01" -205.636200 -40.918210 -32.532120 321.269000 88.511750 0.160611
$hbox 0 "front_gear_door" -8.780000 -26.690000 -2.410000 8.780000 27.100000 3.360000
$hbox 0 "Object08" -74.390000 -216.410000 -47.300000 74.310000 379.490000 132.710000
$hbox 0 "Object04" -1.620000 -16.400000 -27.690000 1.620000 1.510000 0.570000
$hbox 0 "Object05" -5.950000 -1.360000 -14.980000 5.950000 16.880000 1.220000
$hbox 0 "Object03" -47.950000 -7.310000 -42.880000 47.950000 10.270000 1.570000
$hbox 0 "back_gear_door" -16.960000 -24.480000 -2.440000 81.980000 24.760000 5.380000
$hbox 0 "blades" -254.910000 -264.110000 -52.720000 252.350000 265.790000 24.400000
$hbox 0 "light" -6.590000 -6.830000 -9.060000 6.610000 17.520000 8.680000

$sequence "idle1" "idle1_ani" ACT_IDLE 1
$sequence "fly" "fly_ani" fps 20 {
	event 2001 511 ""
}
