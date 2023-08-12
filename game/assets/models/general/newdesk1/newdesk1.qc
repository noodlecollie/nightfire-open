$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "newdesk1.mdl"

$root "desk1"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "main"
{
	studio "newdesk1_ref_ref"
}
$bodygroup "desks"
{
	studio "newdesk1_junk_ref_ref"
}



$hbox 0 "desk1" -104.920000 -108.770000 0.000000 104.000000 123.770000 131.300000

$sequence "idle1" "idle1_ani"
