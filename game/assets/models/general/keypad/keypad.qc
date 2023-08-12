$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "keypad.mdl"

$root "default"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "keypad_ref_ref"
}

$attachment "" "default" -1.400000 -2.700000 0.000000


$hbox 0 "default" -4.270000 -1.220000 -3.990000 4.270000 1.220000 3.990000

$sequence "idle" "idle_ani" fps 20
