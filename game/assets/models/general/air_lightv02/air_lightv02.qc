$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "air_lightv02.mdl"

$root "LIGHT"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "air_light_double_red_lod0_ref"
	studio "air_light_double_red_lod1_ref"
	studio "air_light_double_red_lod2_ref"
	studio "air_light_double_red_lod3_ref"
	studio "air_light_double_white_lod0_ref"
	studio "air_light_double_white_lod1_ref"
	studio "air_light_double_white_lod2_ref"
	studio "air_light_double_white_lod3_ref"
}



$hbox 0 "LIGHT" -3.310000 -7.980000 0.000000 3.240000 8.040000 54.910000

$sequence "idle1" "idle1_ani"
