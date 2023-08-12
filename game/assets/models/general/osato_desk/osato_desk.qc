$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "osato_desk.mdl"

$root "desk"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "stuff"
{
	studio "osato_desk1_ref_ref"
	studio "osato_deskstuff_01_ref"
	studio "osato_deskstuff_02_ref"
	studio "osato_deskstuff_03_ref"
	studio "osato_deskstuff_04_ref"
	studio "osato_deskstuff_05_ref"
	studio "osato_desk_exploded_ref"
}



$hbox 0 "desk" -50.000000 15.000000 36.000000 50.000000 50.000000 0.000000
$hbox 0 "desk" -50.000000 -50.000000 36.000000 -15.000000 15.000000 0.000000

$sequence "idle1" "idle1_ani" fps 20
