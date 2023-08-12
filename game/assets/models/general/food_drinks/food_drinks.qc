$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "food_drinks.mdl"

$root "plate"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "food_drinks"
{
	studio "food_drinks_plate_ref"
	studio "food_drinks_shrimp_cocktail_ref"
	studio "food_drinks_tray_1_ref"
	studio "food_drinks_wineglass_white_ref"
	studio "food_drinks_wineglass_red_ref"
	studio "food_drinks_fork_ref"
	studio "food_drinks_caviar_ref"
}



$hbox 0 "shrimp_cocktail" -3.710000 -3.230000 -0.030000 3.700000 3.200000 8.650000
$hbox 0 "wineglass_1" -1.650000 -1.430000 0.000000 1.660000 1.430000 7.720000
$hbox 0 "wineglass_2" -1.920000 -1.640000 0.000000 1.900000 1.670000 6.100000

$sequence "idle1" "idle1_ani" loop
