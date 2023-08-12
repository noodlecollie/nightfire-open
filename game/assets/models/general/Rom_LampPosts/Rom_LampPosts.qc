$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "Rom_LampPosts.mdl"

$root "Lamppost"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "Rom_Lights"
{
	studio "Rom_LampPost_Four_ref"
	studio "Rom_LampPost_Single_ref"
}



$hbox 0 "Lamppost" -39.760000 -38.910000 -116.520000 39.760000 40.400000 121.700000

$sequence "idle1" "idle1_ani" ACT_IDLE 1
