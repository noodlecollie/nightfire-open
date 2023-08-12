$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "hanging_lantern_white.mdl"

$root "Hanging lantern"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "hanging_lantern_white_ref"
}



$hbox 1 "Hanging lantern" -10.000000 -9.000000 -43.000000 10.000000 10.000000 -19.430000

$sequence "idle1" "idle1_ani" fps 10 loop
$sequence "idle2" "idle2_ani" fps 15 loop
$sequence "idle3" "idle3_ani" fps 15 loop
$sequence "f_hit" "f_hit_ani" loop
$sequence "b_hit" "b_hit_ani"
$sequence "l_hit" "l_hit_ani"
$sequence "r_hit" "r_hit_ani"
