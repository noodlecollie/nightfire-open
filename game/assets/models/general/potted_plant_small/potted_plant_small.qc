$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "potted_plant_small.mdl"

$root "Cylinder01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "potted_plant_small_ref_ref"
}



$hbox 0 "Cylinder01" -8.280000 -8.280000 -14.650000 8.280000 8.280000 0.000000
$hbox 0 "default" -15.700000 -15.920000 -15.040000 10.720000 11.140000 15.040000

$sequence "idle" "idle_ani" fps 20
