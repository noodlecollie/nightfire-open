$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "grand_piano.mdl"

$root "piano"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "GRAND_PIANO_REFERENCE_ref"
}



$hbox 0 "arm" -0.630000 -1.450000 -0.790000 2.470000 33.770000 0.790000

$sequence "idle1" "idle1_ani"
$sequence "shot" "shot_ani"
$sequence "idle_shot" "idle_shot_ani"
