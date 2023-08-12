$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_pdw90.mdl"

$root "R_GUN"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_pdw90_reference_ref"
}
$bodygroup "hands"
{
	studio "v_pdw90_hands_reference_ref"
	blank
}

$attachment "" "R_GUN" 0.000000 -12.000000 3.000000


$hbox 0 "R_GUN" -1.130000 -11.550000 -2.220000 1.895133 16.334670 8.852154
$hbox 0 "clip" -0.950000 -11.940000 0.000000 0.950000 0.330000 1.010000
$hbox 0 "trigger" 0.000000 0.000000 -2.060000 19.910000 8.650000 0.000000
$hbox 0 "Bip01 L Hand" -0.570000 -0.850000 -2.110000 3.879326 1.190000 1.590000
$hbox 0 "Bip01 L Finger0" -0.150000 -0.640000 -0.880000 1.527038 1.260000 0.980000
$hbox 0 "Bip01 L Finger01" 0.000000 -0.330000 -0.480000 1.460000 1.440000 1.030000
$hbox 0 "L_Arm_bone" -10.250000 -2.150000 -1.910000 3.210000 1.810000 1.650000
$hbox 0 "Bip01 R Hand" -0.570000 -0.860000 -1.590000 3.879327 1.270000 2.170000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.550000 -1.330000 1.527039 1.000000 0.810000
$hbox 0 "Bip01 R Finger01" -0.220000 -0.320000 -1.230000 1.370000 0.900000 0.480000
$hbox 0 "R_Arm_bone" -2.510000 -2.150000 -1.840000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 15
$sequence "idle2" "idle2_ani" fps 15
$sequence "idle3" "idle3_ani" fps 15
$sequence "fire" "fire_ani" {
	event 5001 0 "21"
	event 5004 0 "weapons/p90_fire1.wav"
}
$sequence "reload_empty" "reload_empty_ani" fps 45 {
	event 5004 0 "weapons/p90_reload_empty.wav"
}
$sequence "reload" "reload_ani" fps 45 {
	event 5004 0 "weapons/p90_reload.wav"
}
$sequence "draw" "draw_ani" {
	event 5004 0 "weapons/p90_draw.wav"
}
$sequence "holster" "holster_ani" fps 15 {
	event 5004 0 "weapons/p90_holster.wav"
}
