$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "coffee_cups.mdl"

$root "full_coffee"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "cups"
{
	studio "full_cup_ref"
	studio "cups_stacked_tall_ref"
	studio "cups_stacked_short_ref"
}



$hbox 0 "full_coffee" -2.220000 -1.920000 0.000000 2.230000 1.920000 8.280000
$hbox 0 "full_coffee04" -2.250000 -1.950000 -12.080000 2.250000 1.950000 0.000000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
