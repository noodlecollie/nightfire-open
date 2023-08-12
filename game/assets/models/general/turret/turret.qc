$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "turret.mdl"

$root "default"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "turret_ref_ref"
}

$attachment "" "guns" 12.200000 -34.000000 3.800000
$attachment "" "guns" 12.200000 -34.000000 -3.500000
$attachment "" "guns" -12.200000 -34.000000 3.800000
$attachment "" "guns" -12.200000 -34.000000 -3.500000

$controller 0 "swivelarm" 32800 0 360
$controller 1 "guns" XR -30 30

$hbox 0 "default" -46.960000 -46.960000 -21.150000 46.960000 46.960000 8.400000
$hbox 0 "dome" -31.060000 -31.060000 -31.760000 31.060000 31.060000 31.760000
$hbox 0 "swivelarm" -12.940000 -5.180000 -4.150000 12.940000 5.180000 4.150000
$hbox 0 "guns" -22.950000 -33.490000 -12.180000 22.950000 18.700000 12.500000

$sequence "idle" "idle_ani"
$sequence "deployed" "deployed_ani" fps 15 {
	event 5004 1 "weapons/clamturret_open.wav"
}
$sequence "firing" "firing_ani" {
	event 5004 1 "weapons/clamturret_fire1.wav"
	event 5001 1 "31"
	event 5011 3 "31"
	event 5021 5 "31"
	event 5031 7 "31"
}
$sequence "death" "death_ani" fps 15 {
	event 5004 1 "weapons/clamturret_close.wav"
}
$sequence "deploy_idle" "deploy_idle_ani"
