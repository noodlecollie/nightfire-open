$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "sec_computer.mdl"

$root "sec_computer"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "sec_computer_ref"
}



$hbox 0 "sec_computer" -17.070000 -19.910000 -6.400000 17.070000 19.910000 6.400000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
