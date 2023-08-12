$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "party_guy.mdl"

$root "Bip01"

$flags 18432
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 65.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "party_guy_ref"
	blank
}
$bodygroup "heads"
{
	studio "party_guy_head_1_ref"
	studio "party_guy_head_2_ref"
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
	studio "party_guy_skeleton_ref"
}
$bodygroup "wineglass"
{
	studio "party_guy_wineglass_ref"
	blank
}


$controller 0 "Bone39" 32800 0 0
$controller 1 "Bone21" 32800 0 0
$controller 2 "Bip01 Head" XR -55 55

$hbox 0 "Bip01" -5.530000 -3.540000 -5.530000 4.590000 3.560000 2.390000
$hbox 0 "Bip01 Pelvis" -9.300000 -5.310000 -8.440000 4.550000 6.940000 8.460000
$hbox 0 "Bip01 Spine" -3.040000 -4.560000 -7.670000 4.270000 6.650000 7.680000
$hbox 0 "Bip01 L Thigh" -2.530000 -5.720000 -4.220000 21.060000 6.520000 4.410000
$hbox 0 "Bip01 L Calf" -2.910000 -4.180000 -3.470000 21.310000 3.860000 2.930000
$hbox 0 "Bip01 L Foot" -2.560000 -3.470000 -2.760000 3.980000 6.050000 2.910000
$hbox 0 "Bip01 L Toe0" -0.380000 -0.070000 -1.990000 4.260000 3.000000 1.890000
$hbox 0 "Bip01 R Thigh" -2.530000 -5.730000 -4.460000 21.020000 6.530000 4.200000
$hbox 0 "Bip01 R Calf" -2.890000 -4.050000 -3.070000 21.370000 4.090000 3.370000
$hbox 0 "Bip01 R Foot" -2.590000 -3.160000 -3.120000 4.050000 6.440000 2.530000
$hbox 0 "Bip01 R Toe0" -0.100000 -0.080000 -2.170000 4.620000 3.020000 1.710000
$hbox 0 "Bip01 Spine1" -2.240000 -4.320000 -6.950000 6.030000 6.340000 6.970000
$hbox 0 "Bip01 Spine2" -0.510000 -5.020000 -7.870000 7.850000 6.340000 7.880000
$hbox 0 "Bip01 Spine3" -3.060000 -5.050000 -7.870000 8.850000 6.210000 7.880000
$hbox 0 "Bip01 L Clavicle" -0.430000 -3.950000 -5.230000 6.730000 3.190000 2.800000
$hbox 0 "Bip01 L UpperArm" -1.320000 -4.260000 -3.120000 13.470000 2.960000 2.810000
$hbox 0 "Bip01 L Forearm" -1.510000 -2.320000 -2.800000 11.700000 2.960000 1.710000
$hbox 0 "Bip01 L Hand" -0.230000 -1.320000 -1.980000 4.899684 1.630000 2.449687
$hbox 0 "Bip01 L Finger0" 0.000000 -0.580000 -1.560000 2.060018 0.770000 0.850000
$hbox 0 "Bip01 L Finger01" -0.030000 -0.710000 -1.310000 1.780000 0.410000 0.570000
$hbox 0 "Bip01 R Clavicle" -0.250000 -3.930000 -2.800000 6.680000 3.170000 5.190000
$hbox 0 "Bip01 R UpperArm" -1.130000 -4.240000 -2.810000 13.490000 2.960000 3.130000
$hbox 0 "Bip01 R Forearm" -1.490000 -2.140000 -1.690000 11.840000 2.970000 2.810000
$hbox 0 "Bip01 R Hand" -0.270000 -1.320000 -1.990000 4.150000 1.640000 1.980000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.570000 -0.900000 2.060019 0.780000 1.510000
$hbox 0 "Bip01 R Finger01" -0.050000 -0.710000 -0.620000 1.760000 0.410000 1.260000
$hbox 0 "Bip01 Head" -0.450000 -4.380000 -3.490000 9.010000 4.840000 3.500000
$hbox 0 "Bone39" -2.500000 -2.788136 -2.300000 2.450000 0.240000 0.000000
$hbox 0 "wineglass" -1.650000 -1.430000 0.000000 1.660000 1.430000 7.710000

$sequence "null" "null_ani" loop
$sequence "a" "a_ani" loop
$sequence "e" "e_ani" loop
$sequence "o" "o_ani" loop
$sequence "f" "f_ani" loop
$sequence "s" "s_ani" loop
$sequence "m" "m_ani" loop
$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 15
$sequence "idle2" "idle2_ani" ACT_IDLE 1
$sequence "idle3" "idle3_ani" ACT_IDLE 2
$sequence "idle4" "idle4_ani" ACT_IDLE 3
$sequence "idle5" "idle5_ani" ACT_IDLE 2
$sequence "idle6" "idle6_ani" ACT_IDLE 2
$sequence "idle7" "idle7_ani" ACT_IDLE 2
$sequence "walk" "walk_ani" ACT_WALK 1 LX LY loop
$sequence "cower" "cower_ani" ACT_EXCITED 1 fps 20 loop
$sequence "cower2" "cower2_ani" ACT_FEAR_DISPLAY 1 fps 20 loop
$sequence "cower_loop" "cower_loop_ani" ACT_COWER 1 fps 20 loop
$sequence "death" "death_ani" ACT_DIESIMPLE 1 LX LY
