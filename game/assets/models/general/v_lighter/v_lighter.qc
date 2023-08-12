$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_lighter.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "v_lighter_reference_ref"
}
$bodygroup "hands"
{
	studio "v_lighter_hands_reference_ref"
	blank
}

$attachment "" "Mesh01" 0.000000 0.000000 2.700000


$hbox 0 "Bip01 R Hand" -0.570000 -0.850000 -1.590000 3.879325 1.000000 2.100000
$hbox 0 "Bip01 L Hand" -0.770000 -0.850000 -2.430000 3.879325 1.310000 1.590000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.790000 -0.770000 1.527039 1.080000 1.550000
$hbox 0 "Bip01 L Finger01" -0.150000 -0.320000 -0.720000 1.380000 1.560000 1.830000
$hbox 0 "L_Arm_bone" -10.250000 -2.140000 -1.900000 4.130000 1.820000 1.650000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.780000 -1.200000 1.527042 1.050000 0.800000
$hbox 0 "Bip01 R Finger01" 0.000000 -0.330000 -0.790000 1.400000 0.760000 0.480000
$hbox 0 "R_Arm_bone" -2.510000 -2.150000 -1.840000 10.940000 1.810000 1.710000

$sequence "idle1" "idle1_ani" fps 15
$sequence "idle2" "idle2_ani" fps 15
$sequence "flick" "flick_ani" {
	event 1 9 ""
	event 5004 0 "gadgets/cam_light1.wav"
}
$sequence "camera_on" "camera_on_ani" {
	event 5004 0 "gadgets/cam_open.wav"
}
$sequence "camera_idle" "camera_idle_ani"
$sequence "camera_off" "camera_off_ani" {
	event 5004 0 "gadgets/cam_close.wav"
}
$sequence "draw" "draw_ani" {
	event 5004 0 "gadgets/cam_draw.wav"
}
$sequence "holster" "holster_ani"
$sequence "take_picture" "take_picture_ani" {
	event 5004 0 "gadgets/cam_fire.wav"
}
