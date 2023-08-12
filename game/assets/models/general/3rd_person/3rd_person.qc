$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "3rd_person.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "NEW_bond_suit_ref"
	studio "NEW_bond_stea_ref"
	studio "NEW_bond_tux_ref"
}
$bodygroup "heads"
{
	studio "NEW_bond_head_ref"
	blank
}
$bodygroup "accessories"
{
	blank
	studio "bond_suction_reference_ref"
}


$controller 0 "Bip01 Head" XR -55 55

$hbox 0 "Bip01" -5.530000 -3.540000 -5.530000 5.250000 3.560000 9.760000
$hbox 0 "Bip01 Pelvis" -9.300000 -5.310000 -9.170000 7.370000 6.940000 8.460000
$hbox 0 "Bip01 Spine" -3.040000 -5.050000 -7.670000 7.210000 6.650000 7.680000
$hbox 0 "Bip01 L Thigh" -2.530000 -5.720000 -4.890000 21.060000 6.520000 4.410000
$hbox 0 "Bip01 L Calf" -2.910000 -4.650000 -3.490000 21.310000 3.860000 3.190000
$hbox 0 "Bip01 L Foot" -2.560000 -3.470000 -2.760000 3.980000 6.050000 2.910000
$hbox 0 "Bip01 L Toe0" -0.790000 -0.070000 -2.660000 4.260000 4.377118 2.740000
$hbox 0 "Bip01 R Thigh" -2.530000 -5.730000 -4.460000 21.020000 6.530000 4.740000
$hbox 0 "Bip01 R Calf" -2.890000 -4.610000 -3.490000 21.370000 4.090000 3.370000
$hbox 0 "Bip01 R Foot" -2.590000 -3.430000 -3.120000 4.050000 6.440000 2.850000
$hbox 0 "Bip01 R Toe0" -0.750000 -0.080000 -2.560000 4.620000 3.270000 2.850000
$hbox 0 "Bip01 Spine1" -2.240000 -8.560000 -6.950000 7.210000 6.340000 6.970000
$hbox 0 "Bip01 Spine2" -2.850000 -9.160000 -7.870000 8.650000 6.340000 7.880000
$hbox 0 "Bip01 Spine3" -3.060000 -9.160000 -7.870000 9.300000 6.210000 7.880000
$hbox 0 "Bip01 Neck" -2.863640 -1.620000 -3.171192 1.979600 3.160000 3.171194
$hbox 0 "Bip01 L Clavicle" -0.020000 -4.370000 -3.060000 6.750000 2.510000 2.800000
$hbox 0 "Bip01 L UpperArm" -0.990000 -4.260000 -3.120000 13.470000 2.960000 2.930000
$hbox 0 "Bip01 L Forearm" -1.510000 -2.320000 -2.810000 11.860000 2.970000 1.710000
$hbox 0 "Bip01 L Hand" -1.180000 -1.320000 -1.980000 4.180000 1.840000 1.990000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.770000 -1.560000 2.250000 0.770000 0.850000
$hbox 0 "Bip01 L Finger01" -0.030000 -0.710000 -1.310000 1.780000 0.410000 0.570000
$hbox 0 "Bip01 R Clavicle" -0.060000 -4.480000 -2.800000 6.680000 2.530000 3.060000
$hbox 0 "Bip01 R UpperArm" -1.010000 -4.240000 -2.930000 13.490000 2.960000 3.130000
$hbox 0 "Bip01 R Forearm" -1.490000 -2.140000 -1.690000 11.840000 2.970000 2.810000
$hbox 0 "Bip01 R Hand" -1.200000 -1.320000 -1.990000 4.150000 1.880000 1.980000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.760000 -0.900000 2.230000 0.780000 1.510000
$hbox 0 "Bip01 R Finger01" -0.050000 -0.710000 -0.620000 1.760000 0.410000 1.260000
$hbox 0 "Bip01 Head" -0.670000 -4.230000 -3.620000 8.740000 4.970000 3.700000
$hbox 0 "Cylinder06" -3.510000 -1.550000 -4.790000 3.410000 3.340000 2.930000
$hbox 0 "Cylinder03" -5.100000 -1.040000 -3.500000 3.150000 2.480000 3.420000
$hbox 0 "Cylinder04" -5.630000 -1.100000 -3.220000 5.500000 1.060000 3.460000
$hbox 0 "Cylinder05" -5.640000 -1.100000 -3.220000 5.490000 1.060000 3.460000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
$sequence "idle2" "idle2_ani" ACT_IDLE 2 fps 15 loop
$sequence "wallstrafemount" "wallstrafemount_ani" fps 38
$sequence "wallhugdismount" "wallhugdismount_ani" fps 38
$sequence "wallstrafeidle" "wallstrafeidle_ani" fps 15 loop
$sequence "wallstrafeleft" "wallstrafeleft_ani" AXR AYR fps 38
$sequence "wallhugright" "wallhugright_ani" AXR AYR fps 38
$sequence "wallclimbmount" "wallclimbmount_ani" AZR fps 32
$sequence "wallclimbidle" "wallclimbidle_ani" fps 15 loop
$sequence "wallclimbup" "wallclimbup_ani" AZR
$sequence "wallclimbdown" "wallclimbdown_ani" AZR
$sequence "wallleft" "wallleft_ani" AZR fps 32
$sequence "wallright" "wallright_ani" AZR fps 32
$sequence "wallclimbdis" "wallclimbdis_ani" AZR fps 27
$sequence "cablegrapplemount" "cablegrapplemount_ani" AZR fps 39
$sequence "cablegrapplecycle" "cablegrapplecycle_ani" AXR AYR AZR
$sequence "cablegrappleidle" "cablegrappleidle_ani" fps 25 loop
$sequence "cablegrappleturn" "cablegrappleturn_ani" AXR AYR AZR fps 35
$sequence "cablegrappleend" "cablegrappleend_ani" fps 40
$sequence "cablegrappledis" "cablegrappledis_ani" AZR fps 32
$sequence "sneakforward" "sneakforward_ani" fps 25
$sequence "sneakidle" "sneakidle_ani" fps 22 loop
$sequence "sneakduck" "sneakduck_ani" fps 22 loop
$sequence "sneakdismount" "sneakdismount_ani" fps 19
$sequence "grapplefly" "grapplefly_ani" loop
$sequence "balancemount" "balancemount_ani"
$sequence "balanceidle" "balanceidle_ani" loop
$sequence "balancewalk" "balancewalk_ani"
$sequence "balancetrip" "balancetrip_ani"
$sequence "balanceturn" "balanceturn_ani"
$sequence "balancefall1" "balancefall1_ani"
$sequence "balancefall2" "balancefall2_ani"
$sequence "sneakduckto" "sneakduckto_ani" fps 33
$sequence "sneakduckfrom" "sneakduckfrom_ani" fps 27
$sequence "balancedismount" "balancedismount_ani"
$sequence "ropemount" "ropemount_ani"
$sequence "ropeup" "ropeup_ani"
$sequence "ropedown" "ropedown_ani"
$sequence "ropeidle" "ropeidle_ani" fps 10 loop
$sequence "ropedismount" "ropedismount_ani"
$sequence "ledgemount" "ledgemount_ani" fps 29
$sequence "ledgeidle" "ledgeidle_ani" fps 10 loop
$sequence "ledgeleft" "ledgeleft_ani" fps 32
$sequence "ledgeright" "ledgeright_ani" fps 32
$sequence "ledgedismount" "ledgedismount_ani" fps 32
