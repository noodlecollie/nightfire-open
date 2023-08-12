$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "japanese_chair.mdl"

$root "Box04"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "japanese_chair_ref_ref"
}



$hbox 0 "Box04" -14.100000 -17.180000 -13.150000 17.011360 17.180000 13.000000
$hbox 0 "Box02" -12.390000 -12.390000 0.000000 12.390000 12.390000 2.510000
$hbox 0 "Box03" -2.960000 -13.740000 -0.780000 2.960000 13.740000 3.062071
$hbox 0 "armrest tubes" -0.900000 -12.290000 -2.420000 0.900000 12.290000 3.006294
$hbox 0 "armrest1" -2.310000 -16.990000 -0.750000 2.310000 16.990000 0.750000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 25 loop
$sequence "hit1" "hit1_ani"
$sequence "hit2" "hit2_ani"
