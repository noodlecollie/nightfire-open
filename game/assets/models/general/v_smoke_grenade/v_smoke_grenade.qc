$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_smoke_grenade.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_smokegrenade_reference_ref"
}
$bodygroup "viewhands"
{
	studio "v_smokegrenade_hands_reference_ref"
	blank
}



$hbox 0 "Bip01 R Hand" -0.560000 -0.850000 -1.580000 3.879325 1.750000 2.020000
$hbox 0 "grenade" -1.280000 -1.380000 -2.420000 1.280000 1.280000 3.770000
$hbox 0 "Bip01 L Hand" -0.570000 -0.860000 -2.280000 3.879327 1.730000 1.590000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.560000 -0.760000 1.527038 1.050000 1.240000
$hbox 0 "Bip01 L Finger01" -0.170000 -0.320000 -0.710000 1.390000 1.300000 1.160000
$hbox 0 "L_Arm_bone" -10.250000 -2.150000 -1.900000 3.950000 1.820000 1.650000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.490000 -1.290000 1.527038 1.050000 0.700000
$hbox 0 "Bip01 R Finger01" -0.220000 -0.320000 -1.590000 1.380000 1.390000 0.680000
$hbox 0 "R_Arm_bone" -2.910000 -2.150000 -1.840000 10.940000 1.810000 1.710000

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
