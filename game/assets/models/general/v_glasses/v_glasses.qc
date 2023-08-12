$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "v_glasses.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "v_glasses_reference_ref"
}
$bodygroup "hands"
{
	studio "v_glasses_hands_reference_ref"
	blank
}



$hbox 0 "Bip01 R Hand" -0.560000 -0.850000 -1.590000 3.879329 6.320138 2.539178
$hbox 0 "Bip01 R Finger0" 0.000000 -0.570000 -1.210000 1.527040 1.050000 0.800000
$hbox 0 "Bip01 R Finger01" -0.210000 -0.330000 -1.230000 1.430000 0.910000 0.490000
$hbox 0 "R_Arm_bone" -2.520000 -2.140000 -1.840000 10.940000 1.820000 1.710000

$sequence "holster" "holster_ani" {
	event 5004 1 "gadgets/nightvision_off.wav"
}
$sequence "draw" "draw_ani" {
	event 5004 1 "gadgets/nightvision_draw.wav"
	event 5004 41 "gadgets/nightvision_on.wav"
}
