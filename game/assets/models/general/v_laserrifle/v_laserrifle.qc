$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_laserrifle.mdl"

$root "R_GUN"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_laser_reference_ref"
}
$bodygroup "hands"
{
	studio "v_laser_hands_reference_ref"
	blank
}

$attachment "" "R_GUN" 0.000000 -24.000000 2.500000

$controller 0 "R_charge" Y -100 0

$hbox 0 "R_GUN" -1.550000 -23.610000 -3.810000 1.400000 6.270000 5.820000
$hbox 0 "Bip01 L UpperArm" 0.000000 -3.430000 -2.740000 11.910000 2.950000 3.540000
$hbox 0 "Bip01 L Hand" -2.800000 -1.460000 -2.460000 3.879325 1.510000 2.220000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.650000 -1.240000 1.527039 1.140000 0.560000
$hbox 0 "Bip01 L Finger01" 0.000000 -0.390000 -0.530000 1.490000 1.200000 1.010000
$hbox 0 "Bip01 L Finger02" -0.080000 -0.390000 -0.520000 1.590000 0.660000 0.570000
$hbox 0 "L_Arm_bone" -8.810000 -2.650000 -2.250000 3.230000 3.400000 2.570000

$sequence "idle1" "idle1_ani" fps 15
$sequence "IDLE2" "IDLE2_ani" fps 15
$sequence "FIRE" "FIRE_ani" {
	event 5004 0 "weapons/laser_fire1.wav"
	event 5001 0 "34"
}
$sequence "FIRE_START" "FIRE_START_ani" {
	event 5004 0 "weapons/laser_recharge.wav"
}
$sequence "FIRE_HOLD" "FIRE_HOLD_ani"
$sequence "FIRE_RETURN" "FIRE_RETURN_ani" {
	event 5004 8 "weapons/laser_alt_fire1.wav"
}
$sequence "DRAW" "DRAW_ani" {
	event 5004 0 "weapons/laser_draw.wav"
}
$sequence "HOLSTER" "HOLSTER_ani"
