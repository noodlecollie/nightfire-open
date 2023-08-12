$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "w_ammo_raptor.mdl"

$root "Ammo_box03"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "w_ammo_raptor_REFERENCE_ref"
}



$hbox 0 "Ammo_box03" -4.980000 -7.440000 -0.090000 7.620000 5.820000 13.040000

$sequence "idle1" "idle1_ani"
