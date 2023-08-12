$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "japanese_scroll1.mdl"

$root "scroll1"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "japanese_scroll1_ref"
}



$hbox 0 "scroll1" -10.730000 -0.570000 -51.310000 10.730000 0.570000 0.060000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
$sequence "hit" "hit_ani"
