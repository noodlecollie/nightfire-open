$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "ergo_chair.mdl"

$root "ergo_chair"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "ergo_chair_ref"
}



$hbox 0 "ergo_chair" -15.740000 -5.770000 -0.320000 15.740000 27.060000 49.130000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
$sequence "hit" "hit_ani"
