$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "beach_rock.mdl"

$root "bigrock"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "beach_rock_ref_ref"
	studio "beach_rock2_ref_ref"
	studio "beach_rock3_ref_ref"
}



$hbox 0 "bigrock" -98.220000 -124.430000 -36.080000 118.710000 143.290000 86.510000
$hbox 0 "midrock" -98.220000 -124.430000 -36.080000 98.220000 124.430000 36.080000
$hbox 0 "smallrock	" -106.040000 -128.980000 -23.160000 95.360000 125.370000 17.450000

$sequence "idle1" "idle1_ani" fps 20
