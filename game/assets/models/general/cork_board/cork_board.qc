$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "cork_board.mdl"

$root "cb"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "paper"
{
	studio "cork_board_01_ref"
	studio "cork_board_02_ref"
	studio "cork_board_03_ref"
	studio "cork_board_04_ref"
	studio "cork_board_exploded_ref"
}



$hbox 0 "cb" -35.000000 -5.000000 -26.000000 35.000000 2.000000 22.000000

$sequence "idle" "idle_ani" fps 20
