$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "w_ammo_grenadelauncher.mdl"

$root "ammo"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

$bodygroup "studio"
{
	studio "w_ammo_grenadelauncher_REFERENCE_ref"
}

$hbox 0 "ammo" -3.360000 -4.220000 -0.010000 3.370000 4.330000 8.540000

$sequence "idle1" "idle1_ani"
