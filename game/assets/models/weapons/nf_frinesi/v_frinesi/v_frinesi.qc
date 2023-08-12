$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "v_frinesi.mdl"

$root "Bip01"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

$bodygroup "studio"
{
	studio "v_frinesi_reference_ref"
}
$bodygroup "hands"
{
	studio "v_frinesi_hands_reference_ref"
	blank
}

$attachment "" "Reciever" 0.000000 -29.000000 4.500000

$hbox 0 "Bip01 L Hand" -0.570000 -0.840000 -2.200000 3.879326 1.390000 1.590000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.580000 -0.930000 1.740000 1.280000 0.660000
$hbox 0 "Bip01 L Finger01" -0.130000 -0.570000 -0.560000 1.460000 1.350000 0.690000
$hbox 0 "L_Arm_bone" -10.250000 -2.140000 -1.910000 3.210000 1.820000 1.640000
$hbox 0 "Reciever" -0.960000 -28.000000 -2.190000 0.910685 44.300610 6.890000
$hbox 0 "Stock" -1.950000 -16.880000 -0.850000 0.100000 0.000000 3.480000
$hbox 0 "Stock2" -0.730000 -3.560000 -1.350000 0.740000 0.000000 0.470000
$hbox 0 "Charger" -1.130000 -20.920000 -3.710000 1.390000 0.000000 0.740000
$hbox 0 "shell" -0.530000 -2.760000 -0.530000 0.530000 0.000000 0.530000

$sequence "idle1" "idle1_ani" fps 10
$sequence "shoot" "shoot_ani" {
	event 5001 0 "33"
}
$sequence "shoot_big" "shoot_big_ani" {
	event 5001 0 "33"
}
$sequence "reload" "reload_ani" fps 40
$sequence "pump" "pump_ani" fps 40
$sequence "start_reload" "start_reload_ani" fps 30
$sequence "draw" "draw_ani" {
	event 5004 0 "weapons/weapon_frinesi/frinesi_draw.wav"
}
$sequence "reholster" "reholster_ani"
$sequence "idle2" "idle2_ani" fps 10
$sequence "idle3" "idle3_ani" fps 10
$sequence "idle4" "idle4_ani"
