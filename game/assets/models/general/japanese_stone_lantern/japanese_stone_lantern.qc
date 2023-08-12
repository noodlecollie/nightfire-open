$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "japanese_stone_lantern.mdl"

$root "LANTERN"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "lantern"
{
	studio "japanese_stone_lantern_01_lod1_ref"
	studio "japanese_stone_lantern_02_lod1_ref"
	studio "japanese_stone_statue_01_lod0_ref"
}



$hbox 0 "LANTERN" -16.870000 -19.650000 -0.280000 16.870000 19.590000 100.700000

$sequence "idle1" "idle1_ani"
