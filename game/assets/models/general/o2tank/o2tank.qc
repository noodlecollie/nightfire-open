$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "o2tank.mdl"

$root "tank"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "tank"
{
	studio "o2tank_ref"
	studio "dam_o2tank_ref"
}



$hbox 0 "tank" -20.590000 -20.970000 -67.760000 20.390000 19.980000 67.260000

$sequence "idle1" "idle1_ani" ACT_IDLE 1
