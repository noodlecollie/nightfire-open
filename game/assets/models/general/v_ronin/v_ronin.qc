$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_ronin.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_ronin_reference_ref"
}



$hbox 0 "Bip01 L Hand" -0.860000 -0.850000 -2.430000 3.879326 1.310000 1.590000
$hbox 0 "Bip01 L Finger0" -0.070000 -0.750000 -0.890000 1.527036 1.070000 1.550000
$hbox 0 "Bip01 L Finger01" -0.160000 -0.330000 -0.730000 1.380000 1.560000 1.830000
$hbox 0 "L_Arm_bone" -10.250000 -2.150000 -1.900000 4.520000 1.820000 1.650000
$hbox 0 "Bip01 R Hand" -212.114100 -47.362970 -157.043700 3.879324 1.550000 2.300000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.860000 -1.510000 1.527041 1.050000 0.770000
$hbox 0 "Bip01 R Finger01" -0.190000 -0.320000 -1.930000 1.380000 1.020000 0.730000
$hbox 0 "remote" -0.810000 -4.090000 -3.270000 0.880000 0.700000 3.270000
$hbox 0 "R_Arm_bone" -3.420000 -2.150000 -1.840000 10.940000 1.810000 1.710000
$hbox 0 "case" -10.570000 -2.110000 -7.500000 10.570000 2.100000 7.810000
$hbox 0 "handel" -2.890000 -0.210000 -0.550000 2.890000 2.510000 0.540000

$sequence "case_idle1" "case_idle1_ani" fps 15
$sequence "case_idle2" "case_idle2_ani" fps 15
$sequence "case_idle3" "case_idle3_ani" fps 15
$sequence "case_pull" "case_pull_ani"
$sequence "case_throw" "case_throw_ani" {
	event 5004 0 "weapons/ronin_fire.wav"
}
$sequence "case_deploy" "case_deploy_ani"
$sequence "case_draw" "case_draw_ani" {
	event 5004 0 "weapons/ronin_draw.wav"
}
$sequence "case_holster" "case_holster_ani"
$sequence "detonator_idle1" "detonator_idle1_ani" fps 15
$sequence "detonator_idle2" "detonator_idle2_ani" fps 15
$sequence "detonator_fire_sentry" "detonator_fire_sentry_ani" {
	event 5004 0 "weapons/ronin_remote_on.wav"
}
$sequence "detonator_fire_explode" "detonator_fire_explode_ani" {
	event 5004 0 "weapons/ronin_remote_discard.wav"
}
$sequence "detonator_draw" "detonator_draw_ani" {
	event 5004 0 "weapons/ronin_remote_draw.wav"
}
$sequence "detonator_holster" "detonator_holster_ani" {
	event 5004 0 "weapons/ronin_remote_draw.wav"
}
