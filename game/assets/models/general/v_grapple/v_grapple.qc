$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_grapple.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_grapple_reference_ref"
}
$bodygroup "hands"
{
	studio "v_grapple_hands_reference_ref"
	blank
}

$attachment "" "cell" 0.700000 3.300000 0.000000


$hbox 0 "Bip01 R Hand" -0.560000 -0.850000 -1.590000 3.879325 1.450000 2.420000
$hbox 0 "Bip01 L Hand" -0.560000 -0.850000 -2.280000 3.879328 1.300000 1.590000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.420000 -0.770000 2.050000 1.050000 0.840000
$hbox 0 "Bip01 L Finger01" 0.000000 -0.320000 -0.480000 1.520000 1.050000 1.000000
$hbox 0 "L_Arm_bone" -10.250000 -2.140000 -1.900000 3.210000 1.820000 1.650000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.820000 -0.920000 1.527037 1.050000 0.750000
$hbox 0 "Bip01 R Finger01" -0.210000 -0.330000 -0.690000 1.400000 1.130000 0.730000
$hbox 0 "R_Arm_bone" -3.000000 -2.150000 -1.850000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 15
$sequence "FIRE_HOLD" "FIRE_HOLD_ani" fps 10 loop
$sequence "FIRE_RETURN" "FIRE_RETURN_ani"
$sequence "Pull" "Pull_ani" loop
$sequence "DRAW" "DRAW_ani" {
	event 5004 0 "gadgets/grapple_draw.wav"
}
$sequence "HOLSTER" "HOLSTER_ani" {
	event 5004 0 "gadgets/grapple_holster.wav"
}
$sequence "FIRE_START" "FIRE_START_ani" {
	event 5004 0 "gadgets/grapple_fire.wav"
}
