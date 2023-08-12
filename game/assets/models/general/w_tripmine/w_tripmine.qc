$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "w_tripmine.mdl"

$root "Box01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "tripmine"
{
	studio "w_tripmine_laser_reference_ref"
	studio "w_tripmine_prox_reference_ref"
}

$attachment "" "Box01" 0.000000 2.200000 0.000000
$attachment "" "Box01" 0.000000 2.200000 100.000000
$attachment "" "Box01" -1.600000 2.800000 1.800000


$hbox 0 "Box01" -5.760000 -2.800000 -2.180000 5.770000 7.640000 2.200000

$sequence "pickup_idle" "pickup_idle_ani"
$sequence "active_idle" "active_idle_ani"
