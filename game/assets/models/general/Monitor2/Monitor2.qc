$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "Monitor2.mdl"

$root "Monitor 2 Sides"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "Monitor2_ref"
}



$hbox 0 "Monitor 2 Sides" -6.130000 -7.040000 0.000000 6.070000 0.000000 10.490000
$hbox 0 "Monitor 2 Screen" -4.930000 -6.480000 0.000000 4.870000 0.000000 9.490000
$hbox 0 "Monitor 2 bezil" -6.130000 -7.220000 0.000000 6.070000 0.000000 10.490000
$hbox 0 "Monitor 2 Back" -6.020000 -5.610000 0.000000 5.960000 0.000000 9.960000
$hbox 0 "Monitor 2 Stand" -3.390000 -5.610000 0.000000 3.110000 0.820000 9.360000
$hbox 0 "Monitor 2 Controls" -9.200000 -5.180000 0.000000 0.000000 0.000000 10.490000

$sequence "idle1" "idle1_ani"
