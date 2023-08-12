$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "servant_girl_01.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "servant_girl_01_reference_ref"
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
	studio "servant_girl_01_underwear_ref"
}


$controller 0 "Bone39" 32800 0 0
$controller 1 "Bone21" 32800 0 0
$controller 2 "Bip01 Head" XR -55 55

$hbox 0 "Bip01 Pelvis" -9.210000 -4.640000 -6.210000 4.530000 4.430000 6.210000
$hbox 0 "Bip01 Spine" -2.513427 -2.340000 -4.490000 5.250000 4.790000 4.490000
$hbox 0 "Bip01 L Thigh" -4.020000 -4.740000 -3.380000 18.820000 4.240000 4.390000
$hbox 0 "Bip01 L Calf" -2.840000 -3.180000 -2.590000 19.260000 3.190000 2.100000
$hbox 0 "Bip01 L Foot" -1.050000 -2.600000 -1.800000 7.405087 3.810000 1.520000
$hbox 0 "Bip01 L Toe0" -1.350000 0.000000 -1.800000 2.230000 1.870000 1.520000
$hbox 0 "Bip01 R Thigh" -4.020000 -4.740000 -4.420000 18.820000 4.240000 3.380000
$hbox 0 "Bip01 R Calf" -2.840000 -3.180000 -2.100000 19.260000 3.190000 2.590000
$hbox 0 "Bip01 R Foot" -1.050000 -2.600000 -1.520000 7.405087 3.810000 1.800000
$hbox 0 "Bip01 R Toe0" -1.350000 0.000000 -1.520000 2.230000 1.870000 1.800000
$hbox 0 "Bip01 Spine1" -1.310000 -2.110000 -4.590000 3.860000 4.640000 4.590000
$hbox 0 "Bip01 Spine2" -1.580000 -3.890000 -5.740000 6.650000 5.700000 5.740000
$hbox 0 "Bip01 Spine3" -1.670000 -3.920000 -6.030000 7.010000 4.470000 6.030000
$hbox 0 "Bip01 Neck" -1.598847 -1.600000 -2.230000 2.880000 2.480000 2.240000
$hbox 0 "Bip01 Head" -0.420000 -5.890000 -4.150000 10.430000 4.420000 3.410000
$hbox 0 "Bone39" -2.370000 -2.838460 -2.040000 2.380000 0.730000 0.000000
$hbox 0 "Bip01 L Clavicle" 0.000000 -2.090000 -1.050000 5.890000 2.550000 2.260000
$hbox 0 "Bip01 L UpperArm" -1.140000 -2.020000 -2.950000 10.830000 2.590000 2.090000
$hbox 0 "Bip01 L Forearm" -0.770000 -1.640000 -1.360000 11.920000 1.790000 1.460000
$hbox 0 "Bip01 L Hand" -0.530000 -0.780000 -1.600000 4.030000 1.200000 1.400000
$hbox 0 "Bip01 R Clavicle" 0.000000 -2.090000 -2.260000 5.890000 2.550000 1.750000
$hbox 0 "Bip01 R UpperArm" -1.140000 -2.020000 -2.090000 10.830000 2.590000 2.950000
$hbox 0 "Bip01 R Forearm" -0.770000 -1.640000 -1.460000 11.920000 1.790000 1.360000
$hbox 0 "Bip01 R Hand" -0.530000 -0.780000 -1.400000 4.020000 1.200000 1.600000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 25 loop
$sequence "idle3" "idle3_ani" ACT_IDLE 1 fps 20 loop
$sequence "walk" "walk_ani" ACT_WALK 1 LX fps 38 loop
$sequence "run" "run_ani" ACT_RUN 1 LX LY fps 45 loop
$sequence "runcower" "runcower_ani" ACT_RUN_SCARED 1 LX LY fps 25 loop
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
$sequence "hostagescared" "hostagescared_ani" ACT_INVALID 1 fps 25 loop {
	event 5006 1 "hostage"
}
$sequence "hostagedeath" "hostagedeath_ani" ACT_INVALID 1 {
	event 2001 27 ""
	event 2002 27 ""
}
$sequence "hostagerescued" "hostagerescued_ani" ACT_INVALID 1 LX LY fps 25
$sequence "cower_idle" "cower_idle_ani" ACT_COWER 1 fps 20 loop
$sequence "death" "death_ani" ACT_DIESIMPLE 1 LX LY
