$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "SingleBed.mdl"

$root "whitebed"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "SingleBed_ref"
}



$hbox 0 "whitebed" -14.180000 -41.350000 -25.840000 14.180000 41.350000 25.840000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
