$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "lantern_gibs_red.mdl"

$root "redgib1"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "lantern_gibs"
{
	studio "redgib1_ref"
	studio "redgib2_ref"
	studio "redgib3_ref"
	studio "redgib4_ref"
}



$hbox 0 "redgib1" -6.560000 -7.590000 -14.340000 6.470000 7.650000 14.420000
$hbox 0 "redgib2" -12.530000 -5.150000 -16.640000 12.530000 7.090000 16.640000
$hbox 0 "redgib3" 0.000000 0.000000 -68.660000 18.350000 16.850000 0.000000
$hbox 0 "redgib4" -3.730000 -3.080000 -8.190000 3.730000 3.080000 8.190000

$sequence "idle1" "idle1_ani" fps 10 loop
$sequence "idle2" "idle2_ani" fps 15 loop
$sequence "idle3" "idle3_ani" fps 15 loop
$sequence "f_hit" "f_hit_ani" loop
$sequence "b_hit" "b_hit_ani"
$sequence "l_hit" "l_hit_ani"
$sequence "r_hit" "r_hit_ani"
