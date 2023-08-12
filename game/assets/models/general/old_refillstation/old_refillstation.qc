$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "old_refillstation.mdl"

$root "Refill_Station"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "old_refillstation_ref"
}



$hbox 0 "Refill_Station" -16.760000 -13.960000 -0.130000 16.710000 14.110000 75.950000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
