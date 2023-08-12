$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "Monitor1.mdl"

$root "Monitor 1 Sides"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "Monitor1_ref"
}



$hbox 0 "Monitor 1 Sides" -12.330000 -11.630000 0.000000 12.210000 0.000000 21.440000
$hbox 0 "Monitor 1 Screen" -9.920000 -12.250000 0.000000 9.790000 0.000000 19.380000
$hbox 0 "Monitor 1 Stand" -2.770000 -7.440000 0.000000 2.900000 7.440000 11.690000
$hbox 0 "Monitor1 bezil" -12.330000 -12.590000 0.000000 12.210000 0.000000 21.370000
$hbox 0 "Monitor 1 Top" -15.620000 -8.850000 0.000000 0.400000 8.830000 13.770000

$sequence "idle1" "idle1_ani"
