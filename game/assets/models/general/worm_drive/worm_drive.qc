$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "worm_drive.mdl"

$root "master"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "types"
{
	studio "cd_DeckA_reference_ref"
	studio "cd_deckB_reference_ref"
}
$bodygroup "cd"
{
	blank
	studio "cd_reference_ref"
}



$hbox 0 "master" 0.000000 -4.390256 0.000000 1.572874 0.000000 5.602797
$hbox 0 "tray_drive" -2.697139 -3.003252 -1.374315 0.000000 0.639974 0.030614
$hbox 0 "tray" -3.560000 -0.850000 -2.470000 8.350000 0.220000 5.260000
$hbox 0 "unit" -8.290000 -5.600000 -12.310000 12.680000 3.800000 3.260000
$hbox 0 "cd" -1.560000 -1.530000 0.000000 1.560000 1.530000 3.220000

$sequence "idle_closed" "idle_closed_ani"
$sequence "open" "open_ani"
$sequence "idle_open" "idle_open_ani"
$sequence "close" "close_ani"
