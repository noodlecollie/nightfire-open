$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "mayhew.mdl"

$root "Bip01"

$flags 18432
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "mayhew_lod0_ref"
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
	blank
}
$bodygroup "weapons"
{
	blank
	blank
}
$bodygroup "skeleton"
{
	blank
	studio "mayhew_skeleton_ref"
}


$controller 0 "Bone39" 32800 0 0
$controller 1 "Bone21" 32800 0 0

$hbox 0 "Bip01" -2.820000 -35.080000 0.000000 2.030000 0.000000 21.600000
$hbox 0 "Bip01 Pelvis" -4.460000 -5.600000 -7.910000 8.850000 6.200000 7.940000
$hbox 0 "Bip01 Spine" -4.290000 -4.900000 -8.040000 8.380000 7.140000 8.080000
$hbox 0 "Bip01 L Thigh" -3.380000 -5.340000 -4.200000 19.820000 5.650000 4.950000
$hbox 0 "Bip01 L Calf" -3.590000 -3.480000 -3.690000 19.480000 3.760000 3.300000
$hbox 0 "Bip01 L Foot" -0.720000 -3.490000 -2.600000 4.120000 5.190000 2.920000
$hbox 0 "Bip01 L Toe0" -0.580000 -0.020000 -2.150000 3.800000 2.080000 2.120000
$hbox 0 "Bip01 R Thigh" -3.380000 -5.350000 -4.940000 19.810000 5.640000 4.160000
$hbox 0 "Bip01 R Calf" -3.590000 -3.420000 -3.360000 19.480000 4.190000 3.630000
$hbox 0 "Bip01 R Foot" -0.710000 -3.490000 -2.950000 4.120000 5.190000 2.570000
$hbox 0 "Bip01 R Toe0" -0.580000 -0.020000 -2.160000 3.800000 2.080000 2.110000
$hbox 0 "Bip01 Spine1" -0.020000 -5.030000 -8.040000 6.460000 7.140000 8.080000
$hbox 0 "Bip01 Spine2" -0.760000 -5.020000 -7.980000 7.200000 6.790000 8.010000
$hbox 0 "Bip01 Spine3" -3.560000 -4.840000 -6.430000 8.720000 6.430000 7.740000
$hbox 0 "Bip01 Head" -1.050000 -4.110000 -3.790000 8.190000 5.040000 3.940000
$hbox 0 "Bone15" -1.010000 -1.430000 -0.460000 1.030000 0.090000 0.690000
$hbox 0 "Bone39" -2.860000 -2.728410 -2.390000 2.850000 0.290000 0.000000
$hbox 0 "Bip01 L Clavicle" -0.600000 -5.310000 -5.530000 7.300000 3.360000 3.020000
$hbox 0 "Bip01 L UpperArm" -2.120000 -3.850000 -4.430000 12.060000 2.360000 3.490000
$hbox 0 "Bip01 L Forearm" -2.290000 -2.930000 -2.820000 12.010000 2.290000 2.450000
$hbox 0 "Bip01 L Hand" -0.250000 -1.130000 -1.810000 4.440000 1.270000 1.900000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.730000 -1.530000 2.010000 0.530000 0.590000
$hbox 0 "Bip01 L Finger01" 0.000000 -0.580000 -0.720000 2.030000 0.450000 0.590000
$hbox 0 "Bip01 L Finger2" 0.000000 -0.510000 -0.630000 1.940000 0.740000 0.380000
$hbox 0 "Bip01 R Clavicle" -0.420000 -5.320000 -2.980000 7.340000 3.330000 8.230000
$hbox 0 "Bip01 R UpperArm" -2.200000 -3.980000 -3.500000 12.160000 2.320000 4.550000
$hbox 0 "Bip01 R Forearm" -2.370000 -2.940000 -2.390000 11.830000 2.270000 2.800000
$hbox 0 "Bip01 R Hand" -0.300000 -1.050000 -1.950000 4.460000 1.130000 1.790000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.530000 -0.560000 2.050000 0.590000 1.490000

$sequence "null" "null_ani" loop
$sequence "a" "a_ani" loop
$sequence "e" "e_ani" loop
$sequence "o" "o_ani" loop
$sequence "f" "f_ani" loop
$sequence "s" "s_ani" loop
$sequence "m" "m_ani" loop
$sequence "idle_1" "idle_1_ani" ACT_IDLE 2 fps 17
$sequence "idle_2" "idle_2_ani" ACT_IDLE 2 fps 20
$sequence "idle_3" "idle_3_ani" ACT_IDLE 2 fps 25
$sequence "idle_5" "idle_5_ani" ACT_IDLE_ANGRY 2 fps 16
$sequence "idle_6" "idle_6_ani" ACT_EXCITED 2 fps 25
$sequence "walk_1" "walk_1_ani" ACT_WALK 1 LX LY loop {
	event 5006 0 "footsteps"
	event 5006 15 "footsteps"
}
$sequence "run_1" "run_1_ani" ACT_RUN 1 LX LY fps 26 loop {
	event 5006 8 "footsteps"
	event 5006 16 "footsteps"
}
$sequence "dead" "dead_ani"
$sequence "death" "death_ani" ACT_DIEBACKWARD 1 {
	event 2002 50 ""
}
$sequence "death2" "death2_ani" ACT_DIEFORWARD 1 {
	event 2002 35 ""
}
$sequence "dieback1" "dieback1_ani" ACT_INVALID 1 {
	event 11 0 ""
	event 2002 50 ""
}
$sequence "dieforward" "dieforward_ani" ACT_INVALID 1 fps 35 {
	event 11 0 ""
	event 2002 35 ""
}
$sequence "diesimple" "diesimple_ani" ACT_DIESIMPLE 1 {
	event 11 0 ""
	event 2002 50 ""
}
$sequence "diebackwards" "diebackwards_ani" ACT_DIEBACKWARD 1 {
	event 11 0 ""
	event 2002 50 ""
}
$sequence "dieheadshot" "dieheadshot_ani" ACT_DIE_HEADSHOT 1 {
	event 11 0 ""
	event 2002 37 ""
}
$sequence "diegutshot" "diegutshot_ani" ACT_DIE_GUTSHOT 1 fps 25 {
	event 11 0 ""
	event 2002 37 ""
}
$sequence "dienutshot" "dienutshot_ani" ACT_INVALID 1 fps 35 {
	event 11 0 ""
	event 2002 35 ""
}
$sequence "die_explosion1" "die_explosion1_ani" ACT_DIEVIOLENT 1 {
	event 11 0 ""
	event 2002 35 ""
}
$sequence "cower_1" "cower_1_ani" ACT_FEAR_DISPLAY 1 fps 16 loop
$sequence "cower_2" "cower_2_ani" ACT_COWER 1 fps 16 loop
$sequence "hidden_switch" "hidden_switch_ani" {
	event 1003 58 "relay_secret_switch"
}
$sequence "electrocution" "electrocution_ani" ACT_BIG_FLINCH 1 fps 35
