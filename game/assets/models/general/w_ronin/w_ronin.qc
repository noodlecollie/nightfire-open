$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "w_ronin.mdl"

$root "Dummy01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "w_ronin_reference_ref"
}

$attachment "" "barrel" 0.000000 1.000000 9.300000

$controller 0 "gun_yaw" 32800 0 360
$controller 1 "gun_pitch" YR 0 30

$hbox 0 "case_01" -12.820000 -1.450000 0.000000 12.870000 1.353852 21.260000
$hbox 0 "gun_yaw" -2.980000 -2.830000 0.000000 2.970000 2.830000 19.510000
$hbox 0 "gun_pitch" -10.820000 -2.480000 -3.380000 10.360000 2.480000 7.430000
$hbox 0 "barrel" -1.840000 -1.590000 0.000000 1.840000 1.590000 10.830000
$hbox 0 "case_02" -12.470000 -0.010000 0.000000 12.470000 2.180000 20.700000
$hbox 0 "case_03" -12.470000 -2.180000 0.000000 12.470000 0.010000 20.700000

$sequence "idle1" "idle1_ani"
$sequence "spin" "spin_ani"
$sequence "deploy" "deploy_ani"
$sequence "fire" "fire_ani"
$sequence "deploy_idle" "deploy_idle_ani"
