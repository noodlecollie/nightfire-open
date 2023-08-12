$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "shrubv01.mdl"

$root "bush1"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "shrubv01_ref_ref"
	studio "shrubv02_ref_ref"
	studio "shrubv03_ref_ref"
	studio "shrubv04_ref_ref"
}



$hbox 0 "bush1" -16.100000 -13.470000 -15.900000 13.320000 13.570000 10.180000
$hbox 0 "bush02" -25.920000 -21.680000 -25.610000 21.440000 21.860000 16.390000
$hbox 0 "bush03" -37.070000 -31.010000 -36.620000 30.670000 31.260000 23.450000
$hbox 0 "bush04" -48.930000 -40.930000 -48.180000 40.480000 41.260000 31.120000

$sequence "idle" "idle_ani" fps 20
