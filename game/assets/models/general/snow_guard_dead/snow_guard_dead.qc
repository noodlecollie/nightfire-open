$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "snow_guard_dead.mdl"

$root "dead_body"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "snow_guard_dead_ref"
	blank
}
$bodygroup "heads"
{
	blank
}
$bodygroup "accessories"
{
	blank
}
$bodygroup "weapons"
{
	blank
}
$bodygroup "skeleton"
{
	blank
	studio "snow_guard_dead_skeleton_ref"
}



$hbox 0 "dead_body" -27.550000 -7.240000 -3.050000 19.580000 8.210000 79.440000
$hbox 0 "skeleton" -26.970000 -4.570000 -42.170000 18.830000 8.240000 39.330000

$sequence "idle1" "idle1_ani"
