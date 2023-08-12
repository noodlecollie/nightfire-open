$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "japanese_bed.mdl"

$root "bed"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "japanese_bed_ref"
}



$hbox 0 "bed" -58.000000 -47.860000 -31.350000 58.000000 47.860000 31.350000
$hbox 0 "plant" -6.180000 -15.600000 -0.250000 6.330000 14.230000 26.420000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
