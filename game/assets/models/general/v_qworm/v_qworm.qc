$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_qworm.mdl"

$root "Box01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_qworm_reference_ref"
}
$bodygroup "hands"
{
	studio "v_qworm_hands_Reference_ref"
}



$hbox 0 "Bip01 L Hand" -0.560000 -0.850000 -2.280000 3.879327 1.730000 1.590000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.810000 -0.760000 1.527039 1.050000 1.240000
$hbox 0 "Bip01 L Finger01" -0.170000 -0.320000 -0.720000 1.400000 1.290000 1.160000
$hbox 0 "L_Arm_bone" -10.250000 -2.140000 -1.900000 3.370000 1.820000 1.650000
$hbox 0 "Bip01 R Hand" -0.590000 -0.860000 -1.590000 3.879325 1.250000 2.260000
$hbox 0 "Bip01 R Finger0" -0.180000 -0.820000 -1.200000 1.527040 1.050000 0.800000
$hbox 0 "Bip01 R Finger01" -0.210000 -0.330000 -1.190000 1.380000 0.910000 0.760000
$hbox 0 "R_Arm_bone" -3.070000 -2.150000 -1.840000 10.940000 1.810000 1.710000

$sequence "card_draw" "card_draw_ani" {
	event 5004 0 "gadgets/qworm_draw.wav"
}
$sequence "card_holster" "card_holster_ani" {
	event 5004 0 "gadgets/qworm_holster.wav"
}
$sequence "card_idle1" "card_idle1_ani" fps 15
$sequence "card_idle2" "card_idle2_ani" fps 15
$sequence "card_fire_start" "card_fire_start_ani" {
	event 5004 0 "gadgets/qworm_out.wav"
}
$sequence "card_fire_hold" "card_fire_hold_ani" fps 15
$sequence "card_fire_return" "card_fire_return_ani" {
	event 5004 0 "gadgets/qworm_in.wav"
}
$sequence "cd_trans" "cd_trans_ani" {
	event 5004 0 "gadgets/qworm_fire.wav"
}
$sequence "cd_draw" "cd_draw_ani"
$sequence "cd_holster" "cd_holster_ani"
$sequence "cd_idle1" "cd_idle1_ani" fps 15
$sequence "cd_idle2" "cd_idle2_ani" fps 15
$sequence "cd_fire_start" "cd_fire_start_ani"
$sequence "cd_fire_hold" "cd_fire_hold_ani" fps 15
$sequence "cd_fire_hit" "cd_fire_hit_ani"
$sequence "cd_fire_return" "cd_fire_return_ani"
$sequence "card_trans" "card_trans_ani"
