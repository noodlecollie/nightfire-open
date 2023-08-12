$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "osato_desk_2.mdl"

$root "desk"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "stuff"
{
	studio "osato_desk2_ref_ref"
	studio "desk2_stuff1_ref_ref"
	studio "desk2_stuff2_ref_ref"
	studio "desk2_stuff3_ref_ref"
	studio "desk2_stuff4_ref_ref"
	studio "desk2_exploded_ref"
}



$hbox 0 "desk" -50.000000 -20.000000 36.000000 50.000000 15.000000 0.000000

$sequence "idle1" "idle1_ani" fps 20
