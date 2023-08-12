$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "v_l96a1.mdl"

$root "R_GUN"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "v_l96a1_reference_ref"
}
$bodygroup "hands"
{
	studio "v_l96a1_hands_reference_ref"
}

$attachment "" "R_GUN" 51.000000 4.000000 -0.100000


$hbox 0 "R_GUN" -4.320000 -0.940000 -1.780000 50.480000 7.880000 1.460000
$hbox 0 "R_CLIP" -1.830000 -2.140000 -1.010000 1.610000 1.640000 0.000000
$hbox 0 "R_BOLT" -0.650000 -2.980000 -1.540000 5.770000 0.470000 0.500000
$hbox 0 "Bip01 L Hand" -0.560000 -0.840000 -2.280000 3.820000 1.280000 1.600000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.400000 -0.980000 1.600000 0.630000 0.460000
$hbox 0 "Bip01 L Finger01" -0.220000 -0.440000 -0.810000 1.460000 1.320000 0.610000
$hbox 0 "Bip01 L Finger1" 0.000000 -0.410000 -0.430000 1.460000 1.010000 2.880000
$hbox 0 "Bip01 L Finger11" 0.000000 -0.360000 -0.350000 1.510000 1.360000 2.840000
$hbox 0 "Bip01 L Finger12" 0.000000 -0.140000 -0.190000 1.570000 1.750000 2.620000
$hbox 0 "L_Arm_bone" -10.250000 -2.140000 -1.910000 3.210000 1.820000 1.640000
$hbox 0 "Bip01 R Hand" -0.570000 -0.850000 -1.590000 3.879326 1.430000 2.160000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.650000 -1.320000 1.527037 1.000000 0.790000
$hbox 0 "Bip01 R Finger01" -0.340000 -0.320000 -1.220000 1.460000 0.910000 0.490000
$hbox 0 "R_Arm_bone" -2.520000 -2.150000 -1.840000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 10
$sequence "idle2" "idle2_ani" fps 10
$sequence "fire" "fire_ani" fps 35 {
	event 5005 22 "weapons/weapon_l96a1/l96a1_bolt_pull.wav"
}
$sequence "fire_last" "fire_last_ani"
$sequence "reload" "reload_ani" {
	event 5005 0 "weapons/weapon_l96a1/l96a1_reload.wav"
}
$sequence "reload_charge" "reload_charge_ani" {
	event 5005 0 "weapons/weapon_l96a1/l96a1_reload_empty.wav"
}
$sequence "holster" "holster_ani"
$sequence "draw" "draw_ani" {
	event 5005 0 "weapons/weapon_l96a1/l96a1_draw.wav"
}
