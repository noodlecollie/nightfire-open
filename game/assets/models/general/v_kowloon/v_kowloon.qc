$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_kowloon.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_kowloon_reference_no_laser_ref"
}
$bodygroup "hands"
{
	studio "v_kowloon_hands_reference_ref"
	blank
}

$attachment "" "R_GUN" -8.500000 3.000000 0.000000


$hbox 0 "Bip01 R Hand" -0.570000 -0.860000 -1.590000 3.879324 1.470000 2.110000
$hbox 0 "R_GUN" -7.840000 -2.690000 -0.660000 2.190000 3.338482 0.660000
$hbox 0 "R_SLIDE" -4.580000 -0.450000 -0.090000 4.660000 0.920000 1.250000
$hbox 0 "R_CLIP" -0.070000 -2.040000 -2.590000 0.940000 1.370000 2.400000
$hbox 0 "Mesh01" -7.520000 0.000000 -0.600000 0.000000 2.010000 0.640000
$hbox 0 "Bip01 L Hand" -0.740000 -0.850000 -2.340000 3.879325 1.220000 1.590000
$hbox 0 "Bip01 L Finger0" -0.280000 -0.490000 -0.810000 1.530000 1.050000 1.320000
$hbox 0 "Bip01 L Finger01" -0.150000 -0.320000 -0.760000 1.430000 1.490000 1.160000
$hbox 0 "L_Arm_bone" -10.250000 -2.150000 -1.900000 3.850000 1.810000 1.650000
$hbox 0 "Bip01 R Finger0" -0.210000 -0.790000 -1.370000 1.527040 1.050000 0.770000
$hbox 0 "Bip01 R Finger01" -0.220000 -0.320000 -1.310000 1.430000 0.730000 0.740000
$hbox 0 "R_Arm_bone" -3.640000 -2.150000 -1.840000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 15
$sequence "idle2" "idle2_ani" fps 15
$sequence "idle3" "idle3_ani" fps 15
$sequence "shoot" "shoot_ani" {
	event 5001 1 "22"
	event 5004 1 "weapons/kowloon_fire1.wav"
}
$sequence "shoot_empty" "shoot_empty_ani" fps 45 {
	event 5001 1 "22"
	event 5004 1 "weapons/kowloon_fire1.wav"
}
$sequence "reload" "reload_ani" fps 45 {
	event 5004 1 "weapons/p99_reload_empty.wav"
}
$sequence "reload_noshot" "reload_noshot_ani" fps 45 {
	event 5004 1 "weapons/p99_reload.wav"
}
$sequence "draw" "draw_ani" {
	event 5004 1 "weapons/p99_draw.wav"
}
$sequence "holster" "holster_ani"
