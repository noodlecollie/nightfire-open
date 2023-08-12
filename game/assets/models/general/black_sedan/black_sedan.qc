$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "black_sedan.mdl"

$root "mercedes"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "cars"
{
	studio "sedan_black_reference_ref"
	studio "black_sedan_exploded_reference_ref"
}



$hbox 0 "mercedes" -57.990000 -107.570000 -3.930000 51.820000 125.880000 67.730000

$sequence "idle1" "idle1_ani"
$sequence "explode" "explode_ani"
