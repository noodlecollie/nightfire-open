$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "box_fan.mdl"

$root "default01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "box_fan_ref_ref"
}



$hbox 0 "default01" -4.610000 -4.490000 -2.440000 9.690000 0.920000 14.370000
$hbox 0 "Object02" -13.580000 -4.990000 -14.370000 13.580000 4.990000 14.370000
$hbox 0 "Object01" -10.470000 -1.900000 -10.670000 10.470000 1.900000 10.670000

$sequence "idle1" "idle1_ani" fps 20 loop
