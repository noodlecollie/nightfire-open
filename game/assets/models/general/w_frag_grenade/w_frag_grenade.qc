$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "w_frag_grenade.mdl"

$root "Cylinder01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "w_frag_grenade_reference_ref"
}



$hbox 0 "Cylinder01" -5.200000 -2.830000 -0.120000 3.470000 2.830000 5.720000

$sequence "idle1" "idle1_ani" fps 1
$sequence "roll1" "roll1_ani" loop
$sequence "roll2" "roll2_ani" loop
$sequence "toss" "toss_ani" loop
$sequence "toss2" "toss2_ani" loop
$sequence "toss" "toss_ani" loop
$sequence "toss" "toss_ani" loop
