$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "w_raptor.mdl"

$root "gun"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "w_raptor_reference_ref"
}



$hbox 0 "gun" -7.780000 -5.690000 -0.110000 6.830000 2.110000 1.530000

$sequence "idle1" "idle1_ani"
