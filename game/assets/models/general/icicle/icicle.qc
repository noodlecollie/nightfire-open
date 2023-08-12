$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "icicle.mdl"

$root "icicle"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "icicle"
{
	studio "icicle_01_reference_ref"
	studio "icicle_02_reference_ref"
	studio "icicle_03_reference_ref"
}



$hbox 0 "icicle" -28.570000 -15.470000 -88.530000 32.940000 11.930000 120.500000

$sequence "idle1" "idle1_ani"
$sequence "fall" "fall_ani" fps 15 loop
