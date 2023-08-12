$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "w_ammo_shotgun.mdl"

$root "ammo"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

$bodygroup "studio"
{
	studio "w_ammo_shotgun_REFERENCE_ref"
}

$hbox 0 "ammo" -4.570000 -6.770000 -0.020000 8.190000 4.460000 12.670000

$sequence "idle1" "idle1_ani"
