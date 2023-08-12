$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "Suithanger.mdl"

$root "HanginSpaceSuit"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "Suithanger_ref"
}



$hbox 0 "HanginSpaceSuit" -12.790000 -11.100000 0.000000 12.790000 11.100000 72.920000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
