$cd .
$cdtexture "../../../../../content/nfopen/textures/mdl"
$modelname "w_mp9.mdl"

$root "W_GUN"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

$bodygroup "studio"
{
	studio "W_MP9_reference_ref"
}

$hbox 0 "W_GUN" -0.450000 -10.030000 -7.890000 1.540000 8.240000 3.710000

$sequence "idle" "idle_ani"
