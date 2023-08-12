$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "rope.mdl"

$root "mesh_rope_coilShape"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "rope_ref"
}



$hbox 0 "mesh_rope_coilShape" -3.190000 -11.360000 -11.360000 3.190000 11.360000 11.360000

$sequence "idle1" "idle1_ani" ACT_IDLE 1
