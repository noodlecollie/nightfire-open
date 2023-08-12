$cd .
$cdtexture "../../../../../afterburner/textures"

$modelname "w_frinesi.mdl"

$root "SHOTGUN"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

$bodygroup "body"
{
	studio "w_frinesi_reference_ref"
}

$hbox 0 "SHOTGUN" -18.750000 -6.530000 -0.170000 15.030000 2.810000 2.260000

$sequence "idle1" "idle1_ani" fps 1
