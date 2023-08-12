$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "mini_spotlight.mdl"

$root "Bone01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "mini_spotlight_ref"
}



$hbox 0 "Bone01" -2.690000 -1.970000 -5.874546 3.620000 3.630000 1.540000
$hbox 0 "spot_light" -3.640000 -6.400000 -8.220000 3.640000 4.320000 2.280000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 10 loop
$sequence "idle3" "idle3_ani" ACT_IDLE 1 fps 10 loop
