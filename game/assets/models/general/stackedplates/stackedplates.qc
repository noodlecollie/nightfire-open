$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "stackedplates.mdl"

$root "platestack"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "stackedplates"
{
	studio "stackedplates1_ref"
	studio "stackedplates4_ref"
	studio "stackedplates_ref"
}



$hbox 0 "platestack" -6.150000 -6.150000 0.000000 6.150000 6.150000 4.320000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
