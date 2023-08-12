$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "sec_camera1.mdl"

$root "Bone01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "camera"
{
	studio "sec_camera1_reference_ref"
	studio "sec_camera1_destroyed_reference_ref"
}

$attachment "" "camera" 0.000000 -18.000000 -6.000000
$attachment "" "camera" 0.000000 -15.000000 -13.000000
$attachment "" "camera" 0.000000 -21.000000 -15.000000

$controller 0 "camera" ZR -55 55
$controller 1 "camera" XR -45 45

$hbox 0 "Bone01" -2.180000 -2.000000 -9.000000 3.370000 3.540000 2.590000
$hbox 0 "Bone02" 0.000000 0.000000 -2.280000 2.320000 1.650000 0.000000
$hbox 0 "Bone03" 0.000000 0.000000 -2.104117 1.056766 3.180000 0.000000
$hbox 0 "Bone04" -1.640000 0.000000 -2.052561 0.000000 3.820000 0.000000
$hbox 0 "Bone05" -1.768495 0.000000 -1.812411 0.000000 1.341293 0.000000
$hbox 0 "Bone06" -1.150000 0.000000 0.000000 0.000000 1.160000 1.140000
$hbox 0 "camera mount" -2.810000 -3.030000 -11.440000 2.810000 2.310000 0.150000
$hbox 0 "camera" -8.200000 -19.040000 -22.280000 8.780000 7.610000 1.100000

$sequence "idle1" "idle1_ani" ACT_IDLE 1
$sequence "idle_static" "idle_static_ani"
