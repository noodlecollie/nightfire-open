$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "w_energyball.mdl"

$root "Master"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "w_energyball_reference_ref"
}

$attachment "" "Box01" 0.000000 0.000000 0.000000


$hbox 0 "Box01" -12.000000 -12.000000 -12.000000 12.000000 12.000000 12.000000

$sequence "idle1" "idle1_ani"
