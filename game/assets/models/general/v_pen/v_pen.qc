$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_pen.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_pen_reference_ref"
}
$bodygroup "hands"
{
	studio "v_pen_hands_reference_ref"
	blank
}



$hbox 0 "Bip01 R Hand" -0.590000 -0.860000 -1.590000 3.879324 1.414893 2.230000
$hbox 0 "Bip01 L Hand" -0.560000 -0.850000 -2.280000 3.879328 1.730000 1.590000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.810000 -0.760000 1.527038 1.050000 1.240000
$hbox 0 "Bip01 L Finger01" -0.170000 -0.320000 -0.720000 1.400000 1.290000 1.160000
$hbox 0 "L_Arm_bone" -10.250000 -2.140000 -1.900000 3.370000 1.820000 1.650000
$hbox 0 "Bip01 R Finger0" -0.280000 -0.820000 -1.010000 1.527040 1.060000 0.810000
$hbox 0 "Bip01 R Finger01" -0.220000 -0.330000 -0.960000 1.380000 0.900000 0.780000
$hbox 0 "R_Arm_bone" -3.080000 -2.150000 -1.840000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 10
$sequence "idle2" "idle2_ani" fps 10
$sequence "idle3" "idle3_ani" fps 10
$sequence "fire" "fire_ani" {
	event 93 13 ""
	event 5004 0 "gadgets/pen_fire.wav"
}
$sequence "fire_last" "fire_last_ani" {
	event 1 13 ""
	event 5004 13 "gadgets/pen_fire.wav"
}
$sequence "draw" "draw_ani" fps 45 {
	event 5004 0 "gadgets/pen_draw.wav"
}
$sequence "holster" "holster_ani" {
	event 5004 0 "gadgets/pen_holster.wav"
}
$sequence "idle_off_Screen" "idle_off_Screen_ani" fps 10
