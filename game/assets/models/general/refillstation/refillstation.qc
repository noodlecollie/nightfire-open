$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "refillstation.mdl"

$root "refill_StationShape"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "refillstation_ref"
}



$hbox 0 "refill_StationShape" -24.500000 -36.200000 -9.160000 24.500000 36.200000 9.160000

$sequence "idle1" "idle1_ani" ACT_IDLE 1
