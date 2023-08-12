$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "fish_01.mdl"

$root "Bone01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "lod"
{
	studio "grouper_lod0_ref"
	studio "grouper_lod1_ref"
	studio "grouper_lod2_ref"
	studio "grouper_lod3_ref"
	blank
}



$hbox 0 "Bone01" -7.580000 -24.640000 -16.050000 7.480000 11.440000 10.770000
$hbox 0 "Bone05" -3.710000 -30.980000 -13.530000 3.750000 0.000000 14.310000
$hbox 0 "Bone06" -1.940000 -15.540000 -13.530000 1.990000 11.420000 14.310000
$hbox 0 "Bone07" -1.030000 -16.860000 -8.950000 1.080000 0.000000 8.580000

$sequence "swim" "swim_ani" ACT_SWIM 1 LX loop
