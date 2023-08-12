$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_pda.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_pda_reference_ref"
}
$bodygroup "hands"
{
	studio "v_pda_hands_Reference_ref"
}



$hbox 0 "Bip01 L Hand" -0.550000 -0.850000 -2.330000 3.879325 1.846146 1.590000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.470000 -0.770000 1.530000 1.090000 1.340000
$hbox 0 "Bip01 L Finger01" 0.000000 -0.320000 -0.490000 1.520000 1.090000 0.900000
$hbox 0 "L_Arm_bone" -10.250000 -2.150000 -1.910000 3.210000 1.820000 1.640000
$hbox 0 "Bip01 R Hand" -0.560000 -0.860000 -1.590000 3.879326 1.580000 2.080000
$hbox 0 "Bip01 R Finger0" -0.010000 -0.660000 -1.440000 1.527039 1.050000 0.760000
$hbox 0 "Bip01 R Finger01" -0.210000 -0.320000 -1.450000 1.370000 0.730000 0.730000
$hbox 0 "R_Arm_bone" -3.160000 -2.150000 -1.840000 10.940000 1.820000 1.710000

$sequence "idle1" "idle1_ani" fps 15
$sequence "draw" "draw_ani" {
	event 5004 0 "gadgets/qdeck_draw.wav"
}
$sequence "holster" "holster_ani"
$sequence "select_options" "select_options_ani" fps 40 {
	event 5004 0 "gadgets/qdeck_fire2.wav"
}
$sequence "deselect_options" "deselect_options_ani" fps 40 {
	event 5004 0 "gadgets/qdeck_fire.wav"
}
$sequence "options_idle" "options_idle_ani" fps 15.5 {
	event 5004 0 "gadgets/qdeck_tap1.wav"
}
$sequence "options_out" "options_out_ani" fps 15
$sequence "options_idle1" "options_idle1_ani" fps 15
$sequence "options_idle2" "options_idle2_ani" fps 15
$sequence "options_idle3" "options_idle3_ani" fps 15
