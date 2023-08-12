$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "grapple_tip.mdl"

$root "grapple_tip"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "grapple_tip_ref"
}



$hbox 0 "grapple_tip" -9.650000 -3.270000 -2.220000 9.650000 6.310000 2.220000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
