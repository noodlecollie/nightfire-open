$cdtexture "../../../../content/nfopen/textures"
$cd "."
$modelname "osato_chair.mdl"
$noembeddedtextures

$root "osato_chair"

$flags 0
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "chair"
{
	studio "osato_chair_lod0_ref"
	studio "osato_chair_lod1_ref"
	studio "osato_chair_lod2_ref"
	studio "osato_chair_exploded_ref"
}



$hbox 0 "osato_chair" -12.000000 -15.000000 45.000000 12.000000 12.000000 0.000000

$sequence "idle1" "idle1_ani"
