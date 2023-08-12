$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "vpaint.mdl"

$root "Dummy01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "vpaint_ref_ref"
}



$hbox 0 "Dummy01" -3.586103 -11.442490 0.000000 0.000000 0.000000 2.814059
$hbox 0 "Box01" -11.990000 -1.680000 -16.500000 12.000000 0.130000 16.490000

$sequence "idle" "idle_ani"
$sequence "fall" "fall_ani"
$sequence "down" "down_ani"
$sequence "hit1" "hit1_ani"
$sequence "hit2" "hit2_ani"
