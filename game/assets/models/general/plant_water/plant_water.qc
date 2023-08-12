$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "plant_water.mdl"

$root "Bone01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "stuff"
{
	studio "water_plants_reference_ref"
	studio "water_rock_reference_ref"
	studio "water_branch_reference_ref"
}



$hbox 0 "Bone01" -61.730000 -95.130000 -22.420000 48.720000 37.990000 52.580000
$hbox 0 "Bone02" -18.190000 -9.840000 0.000000 12.850000 10.690000 14.750000
$hbox 0 "Bone03" -18.190000 -9.840000 0.000000 12.850000 10.690000 21.080000
$hbox 0 "Bone04" -18.080000 -9.840000 0.000000 12.970000 10.690000 27.640000

$sequence "idle1" "idle1_ani" fps 6
