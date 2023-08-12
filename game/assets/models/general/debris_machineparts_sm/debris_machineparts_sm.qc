$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "debris_machineparts_sm.mdl"

$root "Cylinder02"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "parts"
{
	studio "debris_machineparts01_ref_ref"
	studio "debris_machineparts02_ref_ref"
	studio "debris_machineparts03_ref_ref"
	studio "debris_machineparts04_ref_ref"
	studio "debris_machineparts05_ref_ref"
	studio "debris_machineparts06_ref_ref"
	studio "debris_machineparts07_ref_ref"
	studio "debris_machineparts08_ref_ref"
	studio "debris_machineparts09_ref_ref"
}



$hbox 0 "Box05" -2.600000 -2.560000 -0.910000 2.600000 2.560000 0.910000
$hbox 0 "Object08" -0.530000 -5.040000 -1.850000 0.530000 5.040000 1.850000
$hbox 0 "Cylinder01" -1.270000 -1.210000 -1.590000 1.270000 1.210000 1.590000
$hbox 0 "gib" -5.300000 -2.010000 -1.420000 5.300000 2.010000 1.420000

$sequence "idle1" "idle1_ani"
