$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "w_rocket.mdl"

$root "origin"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "w_rocket_reference_ref"
}

$attachment "" "rocket" 0.000000 0.000000 0.000000


$hbox 0 "rocket" -3.160000 -26.420000 -3.160000 3.170000 0.320000 3.160000

$sequence "idle0" "idle0_ani" fps 20
$sequence "idle1" "idle1_ani" fps 20
$sequence "idle2" "idle2_ani" fps 20
$sequence "idle3" "idle3_ani" fps 20
$sequence "idle4" "idle4_ani" fps 20
