$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "juicer.mdl"

$root "juicer"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "juicer_ref"
}



$hbox 0 "juicer" -11.710000 -16.110000 -21.430000 11.710000 16.110000 21.430000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
