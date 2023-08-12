$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "vane.mdl"

$root "vane_fancy"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "vanes"
{
	studio "vane_fancy_ref"
	studio "vane_simple_ref"
}



$hbox 0 "vane_fancy" -19.800000 -192.440000 -60.080000 19.800000 149.370000 78.700000
$hbox 0 "vane_simple" -9.990000 -185.790000 -9.990000 9.980000 114.600000 9.980000

$sequence "idle1" "idle1_ani"
