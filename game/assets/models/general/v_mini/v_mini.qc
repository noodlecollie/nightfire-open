$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_mini.mdl"

$root "R_GUN"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_mini_reference_ref"
}
$bodygroup "hands"
{
	studio "v_mini_hands_reference_ref"
}

$attachment "" "R_GUN" 0.000000 -25.000000 4.000000
$attachment "" "R_GUN" 0.000000 -7.000000 6.800000
$attachment "" "R_GUN" 0.000000 -57.000000 6.800000


$hbox 0 "R_GUN" -1.720000 -17.680000 -2.810000 2.460000 3.580000 5.210000
$hbox 0 "R_CLIP" -5.150000 -0.300000 -6.190000 0.760000 2.470000 0.000000
$hbox 0 "R_CLASP" -0.910000 0.000000 -0.100000 0.420000 1.890000 2.260000
$hbox 0 "R_BARREL" -1.730000 -15.980000 -1.470000 1.730000 0.000000 1.470000
$hbox 0 "Object01" -0.790000 -8.330000 0.000000 0.780000 0.000000 7.660000
$hbox 0 "Bip01 L Hand" -0.570000 -0.850000 -1.870000 3.879327 1.190000 1.590000
$hbox 0 "Bip01 L Finger0" -0.160000 -0.640000 -0.890000 1.527041 0.510000 1.340000
$hbox 0 "Bip01 L Finger01" 0.000000 -0.320000 -0.490000 1.460000 1.150000 0.690000
$hbox 0 "L_Arm_bone" -10.250000 -2.140000 -1.910000 3.210000 1.820000 1.640000
$hbox 0 "Bip01 R Hand" -0.570000 -0.850000 -1.590000 3.879325 1.270000 2.170000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.550000 -1.330000 1.527040 1.000000 0.810000
$hbox 0 "Bip01 R Finger01" -0.220000 -0.320000 -1.230000 1.370000 0.900000 0.490000
$hbox 0 "R_Arm_bone" -2.520000 -2.150000 -1.850000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 15
$sequence "idle2" "idle2_ani" fps 15
$sequence "Spinup" "Spinup_ani" {
	event 5004 0 "weapons/mini_spin_up.wav"
}
$sequence "fire" "fire_ani" {
	event 5001 0 "41"
	event 5004 0 "weapons/mini_fire.wav"
}
$sequence "Spindown" "Spindown_ani" {
	event 5004 0 "weapons/mini_spin_down.wav"
}
$sequence "reload" "reload_ani" {
	event 5004 0 "weapons/mini_reload.wav"
}
$sequence "draw" "draw_ani" {
	event 5004 0 "weapons/mini_draw.wav"
}
$sequence "holster" "holster_ani"
$sequence "spin" "spin_ani"
