$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_tripmine.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "v_tripmine_hands_reference_ref"
}
$bodygroup "tripmine"
{
	studio "v_tripmine_reference_ref"
	blank
}

$attachment "" "tripmine" -1.520000 -1.400000 2.800000


$hbox 0 "Bip01 L Hand" -0.570000 -0.850000 -2.070000 3.879325 1.770000 1.580000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.820000 -0.750000 1.527039 1.050000 1.480000
$hbox 0 "Bip01 L Finger01" -0.160000 -0.330000 -0.710000 1.440000 0.890000 1.710000
$hbox 0 "L_Arm_bone" -10.250000 -2.150000 -1.900000 3.580000 1.810000 1.650000
$hbox 0 "Bip01 R Hand" -0.580000 -0.850000 -1.580000 4.020000 3.400000 2.080000
$hbox 0 "Bip01 R Finger0" -0.020000 -0.590000 -2.630000 4.640000 2.320000 0.730000
$hbox 0 "Bip01 R Finger01" -0.180000 -0.330000 -1.690000 1.440000 0.850000 0.710000
$hbox 0 "R_Arm_bone" -3.040000 -2.150000 -1.840000 10.940000 1.810000 1.710000
$hbox 0 "tripmine" -5.760000 -2.200000 -3.200000 5.780000 2.200000 3.190000

$sequence "laser_idle1" "laser_idle1_ani" fps 10
$sequence "laser_idle2" "laser_idle2_ani" fps 10
$sequence "laser_idle3" "laser_idle3_ani" fps 10
$sequence "laser_fire" "laser_fire_ani"
$sequence "laser_altfire" "laser_altfire_ani" {
	event 5004 0 "weapons/tripmine_switch1.wav"
}
$sequence "laser_draw" "laser_draw_ani" {
	event 5004 0 "weapons/tripmine_draw.wav"
}
$sequence "laser_holster" "laser_holster_ani" {
	event 5004 0 "weapons/tripmine_holster.wav"
}
$sequence "prox_idle1" "prox_idle1_ani" fps 10
$sequence "prox_idle2" "prox_idle2_ani" fps 10
$sequence "prox_idle3" "prox_idle3_ani" fps 10
$sequence "prox_fire" "prox_fire_ani"
$sequence "prox_altfire" "prox_altfire_ani" {
	event 5004 0 "weapons/tripmine_switch2.wav"
}
$sequence "prox_draw" "prox_draw_ani" {
	event 5004 0 "weapons/tripmine_draw.wav"
}
$sequence "prox_holster" "prox_holster_ani" {
	event 5004 0 "weapons/tripmine_holster.wav"
}
