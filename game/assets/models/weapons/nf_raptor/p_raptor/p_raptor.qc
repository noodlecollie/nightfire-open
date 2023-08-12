$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "p_raptor.mdl"

$root "Bip01"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

$bodygroup "studio"
{
	studio "p_raptor_REFERENCE_ref"
}

$hbox 0 "gun" -10.660000 -2.130000 -0.090000 2.380000 4.840000 1.370000

$sequence "idle1" "idle1_ani"
