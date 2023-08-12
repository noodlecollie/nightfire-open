$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "silverware.mdl"

$root "butcherknife"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "silverware"
{
	studio "butcherknife_ref"
	studio "steakknife_ref"
	studio "cuttingboard_ref"
	studio "laddle_ref"
}



$hbox 1 "Hanging lantern" 5.000000 0.000000 0.000000 -5.000000 -17.500000 -5.500000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
$sequence "f_hit" "f_hit_ani" loop
$sequence "b_hit" "b_hit_ani"
$sequence "l_hit" "l_hit_ani"
$sequence "r_hit" "r_hit_ani"
