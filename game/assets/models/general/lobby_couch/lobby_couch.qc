$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "lobby_couch.mdl"

$root "couch_1"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "couch"
{
	studio "lobby_couch_1_ref"
	studio "lobby_couch_2_ref"
	studio "lobby_couch_3_ref"
}



$hbox 0 "couch_1" -30.860000 0.000000 -20.260000 30.860000 40.970000 20.260000
$hbox 0 "couch_2" -52.610000 0.000000 -20.260000 52.610000 40.970000 20.260000
$hbox 0 "couch_3" -71.500000 -20.260000 0.000000 71.500000 20.260000 40.970000

$sequence "idle1" "idle1_ani"
