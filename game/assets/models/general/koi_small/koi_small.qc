$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "koi_small.mdl"

$root "Bone01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "kio"
{
	studio "koi_large_lod0_ref"
	studio "koi_large_lod1_ref"
	studio "koi_large_lod2_ref"
	studio "koi_large_lod3_ref"
	blank
}



$hbox 0 "Bone01" -3.210000 -0.710000 -0.560000 0.620000 0.700000 1.720000
$hbox 0 "Bone02" -1.650000 -0.710000 -1.420000 0.180000 0.710000 1.380000
$hbox 0 "Bone03" -2.120000 -0.600000 -1.130000 0.000000 0.620000 0.870000

$sequence "swim" "swim_ani" ACT_SWIM 1 LX loop
$sequence "swim2" "swim2_ani" ACT_SWIM 1 LX fps 70 loop
$sequence "attack" "attack_ani" ACT_IDLE 1 loop
$sequence "hover" "hover_ani" ACT_TWITCH 1 loop
$sequence "leftturn" "leftturn_ani" ACT_TURN_LEFT 1
$sequence "rightturn" "rightturn_ani" ACT_TURN_RIGHT 1
$sequence "death" "death_ani" ACT_DIESIMPLE 1
$sequence "death_end" "death_end_ani" ACT_DIEFORWARD 1
