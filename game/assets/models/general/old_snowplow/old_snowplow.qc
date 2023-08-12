$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "old_snowplow.mdl"

$root "old_plowtr"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "truck"
{
	studio "old_snowplow_ref"
}



$hbox 0 "old_plowtr" -50.460000 -136.270000 -69.420000 50.460000 132.730000 55.160000
$hbox 0 "default" -39.860000 -19.260000 -22.180000 39.860000 36.190000 8.840000

$sequence "idle" "idle_ani" ACT_IDLE 1 fps 20 loop
