$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_up11.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_up11_reference_ref"
}
$bodygroup "hands"
{
	studio "v_up11_hands_reference_ref"
	blank
}

$attachment "" "gun" 0.000000 -7.500000 3.500000


$hbox 0 "Bip01 R Hand" -0.570000 -0.860000 -1.590000 3.879325 1.430000 2.120000
$hbox 0 "gun" -1.320000 -5.100000 -2.830000 1.330000 2.270000 5.210000
$hbox 0 "clip" -1.360000 -5.480000 -1.970000 1.370000 3.820000 1.070000
$hbox 0 "Bip01 L Hand" -0.570000 -0.850000 -2.290000 3.879325 1.280000 1.590000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.750000 -0.820000 1.730000 1.060000 0.680000
$hbox 0 "Bip01 L Finger01" -0.330000 -0.410000 -0.770000 1.410000 0.790000 0.900000
$hbox 0 "L_Arm_bone" -10.250000 -2.150000 -1.910000 3.210000 1.820000 1.640000
$hbox 0 "Bip01 R Finger0" -0.260000 -0.800000 -1.340000 1.527036 1.050000 0.770000
$hbox 0 "Bip01 R Finger01" -0.220000 -0.320000 -1.250000 1.430000 0.720000 0.740000
$hbox 0 "R_Arm_bone" -3.750000 -2.150000 -1.840000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 15
$sequence "idle2" "idle2_ani" fps 15
$sequence "idle3" "idle3_ani" fps 15
$sequence "shoot" "shoot_ani" {
	event 5004 0 "weapons/up11_fire1.wav"
}
$sequence "reload" "reload_ani" {
	event 5004 0 "weapons/up11_reload.wav"
}
$sequence "draw" "draw_ani" {
	event 5004 0 "weapons/up11_draw.wav"
}
$sequence "holster" "holster_ani" {
	event 5004 0 "weapons/up11_holster.wav"
}
