$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "japanese_lamps.mdl"

$root "Cylinder01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "lamps"
{
	studio "japanese_lamp01_ref"
}



$hbox 0 "Cylinder01" -5.470000 -5.470000 0.000000 5.470000 5.470000 9.200000
$hbox 0 "Cylinder02" -0.930000 -0.800000 0.000000 0.930000 0.800000 3.470000
$hbox 0 "Box01" -1.490000 -1.490000 -0.250000 1.490000 1.490000 4.150000
$hbox 0 "Cylinder03" -5.360000 -5.360000 0.000000 5.360000 5.360000 8.270000

$sequence "idle1" "idle1_ani"
