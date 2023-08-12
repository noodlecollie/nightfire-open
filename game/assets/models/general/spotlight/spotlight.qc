$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "spotlight.mdl"

$root "spotlightbase"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "big_spotlight_ref"
}

$attachment "" "spotlight" 0.000000 -16.000000 4.000000
$attachment "" "spotlight" 0.000000 -27.000000 -1.500000

$controller 0 "spotlight" ZR -55 55
$controller 1 "spotlight" XR -55 55

$hbox 0 "spotlight" -7.000000 -23.000000 -5.000000 7.000000 2.000000 13.000000

$sequence "idle" "idle_ani" fps 7 loop
$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 7 loop
$sequence "death" "death_ani" ACT_DIESIMPLE 1 {
	event 2001 1 ""
}
