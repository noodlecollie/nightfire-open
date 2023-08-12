$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "old_truck.mdl"

$root "old_truck"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "truck"
{
	studio "old_truck_closed_back_ref"
}
$bodygroup "old_guy"
{
	studio "old_truck_driver_ref"
}
$bodygroup "paper"
{
	studio "old_truck_paper_reference_ref"
}

$attachment "" "old_truck" 42.000000 80.000000 -53.000000
$attachment "" "old_truck" -42.000000 80.000000 -53.000000
$attachment "" "old_truck" 35.000000 -100.000000 -53.000000
$attachment "" "old_truck" -35.000000 -100.000000 -53.000000
$attachment "" "old_truck" -42.000000 112.000000 -28.000000
$attachment "" "old_truck" 27.000000 -123.000000 -16.500000
$attachment "" "old_truck" -28.700000 -123.000000 -16.500000
$attachment "" "old_truck" 27.000000 -130.000000 -17.500000
$attachment "" "old_truck" -28.700000 -130.000000 -17.500000


$hbox 0 "old_truck" -50.460000 -136.270000 -55.160000 50.460000 130.650000 55.160000
$hbox 0 "tires_back" -48.270000 -18.520000 -19.470000 48.270000 18.520000 19.470000
$hbox 0 "tires_front_right" -6.130000 -18.520000 -19.470000 6.130000 18.520000 19.470000
$hbox 0 "tires_front_left" -6.130000 -18.520000 -19.470000 6.130000 18.520000 19.470000
$hbox 0 "Bip01 Pelvis" -2.800000 -4.560000 -6.910000 6.580000 6.450000 6.910000
$hbox 0 "Bip01 Spine" -2.554760 -4.630000 -6.260000 5.550000 5.170000 6.260000
$hbox 0 "Bip01 L Thigh" -2.260000 -3.520000 -3.470000 18.840000 4.000000 4.440000
$hbox 0 "Bip01 L Calf" -3.130000 -3.670000 -3.210000 16.958320 3.500000 2.910000
$hbox 0 "Bip01 L Foot" -5.140000 -3.640000 -2.960000 3.659291 4.730000 2.680000
$hbox 0 "Bip01 L Toe0" -0.760000 0.000000 -1.880000 3.150000 2.330000 1.950000
$hbox 0 "Bip01 R Thigh" -2.410000 -3.520000 -4.380000 18.420000 4.000000 3.450000
$hbox 0 "Bip01 R Calf" -2.850000 -3.670000 -2.900000 16.958320 3.410000 3.210000
$hbox 0 "Bip01 R Foot" -4.790000 -3.500000 -2.670000 3.659289 4.730000 2.990000
$hbox 0 "Bip01 R Toe0" -0.760000 0.000000 -1.950000 3.150000 2.330000 1.880000
$hbox 0 "Bip01 Spine1" -2.780000 -4.880000 -7.130000 4.400429 5.490000 7.130000
$hbox 0 "Bip01 Spine2" -1.420000 -5.330000 -7.730000 8.840000 5.670000 7.740000
$hbox 0 "Bip01 Spine3" -2.240000 -5.170000 -5.800000 7.450000 5.730000 5.800000
$hbox 0 "Bip01 Neck" -2.329054 -2.550000 -3.502311 2.540000 2.570000 3.502303
$hbox 0 "Bip01 Head" 0.000000 -4.760000 -4.130000 8.190000 5.540000 4.130000
$hbox 0 "Bone39" -2.460000 -3.260888 -2.870000 2.460000 0.140000 0.000000
$hbox 0 "Bip01 L Clavicle" 0.000000 -3.970000 -3.080000 6.060000 3.810000 2.840000
$hbox 0 "Bip01 L UpperArm" -0.960000 -2.830000 -3.630000 11.620000 3.360000 2.710000
$hbox 0 "Bip01 L Forearm" -1.080000 -2.530000 -2.340000 11.480000 2.140000 1.800000
$hbox 0 "Bip01 L Hand" -0.710000 -1.070000 -1.690000 2.920000 1.850000 1.820000
$hbox 0 "Bip01 R Clavicle" 0.000000 -3.970000 -2.840000 6.060000 3.810000 3.080000
$hbox 0 "Bip01 R UpperArm" -0.960000 -2.830000 -2.710000 11.620000 3.360000 3.630000
$hbox 0 "Bip01 R Forearm" -1.080000 -2.530000 -1.800000 11.480000 2.140000 2.340000
$hbox 0 "Bip01 R Hand" -0.710000 -1.070000 -1.820000 2.920000 1.790000 1.690000
$hbox 0 "newspaper" -11.720000 -10.470000 -6.270000 11.720000 7.300000 0.000000

$sequence "idle" "idle_ani" ACT_IDLE 1 fps 15 loop
$sequence "driving" "driving_ani" fps 10 loop
$sequence "driving2" "driving2_ani" fps 10 loop
$sequence "reverse" "reverse_ani" fps 10 loop
$sequence "driverchatting1" "driverchatting1_ani" fps 25
$sequence "driverchatting2" "driverchatting2_ani" fps 25
$sequence "driverchatting3" "driverchatting3_ani" fps 25
$sequence "driverwaving" "driverwaving_ani" fps 25
$sequence "drivercowering" "drivercowering_ani" fps 15 loop
