$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "austria_globe.mdl"

$root "STAND"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "table"
{
	studio "globe_lod0_ref"
	studio "globe_lod1_ref"
	studio "globe_lod2_ref"
}



$hbox 0 "STAND" -15.630000 -15.630000 -0.060000 15.630000 15.630000 40.090000
$hbox 0 "GLOBE" -11.910000 -11.910000 -11.910000 11.910000 11.910000 11.910000

$sequence "idle1" "idle1_ani"
$sequence "used" "used_ani"
