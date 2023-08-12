$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "w_laserbeam.mdl"

$root "Master"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "w_laserbeam_reference_ref"
}

$attachment "" "Master" 0.000000 0.000000 0.000000
$attachment "" "Master" 20.000000 0.000000 0.000000


$hbox 0 "laser" -1.540000 -4.960000 0.000000 1.540000 15.100000 2.120000

$sequence "idle1" "idle1_ani"
