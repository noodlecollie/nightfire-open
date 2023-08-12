$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "fuse_box.mdl"

$root "box"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "box"
{
	studio "fuse_box_01_reference_ref"
	studio "fuse_box_02_reference_ref"
}



$hbox 0 "box" -11.130000 -23.330000 0.000000 11.140000 23.360000 2.180000

$sequence "idle1" "idle1_ani"
