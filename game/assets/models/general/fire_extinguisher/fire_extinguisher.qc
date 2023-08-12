$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "fire_extinguisher.mdl"

$root "firebox0"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "fire_extinguisher_ref"
	studio "fire_extinguisher_damaged_ref"
}



$hbox 0 "firebox0" -4.940000 -7.090000 -12.490000 7.360000 6.030000 12.490000

$sequence "idle1" "idle1_ani"
