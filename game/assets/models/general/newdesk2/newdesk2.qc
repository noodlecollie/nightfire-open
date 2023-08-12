$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "newdesk2.mdl"

$root "desk2"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "main"
{
	studio "newdesk2_ref_ref"
}
$bodygroup "desks"
{
	studio "newdesk2_junk_ref_ref"
}



$hbox 0 "desk2" -102.480000 -115.900000 0.000000 107.280000 133.620000 137.730000

$sequence "idle1" "idle1_ani"
