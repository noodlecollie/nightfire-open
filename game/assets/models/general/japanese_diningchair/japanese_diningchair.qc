$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "japanese_diningchair.mdl"

$root "ergo_chair"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "japanese_diningchair_ref"
}



$hbox 0 "ergo_chair" -14.020000 -13.890000 -0.200000 14.020000 13.890000 57.140000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
$sequence "hit" "hit_ani"
