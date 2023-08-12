$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "party_girl_03.mdl"

$root "Bip01"

$flags 18432
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 65.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "party_girl_03_reference_ref"
	blank
}
$bodygroup "heads"
{
	blank
	blank
}
$bodygroup "accessories"
{
	studio "party_girl_wineglass_ref"
	blank
}
$bodygroup "weapons"
{
	blank
}
$bodygroup "skeleton"
{
	blank
	studio "party_girl_03_underwear_ref"
}


$controller 0 "Bone39" 32800 0 0
$controller 1 "Bone21" 32800 0 0
$controller 2 "Bip01 Head" XR -55 55

$hbox 0 "Bip01 Pelvis" -6.160000 -4.640000 -6.210000 4.530000 4.340000 6.210000
$hbox 0 "Bip01 Spine" -2.513427 -2.340000 -4.490000 5.760000 4.680000 4.490000
$hbox 0 "Bip01 L Thigh" -4.020000 -4.740000 -3.440000 18.820000 4.170000 4.560000
$hbox 0 "Bip01 L Calf" -2.840000 -3.180000 -2.590000 19.740000 3.190000 2.100000
$hbox 0 "Bip01 L Foot" -0.570000 -2.630000 -1.800000 7.410000 3.810000 1.520000
$hbox 0 "Bip01 L Toe0" -1.350000 0.000000 -1.800000 2.230000 1.330000 1.520000
$hbox 0 "Bip01 R Thigh" -4.020000 -4.740000 -4.560000 18.820000 4.170000 3.440000
$hbox 0 "Bip01 R Calf" -2.840000 -3.180000 -2.100000 19.740000 3.190000 2.590000
$hbox 0 "Bip01 R Foot" -0.570000 -2.630000 -1.520000 7.410000 3.810000 1.800000
$hbox 0 "Bip01 R Toe0" -1.350000 0.000000 -1.520000 2.230000 1.330000 1.800000
$hbox 0 "Bip01 Spine1" -1.290000 -2.110000 -4.590000 4.430000 4.600000 4.590000
$hbox 0 "Bip01 Spine2" -1.580000 -3.710000 -5.740000 6.770000 6.040000 5.740000
$hbox 0 "Bip01 Spine3" -2.140000 -3.740000 -6.260000 7.250000 5.270000 6.030000
$hbox 0 "Bip01 Neck" -1.598847 -1.590000 -2.020000 2.340000 2.440000 2.020000
$hbox 0 "Bip01 Head" -0.370000 -4.360000 -3.470000 8.150000 4.520000 3.470000
$hbox 0 "Bone39" -2.040000 -2.838459 -2.140000 2.040000 0.660000 0.000000
$hbox 0 "Bip01 L Clavicle" 0.000000 -2.080000 -1.050000 5.890000 2.550000 2.180000
$hbox 0 "Bip01 L UpperArm" -2.250000 -2.020000 -2.950000 11.320000 2.590000 2.090000
$hbox 0 "Bip01 L Forearm" -1.230000 -1.640000 -1.360000 11.920000 1.790000 1.460000
$hbox 0 "Bip01 L Hand" -0.530000 -0.740000 -1.600000 4.030000 1.200000 1.400000
$hbox 0 "Bip01 R Clavicle" 0.000000 -2.090000 -2.180000 5.890000 2.550000 1.050000
$hbox 0 "Bip01 R UpperArm" -1.140000 -2.020000 -2.090000 11.320000 2.590000 2.950000
$hbox 0 "Bip01 R Forearm" -1.240000 -1.640000 -1.460000 11.910000 1.790000 1.360000
$hbox 0 "Bip01 R Hand" -0.530000 -0.740000 -1.400000 4.020000 1.200000 1.600000
$hbox 0 "wineglass" -1.660000 -1.430000 0.000000 1.650000 1.430000 7.710000

$sequence "null" "null_ani" loop
$sequence "a" "a_ani" loop
$sequence "e" "e_ani" loop
$sequence "o" "o_ani" loop
$sequence "f" "f_ani" loop
$sequence "s" "s_ani" loop
$sequence "m" "m_ani" loop
$sequence "idle1" "idle1_ani" ACT_IDLE 4 fps 25
$sequence "idle2" "idle2_ani" ACT_IDLE 4 fps 20
$sequence "idle3" "idle3_ani" ACT_IDLE 4 fps 25
$sequence "idle4" "idle4_ani" ACT_IDLE 4 fps 25
$sequence "walk" "walk_ani" ACT_WALK 1 LX fps 40 loop
$sequence "run" "run_ani" ACT_RUN 1 LX LY fps 35 loop
$sequence "cower1" "cower1_ani" ACT_EXCITED 1 fps 35 loop {
	event 1004 0 "servant_girl/scream1.wav"
}
$sequence "cower_loop" "cower_loop_ani" ACT_COWER 1 fps 35 loop {
	event 1004 0 "servant_girl/scream1.wav"
}
$sequence "cower2" "cower2_ani" ACT_FEAR_DISPLAY 1 fps 35 loop {
	event 1004 0 "servant_girl/scream1.wav"
}
$sequence "death" "death_ani" ACT_DIESIMPLE 1 LX LY
$sequence "strike_a_pose1" "strike_a_pose1_ani" fps 25
$sequence "strike_a_pose2" "strike_a_pose2_ani" fps 25
$sequence "strike_a_pose3" "strike_a_pose3_ani" fps 15
$sequence "strike_a_pose4" "strike_a_pose4_ani" fps 33
