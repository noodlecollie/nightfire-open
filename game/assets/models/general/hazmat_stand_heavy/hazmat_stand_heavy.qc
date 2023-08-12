$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "hazmat_stand_heavy.mdl"

$root "hazmat_heavy"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "hazmat_stand_heavy_ref_ref"
}



$hbox 0 "hazmat_heavy" -11.980000 -16.680000 0.000000 11.970000 7.980000 70.410000
$hbox 0 "default" -12.690000 -10.990000 -36.450000 12.690000 10.990000 36.450000
$hbox 0 "boot" -6.010000 -9.400000 -0.110000 7.270000 4.380000 11.070000
$hbox 0 "gloves" -4.860000 -4.530000 -1.830000 4.480000 4.420000 2.570000

$sequence "idle1" "idle1_ani" loop
