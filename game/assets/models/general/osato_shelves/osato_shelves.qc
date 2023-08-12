$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "osato_shelves.mdl"

$root "shelves_base"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "shelves"
{
	studio "osato_shelves_ref"
	studio "osato_shelves_top_1_ref"
	studio "osato_shelves_top_2_ref"
}



$hbox 0 "shelves_base" -26.860000 -7.300000 0.000000 26.850000 7.160000 39.590000
$hbox 0 "shelves_top_1" -26.400000 -6.560000 0.000000 26.390000 6.630000 78.630000
$hbox 0 "shelves_top_2" -26.400000 -6.560000 0.000000 26.390000 6.630000 78.630000

$sequence "idle1" "idle1_ani"
