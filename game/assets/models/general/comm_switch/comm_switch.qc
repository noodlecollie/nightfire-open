$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "comm_switch.mdl"

$root "panel"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "switch"
{
	studio "comm_switch_reference_ref"
	studio "comm_switch_broken_reference_ref"
}



$hbox 0 "panel" -25.690000 -45.507480 0.000000 26.300000 0.000000 35.990000
$hbox 0 "switch" -3.070000 0.000000 -5.150000 1.020000 32.420000 4.670000

$sequence "idle1" "idle1_ani" fps 15 loop
$sequence "turn_off" "turn_off_ani"
$sequence "off_idle" "off_idle_ani"
