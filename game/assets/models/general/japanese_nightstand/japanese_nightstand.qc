$cdtexture "../../../../content/nfopen/textures"
$cd "."
$modelname "japanese_nightstand.mdl"
$noembeddedtextures

$root "nightstand"

$flags 0
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "japanese_nightstand_ref"
}



$hbox 0 "nightstand" -15.720000 -21.990000 -15.290000 15.720000 21.990000 15.290000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
