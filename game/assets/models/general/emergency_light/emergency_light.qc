$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "emergency_light.mdl"

$root "default"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "emergency_light_ref_ref"
}



$hbox 0 "default" -3.850000 -3.420000 -2.500000 3.850000 3.420000 2.500000
$hbox 0 "default01" 0.000000 -1.430000 -1.620000 2.460000 1.470000 1.620000

$sequence "idle1" "idle1_ani" loop
$sequence "animation" "animation_ani" loop
