$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "w_smoke_grenade.mdl"

$root "Cylinder01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "w_smoke_grenade_reference_ref"
}



$hbox 0 "Cylinder01" -6.030000 -1.830000 -0.030000 4.050000 1.830000 3.620000

$sequence "idle1" "idle1_ani" fps 1
$sequence "roll1" "roll1_ani" loop
$sequence "roll2" "roll2_ani" loop
$sequence "toss" "toss_ani" loop
$sequence "toss2" "toss2_ani" loop
$sequence "toss" "toss_ani" loop
$sequence "toss" "toss_ani" loop
