$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "tower_holo.mdl"

$root "Dummy01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "tower_holo_ref"
}



$hbox 0 "hologram" -25.690000 -57.380000 0.000000 25.690000 38.130000 107.330000

$sequence "idle1" "idle1_ani" loop
