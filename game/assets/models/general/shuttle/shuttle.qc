$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "shuttle.mdl"

$root "body"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "shuttle_ref"
}



$hbox 0 "body" -351.550000 -600.330000 -233.460000 351.550000 600.330000 233.460000
$hbox 0 "door right" -45.000000 -47.020000 -6.240000 0.100000 47.020000 10.840000
$hbox 0 "door left" -0.100000 -47.020000 -6.220000 45.000000 47.020000 10.840000

$sequence "idle1" "idle1_ani"
$sequence "idle2" "idle2_ani"
