$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "vase_plant.mdl"

$root "vase"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "vase_plant_ref"
}



$hbox 0 "vase" -7.500000 -7.800000 -0.820000 7.500000 7.800000 23.410000
$hbox 0 "plant" -17.920000 -18.580000 -0.110000 16.100000 14.270000 23.090000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 loop
