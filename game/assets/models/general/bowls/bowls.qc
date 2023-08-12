$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "bowls.mdl"

$root "bowl"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "bowls"
{
	studio "red_bowl_ref"
	studio "wood_bowl_ref"
}



$hbox 0 "bowl" -3.760000 -3.760000 0.000000 3.760000 3.760000 2.120000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
