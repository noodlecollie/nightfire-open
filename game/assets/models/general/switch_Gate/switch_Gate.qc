$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "switch_Gate.mdl"

$root "panel01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "switch"
{
	studio "SWITCH_GATE_rEFERENCE_ref"
	studio "SWITCH_GATE_damaged_rEFERENCE_ref"
}



$hbox 0 "panel01" -25.700000 -37.895970 -24.002360 26.300000 0.000000 35.990000
$hbox 0 "message03" -20.380000 -22.680000 0.000000 19.970000 0.000000 32.910000
$hbox 0 "message04" -20.380000 -22.680000 0.000000 19.970000 0.000000 32.910000
$hbox 0 "switch01" -1.370000 0.000000 -6.480000 0.600000 26.670000 6.350000

$sequence "idle1" "idle1_ani"
$sequence "idle_off" "idle_off_ani"
$sequence "turn_on" "turn_on_ani"
$sequence "idle_on" "idle_on_ani"
$sequence "turn_off" "turn_off_ani"
$sequence "damaged" "damaged_ani"
