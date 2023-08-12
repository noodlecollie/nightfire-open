$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "v_grenadelauncher.mdl"

$root "R_GUN"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "v_grenadelauncher_reference_ref"
}
$bodygroup "viewhands"
{
	studio "v_grenadelauncher_hands_referenc_ref"
	blank
}

$attachment "" "R_GUN" 0.000000 -26.000000 4.000000


$hbox 0 "R_GUN" -3.650000 -25.040000 -2.430000 3.870000 3.600000 7.200000
$hbox 0 "handle" -3.280000 0.000000 -0.350000 0.400000 4.940000 3.360000
$hbox 0 "rotator" -3.760000 0.000000 -3.760000 3.760000 4.660000 3.760000
$hbox 0 "Bip01 L Hand" -0.560000 -0.850000 -2.230000 3.879325 1.270000 1.600000
$hbox 0 "Bip01 L Finger0" -0.090000 -0.810000 -0.820000 1.527039 1.060000 1.220000
$hbox 0 "Bip01 L Finger01" -0.160000 -0.310000 -0.760000 1.510000 0.910000 1.230000
$hbox 0 "L_Arm_bone" -10.250000 -2.140000 -1.900000 3.290000 1.820000 1.650000
$hbox 0 "Bip01 R Hand" -0.560000 -0.850000 -1.600000 3.879325 1.110000 2.230000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.560000 -1.270000 1.600000 1.060000 0.800000
$hbox 0 "Bip01 R Finger01" 0.000000 -0.310000 -0.950000 1.510000 0.910000 0.480000
$hbox 0 "R_Arm_bone" -2.520000 -2.140000 -1.840000 10.940000 1.820000 1.710000

$sequence "idle1" "idle1_ani" fps 15
$sequence "idle2" "idle2_ani" fps 15 loop
$sequence "fire" "fire_ani" {
	event 5001 0 "33"
}
$sequence "reload" "reload_ani" fps 45 {
	event 5005 0 "weapons/weapon_grenadelauncher/grenade_launcher_reload.wav"
}
$sequence "draw" "draw_ani" {
	event 5005 0 "weapons/weapon_grenadelauncher/grenade_launcher_draw.wav"
}
$sequence "holster" "holster_ani" {
	event 5005 0 "weapons/weapon_grenadelauncher/grenade_launcher_holster.wav"
}
