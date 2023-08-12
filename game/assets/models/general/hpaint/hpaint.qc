$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "hpaint.mdl"

$root "Dummy02"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "hpaint_ref_ref"
}



$hbox 0 "Dummy02" 0.000000 -7.876953 0.000000 3.848415 0.000000 2.726852
$hbox 0 "Box03" -18.050000 -1.680000 -13.000000 17.890000 0.130000 13.000000

$sequence "idle" "idle_ani"
$sequence "fall" "fall_ani" fps 40
$sequence "down" "down_ani"
$sequence "hit1" "hit1_ani"
$sequence "hit2" "hit2_ani"
