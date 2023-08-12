$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "osato_gunship.mdl"

$root "Master"

$flags 2560
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "helicopter"
{
	studio "osato_gunship_reference_ref"
}
$bodygroup "rockets"
{
	studio "osato_gunship_rocket_01_ref"
	studio "osato_gunship_rocket_02_ref"
	studio "osato_gunship_rocket_03_ref"
	studio "osato_gunship_rocket_04_ref"
	blank
}

$attachment "" "gun_barrel" 0.000000 -45.000000 0.000000
$attachment "" "gun_barrel" 0.000000 -65.000000 0.000000
$attachment "" "Master" 0.000000 90.000000 0.000000
$attachment "" "Master" -40.000000 -10.000000 130.000000
$attachment "" "Master" -40.000000 -10.000000 -130.000000

$controller 0 "rotor" 32784 0 360
$controller 0 "Rear_rotor" 32776 0 360
$controller 1 "gun" YR -90 90
$controller 2 "gun_barrel" XR -10 45

$hbox 0 "helicopter" -50.000000 -250.000000 -50.000000 50.000000 -330.000000 25.000000
$hbox 0 "helicopter" -50.000000 -190.000000 -50.000000 50.000000 -250.000000 50.000000
$hbox 0 "helicopter" -50.000000 -190.000000 -50.000000 50.000000 130.000000 100.000000
$hbox 0 "helicopter" -30.000000 130.000000 0.000000 30.000000 280.000000 80.000000
$hbox 0 "helicopter" -15.000000 280.000000 30.000000 15.000000 490.000000 80.000000
$hbox 0 "helicopter" -10.000000 440.000000 80.000000 10.000000 540.000000 180.000000
$hbox 0 "helicopter" -200.000000 0.000000 -30.000000 200.000000 130.000000 50.000000

$sequence "idle" "idle_ani"
