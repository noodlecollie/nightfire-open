$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "ninja.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "ninja_ref"
	blank
}
$bodygroup "heads"
{
	blank
	blank
}
$bodygroup "accessories"
{
	blank
	studio "sword_1_ref"
	studio "sword_2_ref"
}
$bodygroup "guns"
{
	blank
	studio "gun_1_ref"
	studio "gun_2_ref"
}
$bodygroup "skeleton"
{
	blank
	studio "ninja_skeleton_ref"
}

$attachment "" "Bip01 L Hand" 22.400000 0.000000 -4.000000

$controller 0 "Bone39" 32800 0 0

$hbox 3 "Bip01 Pelvis" -3.640000 -6.050000 -8.070000 6.340000 8.410000 8.300000
$hbox 6 "Bip01 L Thigh" 0.000000 -6.350000 -6.050000 16.310000 5.570000 3.840000
$hbox 6 "Bip01 L Calf" 0.000000 -5.190000 -4.440000 22.150000 5.680000 4.780000
$hbox 6 "Bip01 L Foot" -1.080000 -1.900000 -3.240000 3.230000 11.210000 2.860000
$hbox 7 "Bip01 R Thigh" 0.000000 -6.350000 -4.060000 16.310000 5.570000 5.820000
$hbox 7 "Bip01 R Calf" 0.000000 -5.190000 -5.010000 22.150000 5.680000 4.220000
$hbox 7 "Bip01 R Foot" -1.080000 -1.900000 -3.720000 3.230000 11.210000 2.390000
$hbox 3 "Bip01 Spine1" -4.570000 -9.990000 -10.520000 11.490000 8.420000 10.750000
$hbox 2 "Bip01 Spine2" 0.000000 -10.510000 -11.440000 14.050000 9.260000 12.420000
$hbox 0 "Bip01 Head" 0.000000 -5.430000 -6.130000 14.660000 9.400000 6.110000
$hbox 4 "Bip01 L UpperArm" -2.810000 -1.990000 -4.740000 12.060000 4.780000 3.500000
$hbox 4 "Bip01 L Forearm" 0.000000 -1.430000 -2.150000 10.210000 4.010000 2.980000
$hbox 4 "Bip01 L Hand" -0.270000 -2.650000 -2.750000 7.580000 1.890000 1.800000
$hbox 5 "Bip01 R UpperArm" -2.830000 -1.990000 -3.460000 12.030000 4.780000 4.780000
$hbox 5 "Bip01 R Forearm" 0.000000 -1.430000 -2.940000 10.190000 4.000000 2.180000
$hbox 5 "Bip01 R Hand" -0.160000 -2.040000 -1.790000 7.500000 2.400000 2.760000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 loop
$sequence "jump" "jump_ani" ACT_LEAP 1 {
	event 60 13 ""
}
$sequence "jump2" "jump2_ani" ACT_HOP 1 {
	event 61 15 ""
}
$sequence "melee_attack1" "melee_attack1_ani" ACT_MELEE_ATTACK1 1 fps 32 {
	event 5 9 ""
	event 3 26 ""
	event 3 38 ""
	event 5004 24 "ninja/swing.wav"
	event 5004 37 "ninja/swing2.wav"
}
$sequence "melee_attack2" "melee_attack2_ani" ACT_MELEE_ATTACK2 1 fps 32 {
	event 5 11 ""
	event 3 27 ""
	event 3 38 ""
	event 5004 25 "ninja/swing2.wav"
	event 5004 36 "ninja/swing.wav"
}
$sequence "die_forward" "die_forward_ani" ACT_DIEFORWARD 1 LX LY {
	event 2002 80 ""
}
$sequence "die_backwards" "die_backwards_ani" ACT_DIEBACKWARD 1 LX LY {
	event 2002 80 ""
}
$sequence "die_forward" "die_forward_ani" ACT_DIESIMPLE 1 LX LY {
	event 2002 80 ""
}
$sequence "shoot" "shoot_ani" ACT_RANGE_ATTACK1 1 {
	event 1 3 ""
	event 5001 3 "31"
}
$sequence "shot_burst" "shot_burst_ani" ACT_RANGE_ATTACK1 1 {
	event 6 1 ""
	event 5001 1 "31"
	event 8 9 ""
	event 5001 9 "31"
	event 7 17 ""
	event 5001 17 "31"
}
$sequence "run" "run_ani" ACT_RUN 1 LX loop {
	event 5006 3 "steps"
	event 5006 12 "steps"
}
$sequence "run_limp" "run_limp_ani" ACT_RUN_HURT 1 LX loop
$sequence "dodge_right" "dodge_right_ani" ACT_ROLL_RIGHT 2 LX LY LZ fps 20
$sequence "dodge_left" "dodge_left_ani" ACT_ROLL_LEFT 2 LX LY LZ fps 20
$sequence "slide_right" "slide_right_ani" ACT_ROLL_RIGHT 1 LX LY LZ fps 35
$sequence "slide_left" "slide_left_ani" ACT_ROLL_LEFT 1 LX LY LZ fps 35
$sequence "reload" "reload_ani" ACT_RELOAD 1 fps 20
$sequence "grenade_throw(feet)" "grenade_throw(feet)_ani" ACT_RANGE_ATTACK2 1
$sequence "grenade_toss" "grenade_toss_ani" ACT_RANGE_ATTACK2 1
$sequence "flinch" "flinch_ani" ACT_FLINCH_CHEST 1
$sequence "switch_to_sword" "switch_to_sword_ani" ACT_ARM 1 fps 35 {
	event 5 15 ""
}
$sequence "switch_to_gun" "switch_to_gun_ani" ACT_ARM 1 fps 35 {
	event 5 15 ""
}
