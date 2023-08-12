$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "waiter.mdl"

$root "Bip01"

$flags 18432
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 65.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "waiter_ref"
	blank
}
$bodygroup "heads"
{
	studio "waiter_head_1_ref"
	studio "waiter_head_2_ref"
	blank
}
$bodygroup "accessories"
{
	studio "waiter_tray_1_ref"
	blank
}
$bodygroup "weapons"
{
	blank
}
$bodygroup "skeleton"
{
	blank
	studio "waiter_skeleton_reference_ref"
}


$controller 0 "Bone39" 32800 0 0
$controller 1 "Bone21" 32800 0 0
$controller 2 "Bip01 Head" XR -55 55

$hbox 0 "Bip01 Pelvis" -9.330000 -5.100000 -7.320000 4.760000 5.180000 7.320000
$hbox 0 "Bip01 Spine" -2.559815 -4.930000 -6.710000 5.560000 5.250000 6.720000
$hbox 0 "Bip01 L Thigh" -1.330000 -4.890000 -3.900000 20.280000 4.700000 4.200000
$hbox 0 "Bip01 L Calf" -1.460000 -4.140000 -3.160000 18.669420 3.200000 3.210000
$hbox 0 "Bip01 L Foot" -3.020000 -4.000000 -2.640000 3.732474 4.680000 3.270000
$hbox 0 "Bip01 L Toe0" -0.390000 0.000000 -1.740000 3.560000 2.190000 2.320000
$hbox 0 "Bip01 R Thigh" -1.330000 -4.890000 -4.200000 20.280000 4.700000 3.900000
$hbox 0 "Bip01 R Calf" -1.460000 -4.140000 -3.210000 18.669420 3.200000 3.160000
$hbox 0 "Bip01 R Foot" -3.020000 -4.000000 -3.270000 3.732474 4.680000 2.640000
$hbox 0 "Bip01 R Toe0" -0.390000 0.000000 -2.320000 3.560000 2.190000 1.730000
$hbox 0 "Bip01 Spine1" -2.900000 -5.000000 -7.630000 7.170000 5.710000 7.640000
$hbox 0 "Bip01 Spine2" -2.190000 -5.240000 -9.220000 9.250000 5.970000 9.220000
$hbox 0 "Bip01 Spine3" -1.420000 -5.240000 -6.430000 7.810000 6.100000 6.660000
$hbox 0 "Bip01 Neck" -2.092093 -2.560000 -3.502306 3.300000 2.590000 3.502307
$hbox 0 "Bip01 Head" -1.840000 -4.630000 -3.610000 9.040000 4.820000 3.610000
$hbox 0 "Bone39" -2.700000 -3.260886 -2.480000 2.700000 0.000000 0.000000
$hbox 0 "Bip01 L Clavicle" -1.010000 -4.190000 -5.670000 5.460000 3.890000 3.260000
$hbox 0 "Bip01 L UpperArm" -1.830000 -2.410000 -4.460000 12.150000 3.160000 3.580000
$hbox 0 "Bip01 L Forearm" -2.600000 -2.170000 -2.210000 11.180000 2.540000 1.930000
$hbox 0 "Bip01 L Hand" -0.710000 -1.160000 -1.750000 3.893348 1.770000 1.900000
$hbox 0 "Bip01 L Finger2" -1.790000 -0.500000 -1.120000 1.502225 0.570000 0.420000
$hbox 0 "Bip01 R Clavicle" -0.830000 -4.190000 -3.260000 5.460000 3.890000 3.980000
$hbox 0 "Bip01 R UpperArm" -1.830000 -2.410000 -3.580000 12.140000 3.160000 4.460000
$hbox 0 "Bip01 R Forearm" -2.600000 -2.170000 -1.930000 16.780000 2.540000 2.210000
$hbox 0 "Bip01 R Hand" -0.710000 -1.160000 -1.900000 2.830000 1.770000 1.750000
$hbox 0 "Bone09" -0.740000 -1.230000 -0.610000 0.440000 0.090000 0.410000
$hbox 0 "Bone11" -0.440000 -1.230000 -0.610000 0.740000 0.090000 0.410000
$hbox 0 "Bone17" -0.980000 -0.840000 -0.620000 0.820000 0.670000 0.870000
$hbox 0 "Bone19" -0.830000 -0.840000 -0.620000 0.970000 0.670000 0.870000
$hbox 0 "tray" -6.700000 -1.400000 -6.930000 6.820000 1.500000 6.840000

$sequence "null" "null_ani" loop
$sequence "a" "a_ani" loop
$sequence "e" "e_ani" loop
$sequence "o" "o_ani" loop
$sequence "f" "f_ani" loop
$sequence "s" "s_ani" loop
$sequence "m" "m_ani" loop
$sequence "idle1" "idle1_ani" ACT_IDLE 1
$sequence "idle2" "idle2_ani" ACT_IDLE 1
$sequence "idle3" "idle3_ani" ACT_IDLE 1
$sequence "idle4" "idle4_ani" ACT_IDLE 1
$sequence "walk" "walk_ani" ACT_WALK 1 LX LY fps 25 loop
$sequence "serve" "serve_ani"
$sequence "handoff" "handoff_ani" fps 25
