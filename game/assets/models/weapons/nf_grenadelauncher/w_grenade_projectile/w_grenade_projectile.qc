$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "w_grenade_projectile.mdl"

$root "shell"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "w_grenade_projectile_ref"
}



$hbox 0 "shell" -1.560000 -2.690000 0.000000 1.520000 3.850000 3.010000

$sequence "idle1" "idle1_ani" fps 1
