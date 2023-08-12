$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "chandelier.mdl"

$root "chain"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "chandelier"
{
	studio "chandelier_small_lod0_REFERENCE_ref"
	studio "chandelier_small_lod1_REFERENCE_ref"
	studio "chandelier_large_lod0_REFERENCE_ref"
	studio "chandelier_large_lod1_REFERENCE_ref"
}



$hbox 0 "top" -25.000000 -25.000000 -65.000000 25.000000 25.000000 -15.000000

$sequence "idle1" "idle1_ani"
$sequence "shot" "shot_ani"
