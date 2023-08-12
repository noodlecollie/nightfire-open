$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "cargo_truck.mdl"

$root "default"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "cargo_truck_ref_ref"
}



$hbox 0 "default" -42.830000 -127.060000 0.000000 42.830000 127.060000 105.910000
$hbox 0 "front wheels01" -40.530000 -20.300000 -20.470000 40.530000 20.300000 20.470000
$hbox 0 "front wheels" -40.530000 -20.300000 -20.470000 40.530000 20.300000 20.470000
$hbox 0 "front wheels02" -40.530000 -20.300000 -20.470000 40.530000 20.300000 20.470000

$sequence "idle1" "idle1_ani" fps 20
$sequence "driving" "driving_ani" fps 20
