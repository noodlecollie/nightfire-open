$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "austria_treasure.mdl"

$root "null"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "treasure"
{
	studio "fab_egg_ref"
	studio "dia_necklace_ref"
	studio "pocketwatch_ref"
}



$hbox 0 "null" -4.250000 -5.410000 -11.760000 4.160000 7.360000 9.680000
$hbox 0 "nulll" -10.940000 -10.270000 -2.810000 12.600000 8.710000 2.810000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
