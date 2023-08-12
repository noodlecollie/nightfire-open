$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_watch.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "v_watch_laser_reference_ref"
}

$attachment "" "L_WATCH" 0.000000 0.800000 1.300000
$attachment "" "L_WATCH" 1.000000 0.000000 1.200000

$controller 0 "HHAND" 32800 0 360
$controller 1 "MHAND" 32800 0 360
$controller 2 "SHAND" 32800 0 360

$hbox 0 "Bip01 L Hand" -0.240000 -0.820000 -1.740000 3.820000 1.180000 1.610000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.790000 -1.120000 1.500000 0.820000 0.630000
$hbox 0 "L_Arm_bone" -2.100000 -1.780000 -4.095620 1.910000 2.280000 10.690000
$hbox 0 "L_WATCH" -0.940000 -1.390000 -0.850000 0.940000 1.350000 1.441820
$hbox 0 "Bip01 R Hand" -0.230000 -0.860000 -1.690000 3.820000 1.140000 1.680000
$hbox 0 "Bip01 R Finger0" -0.370000 -0.790000 -0.660000 1.500000 0.920000 1.150000
$hbox 0 "R_Arm_bone" -1.730000 -2.000000 -2.550000 1.900000 1.630000 11.370000

$sequence "idle1" "idle1_ani" loop
$sequence "IDLE2" "IDLE2_ani"
$sequence "FIRE_START" "FIRE_START_ani" fps 15
$sequence "FIRE_HOLD" "FIRE_HOLD_ani" fps 10 loop
$sequence "FIRE_RETURN" "FIRE_RETURN_ani" fps 15
$sequence "DRAW" "DRAW_ani" {
	event 5004 0 "gadgets/watch_draw.wav"
}
$sequence "HOLSTER" "HOLSTER_ani"
$sequence "laser_grapple" "laser_grapple_ani"
$sequence "grapple_laser" "grapple_laser_ani"
$sequence "idle1" "idle1_ani" loop
$sequence "FIRE_START" "FIRE_START_ani" fps 15 {
	event 5004 0 "gadgets/grapple_fire.wav"
}
$sequence "FIRE_HOLD" "FIRE_HOLD_ani" fps 10 loop
$sequence "FIRE_RETURN" "FIRE_RETURN_ani" fps 15
$sequence "DRAW" "DRAW_ani" {
	event 5004 0 "gadgets/watch_draw.wav"
}
$sequence "HOLSTER" "HOLSTER_ani"
