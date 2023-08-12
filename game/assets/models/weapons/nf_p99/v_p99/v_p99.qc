$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "v_p99.mdl"

$root "Bip01"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

$bodygroup "studio"
{
	studio "v_p99_reference_ref"
}
$bodygroup "silencer"
{
	blank
	studio "v_p99_silencer_reference_ref"
}
$bodygroup "hands"
{
	studio "v_hands_reference_ref"
	blank
}

$attachment "" "R_GUN" 0.000000 -7.500000 3.500000


$hbox 0 "Bip01 R Hand" -0.570000 -0.860000 -1.590000 9.631724 2.101088 4.369772
$hbox 0 "R_GUN" -0.620000 -7.140000 -2.260000 0.640000 2.140000 3.820000
$hbox 0 "R_SLIDE" -0.660000 -8.920000 -1.999992 0.620000 0.070000 2.270000
$hbox 0 "SILENCER" -0.630000 -0.650000 -5.730000 0.650000 0.640000 0.000000
$hbox 0 "Bip01 L Hand" -0.750000 -0.850000 -2.370000 3.879324 1.250000 1.590000
$hbox 0 "Bip01 L Finger0" -0.240000 -0.500000 -0.830000 1.530000 1.050000 1.350000
$hbox 0 "Bip01 L Finger01" -0.150000 -0.320000 -0.760000 1.430000 1.520000 1.220000
$hbox 0 "L_Arm_bone" -10.250000 -2.150000 -1.900000 3.860000 1.810000 1.650000
$hbox 0 "Bip01 R Finger0" -0.240000 -0.790000 -1.320000 1.527036 1.050000 0.770000
$hbox 0 "Bip01 R Finger01" -0.220000 -0.320000 -1.210000 1.430000 0.730000 0.750000
$hbox 0 "R_Arm_bone" -3.640000 -2.150000 -1.840000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 15
$sequence "idle2" "idle2_ani" fps 15
$sequence "idle3" "idle3_ani" fps 15
$sequence "shoot" "shoot_ani" {
	event 5001 1 "22"
}
$sequence "shoot_empty" "shoot_empty_ani" {
	event 5001 1 "22"
}
$sequence "reload" "reload_ani" fps 45 {
	event 5005 0 "weapons/weapon_p99/p99_reload_empty.wav"
}
$sequence "reload_noshot" "reload_noshot_ani" fps 45 {
	event 5005 0 "weapons/weapon_p99/p99_reload.wav"
}
$sequence "draw" "draw_ani" {
	event 5005 0 "weapons/weapon_p99/p99_draw.wav"
}
$sequence "holster" "holster_ani"
$sequence "silencer_draw" "silencer_draw_ani" {
	event 5005 0 "weapons/weapon_p99/p99_draw_sil.wav"
}
$sequence "silencer_holster" "silencer_holster_ani" {
	event 5005 0 "weapons/weapon_p99/p99_holster_sil.wav"
}
$sequence "SILENCER_shoot" "SILENCER_shoot_ani"
$sequence "SILENCER_shoot_empty" "SILENCER_shoot_empty_ani"
