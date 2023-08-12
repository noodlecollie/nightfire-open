$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "keycard.mdl"

$root "keycard"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "keycard_ref"
}



$hbox 0 "keycard" -4.220000 -1.920000 0.000000 4.230000 1.920000 2.280000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
