$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "small_rockv01.mdl"

$root "default"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "small_rockv01_ref_ref"
	studio "small_rockv012_ref_ref"
}



$hbox 0 "default" -40.220000 -39.290000 -40.160000 40.220000 39.290000 0.000000
$hbox 0 "r2" -26.810000 -26.190000 -20.180000 26.810000 26.190000 0.000000

$sequence "idle" "idle_ani" fps 20
