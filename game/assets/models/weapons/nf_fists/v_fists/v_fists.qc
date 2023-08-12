$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "v_fists.mdl"

$root "Bip01"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

$bodygroup "studio"
{
	studio "v_fists_reference_ref"
}

$hbox 0 "Bip01 L Forearm" 0.000000 -1.140000 -1.040000 11.368420 0.189989 0.000003
$hbox 0 "Bip01 L Hand" -0.290000 -0.810000 -2.000000 3.810000 1.520000 1.690000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.590000 -1.090000 1.660000 0.730000 0.680000
$hbox 0 "L_Arm_bone" -1.750000 -1.960000 -2.240000 1.930000 1.630000 11.680000
$hbox 0 "Bip01 R Hand" -0.230000 -0.930000 -1.690000 3.830000 1.400000 2.000000
$hbox 0 "Bip01 R Finger01" -0.740000 -0.500000 -0.650000 1.220000 0.680000 0.480000
$hbox 0 "R_Arm_bone" -1.790000 -1.980000 -2.220000 1.820000 1.610000 11.690000

$sequence "punch_idle1" "punch_idle1_ani" fps 1 {
	event 5006 1 "fire"
}
$sequence "punch_jab" "punch_jab_ani" fps 40 {
	event 1 7 ""
	event 5006 1 "fire"
}
$sequence "punch_combo" "punch_combo_ani" fps 40 {
	event 1 7 ""
	event 1 22 ""
	event 5006 1 "fire"
	event 5006 20 "fire"
}
$sequence "karate_chop" "karate_chop_ani" fps 35 {
	event 5006 16 "fire"
}
