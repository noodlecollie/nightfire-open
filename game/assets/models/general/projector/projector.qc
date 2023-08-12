$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "projector.mdl"

$root "default"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "projector_ref_ref"
}



$hbox 0 "default" -11.820000 -10.440000 -5.990000 11.820000 10.900000 5.990000
$hbox 0 "default01" -15.870000 -20.300000 -29.750000 15.870000 21.520000 25.490000

$sequence "idle1" "idle1_ani" fps 20
