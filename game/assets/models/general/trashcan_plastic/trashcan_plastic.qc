$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "trashcan_plastic.mdl"

$root "can"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "trash"
{
	blank
	studio "trashcan_plastic_trash_ref"
}
$bodygroup "studio"
{
	studio "trashcan_plastic_ref"
}



$hbox 0 "trashcan" -5.920000 -7.080000 -3.820000 5.800000 7.110000 2.420000
$hbox 0 "Object01" -6.760000 -7.730000 0.000000 6.230000 8.260000 18.090000

$sequence "idle1" "idle1_ani"
