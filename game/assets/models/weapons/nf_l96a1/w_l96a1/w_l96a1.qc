$cd .
$cdtexture "../../../../../afterburner/textures"
$modelname "w_l96a1.mdl"

$root "Stock"

// No embedded textures
$flags 2048

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "w_l96a1_reference_ref"
}



$hbox 0 "Stock" -16.780000 -3.720000 -1.550000 38.810000 6.940000 1.530000
$hbox 0 "R_Leg" -0.400000 -6.910000 -4.550000 1.320000 0.340000 0.510000
$hbox 0 "L_Leg" -0.390000 -6.940000 -0.500000 1.320000 0.320000 4.560000

$sequence "idle_upright" "idle_upright_ani" fps 1
$sequence "idle_side" "idle_side_ani" fps 1
$sequence "idle_lean" "idle_lean_ani" fps 1
