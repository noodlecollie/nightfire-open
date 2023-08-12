$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "osato_gunvan.mdl"

$root "Master"

$flags 2560
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "van"
{
	studio "boss_van_reference_ref"
	studio "boss_van_exploded_reference_ref"
	blank
}

$attachment "" "gunbody" 0.000000 0.000000 34.000000
$attachment "" "gunbody" -29.000000 0.000000 34.000000
$attachment "" "van" -34.000000 -28.000000 -25.000000
$attachment "" "van" 0.000000 75.000000 -45.000000
$attachment "" "van" 0.000000 -105.000000 -10.000000

$controller 0 "gunbody" ZR -45 45

$hbox 0 "Master" -40.000000 -100.000000 0.000000 40.000000 100.000000 50.000000
$hbox 0 "Master" -40.000000 -70.000000 50.000000 40.000000 100.000000 90.000000
$hbox 1 "Master" -50.000000 -20.000000 20.000000 0.000000 25.000000 80.000000

$sequence "idle1" "idle1_ani"
$sequence "idle_open" "idle_open_ani"
$sequence "drive_closed" "drive_closed_ani" loop
$sequence "open" "open_ani"
$sequence "fire" "fire_ani" {
	event 5011 0 "71"
	event 4 0 ""
}
$sequence "drive_fire" "drive_fire_ani" loop {
	event 5011 0 "71"
	event 4 0 ""
	event 5011 5 "71"
	event 5 0 ""
	event 5011 10 "71"
	event 6 0 ""
	event 5011 15 "71"
	event 4 0 ""
}
$sequence "explode" "explode_ani"
$sequence "turn_right" "turn_right_ani"
$sequence "turn_left" "turn_left_ani"
