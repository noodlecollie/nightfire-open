$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "hanging_Spacesuit.mdl"

$root "HanginSpaceSuit"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "Spacesuit"
{
	studio "hanging_Spacesuit_ref"
	studio "Suithanger_ref"
}



$hbox 0 "HanginSpaceSuit" -14.010000 -20.310000 0.000000 12.790000 13.160000 78.510000

$sequence "idle1" "idle1_ani" ACT_IDLE 1
