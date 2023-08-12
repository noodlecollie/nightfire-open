$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_commando.mdl"

$root "R_GUN"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_commando_reference_ref"
}
$bodygroup "switch"
{
	studio "v_commando_switch_01_reference_ref"
	studio "v_commando_switch_02_reference_ref"
}
$bodygroup "hands"
{
	studio "v_commando_hands_reference_ref"
	blank
}

$attachment "" "R_GUN" 0.000000 -33.000000 3.000000
$attachment "" "R_GUN" 0.000000 -30.000000 -2.000000
$attachment "" "R_GUN" 2.300000 -80.000000 -2.000000


$hbox 0 "R_GUN" -1.140000 -33.000000 -2.690000 1.160000 7.020000 6.530000
$hbox 0 "R_SCOPE" -1.440000 -8.430000 0.000000 1.440000 0.300000 9.110000
$hbox 0 "R_CLIP" -0.780000 -1.850000 -3.940000 0.780000 1.850000 3.940000
$hbox 0 "Bip01 L Hand" -0.570000 -0.840000 -1.820000 3.879325 1.200000 1.590000
$hbox 0 "Bip01 L Finger0" -0.160000 -0.640000 -0.890000 1.527038 0.400000 0.780000
$hbox 0 "Bip01 L Finger01" 0.000000 -0.320000 -0.490000 1.460000 1.140000 0.550000
$hbox 0 "L_Arm_bone" -10.250000 -2.140000 -1.910000 3.210000 1.820000 1.640000
$hbox 0 "Bip01 R Hand" -0.570000 -0.850000 -1.590000 3.879326 1.250000 2.130000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.580000 -1.310000 1.527040 0.970000 0.810000
$hbox 0 "Bip01 R Finger01" -0.210000 -0.320000 -1.230000 1.430000 0.900000 0.480000
$hbox 0 "R_Arm_bone" -2.520000 -2.150000 -1.840000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 15
$sequence "idle2" "idle2_ani" fps 15
$sequence "fire" "fire_ani" {
	event 5001 0 "60"
	event 5004 0 "weapons/sig552_fire3.wav"
}
$sequence "reload_empty" "reload_empty_ani" fps 45 {
	event 5004 0 "weapons/sig552_reload_empty.wav"
}
$sequence "reload" "reload_ani" fps 45 {
	event 5004 0 "weapons/sig552_reload.wav"
}
$sequence "draw" "draw_ani" {
	event 5004 0 "weapons/sig552_draw.wav"
}
$sequence "HOLSTER" "HOLSTER_ani" fps 15
$sequence "AUTO" "AUTO_ani" {
	event 5004 14 "weapons/sig552_select.wav"
}
$sequence "SEMI" "SEMI_ani" {
	event 5004 14 "weapons/sig552_select.wav"
}
