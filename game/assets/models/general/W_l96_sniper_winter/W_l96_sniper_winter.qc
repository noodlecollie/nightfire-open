$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "W_l96_sniper_winter.mdl"

$root "Stock"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "W_l96_winter_reference_ref"
}



$hbox 0 "Stock" -20.980000 -4.660000 -1.940000 48.510000 8.680000 1.920000
$hbox 0 "R_Leg" -0.500000 -8.640000 -5.690000 1.650000 0.430000 0.640000
$hbox 0 "L_Leg" -0.490000 -8.670000 -0.630000 1.650000 0.400000 5.700000

$sequence "idle_upright" "idle_upright_ani" fps 1
$sequence "idle_side" "idle_side_ani" fps 1
$sequence "idle_lean" "idle_lean_ani" fps 1
