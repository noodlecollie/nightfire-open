$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "japanese_tree_round.mdl"

$root "TREE"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "trees"
{
	studio "TREE_round_01_ref"
	studio "TREE_round_02_ref"
	studio "TREE_round_03_ref"
	studio "TREE_round_01_ref"
	studio "TREE_round_02_ref"
	studio "TREE_round_03_ref"
}



$hbox 0 "TREE" -157.490000 -203.450000 -19.440000 256.110000 175.830000 475.490000

$sequence "idle1" "idle1_ani"
$sequence "idle_offset" "idle_offset_ani"
$sequence "idle_offset_64" "idle_offset_64_ani"
