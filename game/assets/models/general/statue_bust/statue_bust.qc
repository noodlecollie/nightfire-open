$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "statue_bust.mdl"

$root "statue_bust"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "main"
{
	studio "statue_bust_lod0_ref"
	studio "statue_bust_lod1_ref"
	studio "statue_bust_lod2_ref"
	studio "statue_bust_broken_ref"
}



$hbox 0 "statue_bust" -8.590000 -7.790000 0.000000 8.590000 7.790000 23.930000

$sequence "idle1" "idle1_ani"
