$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "w_grenadelauncher.mdl"

$root "gun"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "w_grenadelauncher_reference_ref"
}



$hbox 0 "gun" -0.110000 -15.880000 -6.350000 7.120000 15.880000 6.350000

$sequence "idle1" "idle1_ani"
