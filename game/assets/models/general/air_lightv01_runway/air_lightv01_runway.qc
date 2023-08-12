$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "air_lightv01_runway.mdl"

$root "lod0"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "lights"
{
	studio "air_light_red_runway_lod0_ref"
	studio "air_light_red_runway_lod1_ref"
	studio "air_light_red_runway_lod2_ref"
	studio "air_light_red_runway_lod3_ref"
	studio "air_light_white_runway_lod0_ref"
	studio "air_light_white_runway_lod1_ref"
	studio "air_light_white_runway_lod2_ref"
	studio "air_light_white_runway_lod3_ref"
}



$hbox 0 "lod0" -2.420000 -466.710000 -15.930000 2.400000 466.710000 26.010000
$hbox 0 "lod1" -2.390000 -466.710000 -15.930000 2.390000 466.710000 26.010000
$hbox 0 "lod2" -2.390000 -466.710000 -15.930000 2.390000 466.710000 25.480000
$hbox 0 "lod3" -2.390000 -466.710000 -15.930000 2.390000 466.710000 25.480000

$sequence "idle1" "idle1_ani"
