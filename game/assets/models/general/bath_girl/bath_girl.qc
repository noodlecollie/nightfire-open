$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "bath_girl.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "bath_girl_ref"
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
	studio "bath_girl_ref"
}


$controller 0 "Bone39" 32800 0 0
$controller 1 "Bone21" 32800 0 0

$hbox 0 "Bip01 Pelvis" -4.300000 -4.450000 -5.470000 5.410000 4.430000 5.480000
$hbox 0 "Bip01 Spine" -2.513425 -2.470000 -5.060000 5.260000 4.800000 5.060000
$hbox 0 "Bip01 L Thigh" -2.480000 -4.570000 -2.600000 20.320000 4.170000 4.310000
$hbox 0 "Bip01 L Calf" -2.850000 -2.940000 -2.380000 17.160000 2.620000 1.720000
$hbox 0 "Bip01 L Foot" 0.000000 -1.920000 -1.320000 4.140000 4.830000 1.610000
$hbox 0 "Bip01 L Toe0" -0.330000 -0.070000 -1.320000 3.070000 1.170000 1.610000
$hbox 0 "Bip01 R Thigh" -2.480000 -4.570000 -4.330000 20.320000 4.540000 2.600000
$hbox 0 "Bip01 R Calf" -2.850000 -2.940000 -1.730000 17.160000 2.620000 2.380000
$hbox 0 "Bip01 R Foot" -0.610000 -1.910000 -1.600000 4.140000 4.830000 1.280000
$hbox 0 "Bip01 R Toe0" -0.350000 -0.070000 -1.600000 3.070000 1.170000 1.260000
$hbox 0 "Bip01 Spine1" -1.310000 -2.360000 -4.460000 4.280000 4.630000 4.460000
$hbox 0 "Bip01 Spine2" -1.120000 -3.430000 -5.930000 6.150000 4.990000 5.930000
$hbox 0 "Bip01 Spine3" -1.170000 -3.530000 -5.850000 7.010000 4.140000 5.930000
$hbox 0 "Bip01 Neck" -1.598849 -1.450000 -1.990000 1.923764 2.470000 1.990000
$hbox 0 "Bip01 Head" -1.390000 -4.920000 -4.590000 10.010000 4.335272 4.490000
$hbox 0 "Bone39" -2.090000 -2.838459 -2.320000 2.100000 0.840000 0.000000
$hbox 0 "Bip01 L Clavicle" 0.000000 -2.480000 -0.970000 5.880000 2.270000 2.340000
$hbox 0 "Bip01 L UpperArm" -1.900000 -1.960000 -1.730000 10.830000 2.190000 2.180000
$hbox 0 "Bip01 L Forearm" -1.340000 -1.640000 -1.320000 11.920000 1.790000 1.410000
$hbox 0 "Bip01 L Hand" -0.530000 -0.770000 -1.600000 4.030000 1.200000 1.400000
$hbox 0 "Bip01 R Clavicle" 0.000000 -2.480000 -2.340000 5.880000 2.270000 0.970000
$hbox 0 "Bip01 R UpperArm" -1.900000 -1.960000 -2.180000 10.830000 2.190000 1.730000
$hbox 0 "Bip01 R Forearm" -1.340000 -1.640000 -1.410000 11.920000 1.790000 1.320000
$hbox 0 "Bip01 R Hand" -0.530000 -0.770000 -1.400000 4.020000 1.200000 1.600000
$hbox 0 "Bone43" -2.940000 0.000000 -2.260000 2.640000 3.030000 3.170000
$hbox 0 "Bone41" -2.930000 -3.030000 -3.170000 2.910000 0.000000 2.260000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 25
$sequence "idle3" "idle3_ani" ACT_IDLE 1 fps 20
$sequence "walk" "walk_ani" ACT_WALK 1 LX loop
$sequence "run" "run_ani" ACT_RUN 1 LX LY fps 40 loop
$sequence "cower1" "cower1_ani" ACT_EXCITED 1 fps 15 loop {
	event 1004 0 "servant_girl/scream1.wav"
}
$sequence "cower2" "cower2_ani" ACT_FEAR_DISPLAY 1 fps 15 loop {
	event 1004 0 "servant_girl/scream2.wav"
}
$sequence "cower3" "cower3_ani" ACT_CROUCHIDLE 1 fps 15 loop {
	event 1004 0 "servant_girl/scream3.wav"
}
$sequence "hostage" "hostage_ani" ACT_INVALID 1 fps 20 loop
$sequence "hostagerescued" "hostagerescued_ani" ACT_INVALID 1 LX LY fps 20 {
	event 1003 48 "thanks"
}
$sequence "hostagescared" "hostagescared_ani" ACT_INVALID 1 fps 25 loop {
	event 5006 1 "hostage"
}
$sequence "cower_idle" "cower_idle_ani" fps 20 loop
$sequence "hostage_death" "hostage_death_ani" ACT_INVALID 1 {
	event 2001 27 ""
	event 2002 27 ""
}
$sequence "death" "death_ani" ACT_DIESIMPLE 1 LX LY {
	event 2002 32 ""
}
