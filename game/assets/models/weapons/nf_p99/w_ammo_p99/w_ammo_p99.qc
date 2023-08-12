$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "w_ammo_p99.mdl"

$root "ammo"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

$bodygroup "studio"
{
	studio "w_ammo_p99_REFERENCE_ref"
}

$hbox 0 "ammo" -2.560000 -2.700000 0.000000 3.850000 3.030000 2.320000

$sequence "idle1" "idle1_ani"
