$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "dec_plates_l_gibs.mdl"

$root "Object02"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "parts"
{
	studio "decplate_L_gib1_ref"
	studio "decplate_L_gib2_ref"
	studio "decplate_L_gib3_ref"
	studio "decplate_L_gib4_ref"
	studio "decplate_L_gib5_ref"
}



$hbox 0 "Object02" -2.590000 -4.590000 -0.590000 2.590000 4.590000 0.590000
$hbox 0 "Object04" -2.330000 -4.710000 -0.590000 2.330000 4.710000 0.590000
$hbox 0 "Object05" -3.790000 -2.080000 -0.560000 3.790000 2.080000 0.560000
$hbox 0 "Object01" -3.000000 -1.770000 -0.590000 3.000000 1.770000 0.590000

$sequence "idle1" "idle1_ani"
