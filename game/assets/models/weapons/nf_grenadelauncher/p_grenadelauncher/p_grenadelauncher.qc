$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "p_grenadelauncher.mdl"

$root "Bip01"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "p_grenadelauncher_REFERENCE_ref"
}



$hbox 0 "w_gun" -16.330000 -6.530000 -3.720000 16.320000 6.540000 3.720000

$sequence "idle1" "idle1_ani"
