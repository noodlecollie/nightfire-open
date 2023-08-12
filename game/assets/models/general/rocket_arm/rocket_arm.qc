$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "rocket_arm.mdl"

$root "holder"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "rocket_arm_ref_ref"
}



$hbox 0 "holder" -30.510000 -174.890000 -28.490000 30.510000 0.000000 28.760000
$hbox 0 "default" -12.790000 -8.170000 -33.080000 12.790000 236.310000 33.080000
$hbox 0 "grp" -43.020000 -10.120000 -61.960000 43.020000 73.550000 61.960000

$sequence "idle1" "idle1_ani" loop
$sequence "animation" "animation_ani"
