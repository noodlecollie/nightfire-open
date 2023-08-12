$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "carv01.mdl"

$root "exterior"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "cars"
{
	studio "carv02_ref_ref"
	studio "carv02-damaged_ref_ref"
}



$hbox 0 "exterior" -38.200000 -92.830000 0.000000 38.030000 81.100000 62.370000

$sequence "idle1" "idle1_ani"
$sequence "explode1" "explode1_ani"
