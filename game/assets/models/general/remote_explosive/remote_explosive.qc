$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "remote_explosive.mdl"

$root "default"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "remote_explosive_ref_ref"
}



$hbox 0 "default" -6.470000 -2.480000 -4.810000 6.470000 2.850000 4.810000
$hbox 0 "cover" -5.560000 -0.020000 -2.350000 5.560000 2.330000 2.350000

$sequence "idle1" "idle1_ani" fps 20
$sequence "activated" "activated_ani" fps 60
