$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_frag_grenade.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "hands"
{
	studio "v_viewhands_reference_ref"
}
$bodygroup "weapon"
{
	studio "v_frag_grenade_reference_ref"
}



$hbox 0 "Bip01 L Hand" -0.560000 -0.850000 -2.230000 3.879325 1.270000 1.600000
$hbox 0 "Bip01 L Finger0" -0.090000 -0.780000 -0.820000 1.527039 1.060000 1.220000
$hbox 0 "Bip01 L Finger01" -0.160000 -0.310000 -0.760000 1.510000 0.910000 1.230000
$hbox 0 "L_Arm_bone" -10.250000 -2.140000 -1.900000 3.900000 1.820000 1.650000
$hbox 0 "Bip01 R Hand" -0.560000 -0.850000 -1.600000 3.879325 2.033960 2.230000
$hbox 0 "Bip01 R Finger0" -0.030000 -0.770000 -1.220000 1.527039 1.060000 0.800000
$hbox 0 "Bip01 R Finger01" -0.220000 -0.310000 -1.230000 1.510000 0.910000 0.760000
$hbox 0 "R_Arm_bone" -2.870000 -2.140000 -1.840000 10.940000 1.820000 1.710000
$hbox 0 "grenade" -1.710000 -1.700000 -1.670000 1.700000 1.710000 3.770000

$sequence "idle1" "idle1_ani" fps 10
$sequence "idle2" "idle2_ani" fps 10
$sequence "idle3" "idle3_ani" fps 10
$sequence "pull" "pull_ani" {
	event 5004 17 "weapons/grenade_pin.wav"
}
$sequence "throw" "throw_ani" {
	event 1 18 ""
	event 5004 12 "weapons/grenade_throw.wav"
}
$sequence "throw_last" "throw_last_ani" {
	event 1 18 ""
	event 5004 12 "weapons/grenade_throw.wav"
}
$sequence "draw" "draw_ani" {
	event 5004 0 "weapons/grenade_draw.wav"
}
$sequence "holster" "holster_ani" fps 15
