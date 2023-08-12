$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "w_p99.mdl"

$root "gun"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "w_p99_reference_ref"
}



$hbox 0 "gun" -4.600000 -3.670000 0.000000 4.800000 2.890000 1.290000

$sequence "idle1" "idle1_ani" fps 1
