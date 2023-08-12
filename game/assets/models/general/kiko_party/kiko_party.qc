$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "kiko_party.mdl"

$root "Bip01"

$flags 18432
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 65.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "kiko_party_ref"
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
}
$bodygroup "weapons"
{
	blank
}
$bodygroup "skeleton"
{
	blank
	studio "kiko_party_underwear_ref"
}


$controller 0 "Bone39" 32800 0 0
$controller 1 "Bone21" 32800 0 0
$controller 2 "Bip01 Head" XR -55 55

$hbox 0 "Bip01" -1.100000 -7.370000 -26.130000 4.080000 0.000000 4.690000
$hbox 0 "Bip01 Pelvis" -4.920000 -4.320000 -6.510000 6.050000 4.600000 6.510000
$hbox 0 "Bip01 Spine" -2.560359 -3.040000 -4.800000 6.120000 4.140000 4.800000
$hbox 0 "Bip01 L Thigh" -2.620000 -4.440000 -2.720000 21.590000 4.040000 6.130000
$hbox 0 "Bip01 L Calf" -0.890000 -3.530000 -2.280000 18.620900 1.600000 6.830000
$hbox 0 "Bip01 L Foot" -1.630000 -1.730000 -1.740000 4.555087 4.520000 1.430000
$hbox 0 "Bip01 L Toe0" -0.270000 0.000000 -1.740000 2.750000 2.280000 1.460000
$hbox 0 "Bip01 R Thigh" -2.620000 -4.440000 -6.120000 26.120000 4.040000 2.720000
$hbox 0 "Bip01 R Calf" -0.890000 -3.530000 -6.850000 18.620900 1.600000 2.280000
$hbox 0 "Bip01 R Foot" -1.630000 -1.760000 -1.440000 4.555087 4.450000 1.730000
$hbox 0 "Bip01 R Toe0" -0.390000 0.000000 -1.470000 2.590000 2.100000 1.730000
$hbox 0 "Bip01 Spine1" -2.060000 -2.690000 -4.650000 4.250000 4.470000 4.650000
$hbox 0 "Bip01 Spine2" -2.310000 -3.880000 -5.150000 5.810000 5.390000 5.150000
$hbox 0 "Bip01 Spine3" -2.850000 -3.860000 -5.180000 5.870000 4.090000 5.180000
$hbox 0 "Bip01 Neck" -1.521844 -1.760000 -2.281434 1.979605 1.740000 2.281433
$hbox 0 "Bip01 Head" -0.820000 -4.410000 -3.310000 7.870000 4.200000 3.280000
$hbox 0 "Bone39" -2.200000 -2.912899 -2.270000 2.200000 0.180000 0.000000
$hbox 0 "Bip01 L Clavicle" -0.170000 -1.860000 -2.090000 4.970000 2.290000 1.830000
$hbox 0 "Bip01 L UpperArm" -1.580000 -1.930000 -2.230000 13.590000 2.460000 1.700000
$hbox 0 "Bip01 L Forearm" -0.640000 -1.520000 -1.240000 10.320000 1.610000 1.140000
$hbox 0 "Bip01 L Hand" 0.000000 -0.610000 -1.460000 4.440000 0.980000 1.480000
$hbox 0 "Bip01 L Finger01" -0.310000 -0.710000 -0.740000 1.320000 0.360000 0.390000
$hbox 0 "Bip01 R Clavicle" -0.170000 -1.860000 -1.830000 4.970000 2.290000 2.090000
$hbox 0 "Bip01 R UpperArm" -1.580000 -1.930000 -1.700000 13.590000 2.460000 2.230000
$hbox 0 "Bip01 R Forearm" -0.640000 -1.520000 -1.140000 10.320000 1.610000 1.240000
$hbox 0 "Bip01 R Hand" 0.000000 -0.610000 -1.480000 4.430000 0.980000 1.460000
$hbox 0 "Bip01 R Finger01" -0.310000 -0.710000 -0.400000 1.310000 0.360000 0.730000

$sequence "null" "null_ani" loop
$sequence "a" "a_ani" loop
$sequence "e" "e_ani" loop
$sequence "o" "o_ani" loop
$sequence "f" "f_ani" loop
$sequence "s" "s_ani" loop
$sequence "m" "m_ani" loop
$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 20
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 25
$sequence "idle3" "idle3_ani" ACT_IDLE 1 fps 25
$sequence "idle4" "idle4_ani" ACT_IDLE 1 fps 20
$sequence "walk" "walk_ani" ACT_WALK 1 LX LY fps 35 loop
$sequence "run" "run_ani" ACT_RUN 1 LX LY fps 25 loop
$sequence "cower1" "cower1_ani" ACT_EXCITED 1 fps 15 loop {
	event 1004 0 "servant_girl/scream1.wav"
}
$sequence "cower_loop" "cower_loop_ani" ACT_COWER 1 fps 15 loop {
	event 1004 0 "servant_girl/scream1.wav"
}
$sequence "cower2" "cower2_ani" ACT_FEAR_DISPLAY 1 fps 15 loop {
	event 1004 0 "servant_girl/scream1.wav"
}
$sequence "death" "death_ani" ACT_DIESIMPLE 1 LX LY
$sequence "strike_a_pose1" "strike_a_pose1_ani" fps 25 loop
$sequence "strike_a_pose2" "strike_a_pose2_ani" fps 25 loop
