$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "fax.mdl"

$root "Box01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "fax_ref"
}



$hbox 0 "Box01" -11.270000 -13.500000 -0.950000 8.470000 8.340000 6.200000

$sequence "fax_in" "fax_in_ani" fps 25 loop
$sequence "on" "on_ani" fps 35
$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
