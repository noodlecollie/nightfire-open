$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "flat_monitor_2.mdl"

$root "flat_moniter_1"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "monitor"
{
	studio "flat_monitor_2a_ref"
	studio "flat_monitor_2b_ref"
	studio "flat_monitor_2c_ref"
	studio "flat_monitor_2d_ref"
}



$hbox 0 "flat_moniter_1" -11.850000 -5.820000 0.000000 11.850000 5.820000 21.830000
$hbox 0 "screen" -9.180000 -4.080000 0.000000 9.180000 0.000000 19.330000

$sequence "idle1" "idle1_ani"
$sequence "shot" "shot_ani"
