$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "v_raptor.mdl"

$root "gun"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

$bodygroup "studio"
{
	studio "v_raptor_reference_ref"
}
$bodygroup "hands"
{
	studio "v_raptor_hands_reference_ref"
	blank
}

$attachment "" "gun" 0.000000 -13.000000 3.500000

$hbox 0 "gun" -1.080000 -12.840000 -2.730000 1.000000 2.310000 5.220000
$hbox 0 "R_SLIDE" -6.380000 -1.070000 -1.090000 6.150000 1.580000 1.150000
$hbox 0 "Bip01 L Hand" -0.570000 -0.850000 -2.370000 3.879322 1.260000 1.590000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.710000 -0.820000 1.710000 1.060000 0.710000
$hbox 0 "Bip01 L Finger01" -0.320000 -0.430000 -0.770000 1.430000 0.930000 0.970000
$hbox 0 "L_Arm_bone" -10.250000 -2.150000 -1.910000 3.210000 1.820000 1.650000
$hbox 0 "Bip01 R Hand" -0.610000 -0.860000 -1.590000 3.879325 1.430000 2.120000
$hbox 0 "Bip01 R Finger0" -0.330000 -0.790000 -1.340000 1.527039 1.050000 0.770000
$hbox 0 "Bip01 R Finger01" -0.220000 -0.320000 -1.250000 1.430000 0.720000 0.740000
$hbox 0 "R_Arm_bone" -4.070000 -2.150000 -1.840000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 15
$sequence "idle2" "idle2_ani" fps 15
$sequence "idle3" "idle3_ani" fps 15
$sequence "idle4" "idle4_ani" fps 15
$sequence "shoot" "shoot_ani" {
	event 5001 0 "32"
}
$sequence "reload" "reload_ani" {
	event 5005 0 "weapons/weapon_raptor/raptor_reload.wav"
}
$sequence "shoot_last" "shoot_last_ani" {
	event 5001 0 "32"
}
$sequence "reload" "reload_ani" {
	event 5005 0 "weapons/weapon_raptor/raptor_reload_empty.wav"
}
$sequence "idle1_empty" "idle1_empty_ani" fps 15
$sequence "idle2_empty" "idle2_empty_ani" fps 15
$sequence "idle3_empty" "idle3_empty_ani" fps 15
$sequence "idle4_empty" "idle4_empty_ani" fps 15
$sequence "draw" "draw_ani" {
	event 5005 0 "weapons/weapon_raptor/raptor_draw.wav"
}
$sequence "holster" "holster_ani"
