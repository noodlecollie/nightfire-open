$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "air_lightv01.mdl"

$root "light"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "lights"
{
	studio "air_light_red_lod0_ref"
	studio "air_light_red_lod1_ref"
	studio "air_light_red_lod2_ref"
	studio "air_light_white_lod0_ref"
	studio "air_light_white_lod1_ref"
	studio "air_light_white_lod2_ref"
}



$hbox 0 "light" -2.390000 -2.710000 0.000000 2.390000 2.710000 42.010000

$sequence "idle1" "idle1_ani"
