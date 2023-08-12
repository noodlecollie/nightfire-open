$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_rocketlauncher.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "v_rocketlauncher_reference_ref"
}
$bodygroup "viewhands"
{
	studio "v_rocketlauncher_hands_reference_ref"
	blank
}

$attachment "" "gun" 0.000000 0.000000 -6.000000


$hbox 0 "Bip01 L Hand" -0.790000 -0.850000 -6.408154 3.879325 1.670000 1.590000
$hbox 0 "gun" -6.220000 -8.380000 -5.575448 3.870000 3.880000 18.291620
$hbox 0 "clip" -5.570000 -5.450000 -0.010000 2.170000 2.290000 17.580000
$hbox 0 "Bip01 L Finger0" -0.320000 -0.670000 -0.760000 1.527037 1.040000 1.390000
$hbox 0 "Bip01 L Finger01" -0.170000 -0.330000 -0.720000 1.410000 1.240000 1.370000
$hbox 0 "L_Arm_bone" -10.250000 -2.150000 -1.910000 4.470000 1.810000 1.640000
$hbox 0 "Bip01 R Hand" -0.600000 -0.850000 -1.580000 3.879327 1.070000 2.400000
$hbox 0 "Bip01 R Finger0" -0.010000 -0.710000 -1.250000 1.530000 1.050000 0.790000
$hbox 0 "Bip01 R Finger01" -0.210000 -0.320000 -1.670000 1.430000 1.120000 0.740000
$hbox 0 "R_Arm_bone" -3.260000 -2.150000 -1.840000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 15
$sequence "idle2" "idle2_ani" fps 15 loop
$sequence "fire" "fire_ani" {
	event 5004 0 "weapons/stinger_fire.wav"
}
$sequence "reload" "reload_ani" fps 45 {
	event 5004 0 "weapons/stinger_reload.wav"
}
$sequence "draw" "draw_ani" {
	event 5004 0 "weapons/stinger_draw.wav"
}
$sequence "holster" "holster_ani"
$sequence "zoomin" "zoomin_ani" fps 20 loop
$sequence "zoomout" "zoomout_ani" fps 20 loop
