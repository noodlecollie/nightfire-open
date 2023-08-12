$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_hat.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_hat_reference_ref"
}



$hbox 0 "Bip01 L Hand" -0.560000 -0.850000 -2.280000 3.879325 1.730000 1.590000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.460000 -0.760000 1.527039 1.050000 1.240000
$hbox 0 "Bip01 L Finger01" -0.170000 -0.320000 -0.720000 1.400000 1.300000 1.160000
$hbox 0 "L_Arm_bone" -10.250000 -2.140000 -1.900000 3.770000 1.820000 1.650000
$hbox 0 "Bip01 R Hand" -0.570000 -0.860000 -1.590000 3.879327 6.012560 2.550779
$hbox 0 "Bip01 R Finger0" -0.170000 -0.790000 -1.370000 1.527038 1.050000 0.770000
$hbox 0 "Bip01 R Finger01" -0.220000 -0.320000 -1.360000 1.370000 0.720000 0.740000
$hbox 0 "hat" -5.150000 -5.220000 0.000000 5.130000 5.230000 4.360000
$hbox 0 "R_Arm_bone" -3.660000 -2.150000 -1.840000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 10
$sequence "idle2" "idle2_ani" fps 10
$sequence "idle3" "idle3_ani" fps 10
$sequence "fire" "fire_ani" {
	event 93 14 ""
	event 5004 1 "gadgets/hat_throw.wav"
}
$sequence "fire_last" "fire_last_ani" {
	event 1 14 ""
	event 5004 14 "gadgets/hat_throw.wav"
}
$sequence "draw" "draw_ani" fps 40 {
	event 5004 0 "gadgets/pen_draw.wav"
}
$sequence "holster" "holster_ani" {
	event 5004 55 "gadgets/pen_holster.wav"
}
$sequence "idle_off_Screen" "idle_off_Screen_ani" fps 10
