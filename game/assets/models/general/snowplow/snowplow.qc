$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "snowplow.mdl"

$root "old_plowtr"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "snowplow_ref_ref"
}
$bodygroup "plow"
{
	blank
	studio "snowplow_plow_ref"
}
$bodygroup "top"
{
	blank
	studio "snowplow_top_ref"
}



$hbox 0 "old_plowtr" -50.460000 -136.270000 -69.420000 50.460000 132.730000 27.760000
$hbox 0 "plow" -39.860000 -19.260000 -22.180000 39.860000 36.190000 8.840000
$hbox 0 "canapy" -45.690000 -24.100000 -22.060000 45.200000 132.480000 55.160000

$sequence "idle1" "idle1_ani" fps 20
