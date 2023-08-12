$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "austria_exterior_light.mdl"

$root "Object01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "lights"
{
	studio "exterior_light_01_reference_ref"
	studio "exterior_light_02_reference_ref"
	studio "exterior_light_03_reference_ref"
	studio "interior_light_01_reference_ref"
	studio "interior_light_02_reference_ref"
	blank
	blank
}



$hbox 0 "Object01" -8.770000 -8.030000 -14.390000 8.850000 16.040000 20.840000

$sequence "idle1" "idle1_ani"
