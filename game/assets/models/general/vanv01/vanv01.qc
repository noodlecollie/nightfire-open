$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "vanv01.mdl"

$root "master"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "van"
{
	studio "osato_van_reference_ref"
	studio "osato_van_open_reference_ref"
	studio "osato_van_exploded_reference_ref"
}



$hbox 0 "master" -34.208980 0.000000 -48.097370 14.708040 68.198140 79.542590
$hbox 0 "crate" -62.500000 -21.560000 -10.570000 10.780000 40.130000 31.750000
$hbox 0 "van body" -27.930000 -17.200000 -19.870000 186.010000 69.500000 83.390000
$hbox 0 "windows" -38.930000 -22.950000 -13.520000 38.930000 22.990000 13.060000
$hbox 0 "door" -28.220000 -23.350000 -30.190000 0.000000 23.350000 30.190000

$sequence "idle1" "idle1_ani"
$sequence "explode" "explode_ani"
