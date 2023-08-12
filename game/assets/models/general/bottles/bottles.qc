$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "bottles.mdl"

$root "oil"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "bottles"
{
	studio "olive_oil_bottle_ref"
	studio "olive_oil_bottle2_ref"
	studio "vinegar_ref"
	studio "spice1_ref"
	studio "spice2_ref"
}



$hbox 0 "oil" -1.760000 -1.760000 0.000000 1.760000 1.760000 12.740000
$hbox 0 "oil2" -2.730000 -1.290000 0.000000 2.730000 1.290000 9.330000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 10 loop
