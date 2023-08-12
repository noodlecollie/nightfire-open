$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_taser.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "v_taser_reference_ref"
}
$bodygroup "hands"
{
	studio "v_taser_hands_reference_ref"
	blank
}

$attachment "" "Object02" -4.000000 0.000000 0.000000
$attachment "" "Object03" -4.000000 0.000000 0.000000


$hbox 0 "Bip01 R Hand" -0.230000 -0.930000 -1.690000 3.820000 1.450000 2.000000
$hbox 0 "Bip01 R Finger01" -0.160000 -0.510000 -0.650000 1.200000 0.670000 0.480000
$hbox 0 "R_Arm_bone" -1.800000 -1.990000 -2.220000 1.820000 1.600000 11.690000

$sequence "idle1" "idle1_ani" fps 15
$sequence "IDLE2" "IDLE2_ani" fps 15
$sequence "FIRE_START" "FIRE_START_ani"
$sequence "FIRE_HOLD" "FIRE_HOLD_ani" fps 15 loop
$sequence "FIRE_RETURN" "FIRE_RETURN_ani"
$sequence "DRAW" "DRAW_ani" {
	event 5004 0 "gadgets/taser_draw.wav"
}
$sequence "HOLSTER" "HOLSTER_ani"
