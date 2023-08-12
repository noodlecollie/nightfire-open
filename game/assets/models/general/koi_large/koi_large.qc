$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "koi_large.mdl"

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



$hbox 0 "Bone01" -9.180000 -2.040000 -1.600000 1.790000 2.000000 4.930000
$hbox 0 "Bone02" -4.710000 -2.030000 -4.070000 0.530000 2.040000 3.940000
$hbox 0 "Bone03" -6.080000 -1.740000 -3.250000 0.000000 1.770000 2.490000
$hbox 0 "Bone04" -4.880000 -0.670000 -2.820000 0.050000 0.670000 2.580000

$sequence "swim" "swim_ani" ACT_SWIM 1 LX loop
$sequence "swim2" "swim2_ani" ACT_SWIM 1 LX fps 70 loop
$sequence "attack" "attack_ani" ACT_IDLE 1 loop
$sequence "hover" "hover_ani" ACT_TWITCH 1 loop
$sequence "leftturn" "leftturn_ani" ACT_TURN_LEFT 1
$sequence "rightturn" "rightturn_ani" ACT_TURN_RIGHT 1
$sequence "death" "death_ani" ACT_DIESIMPLE 1
$sequence "death_end" "death_end_ani" ACT_DIEFORWARD 1
