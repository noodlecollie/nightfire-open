$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "rope_dividers.mdl"

$root "pole"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "divider"
{
	studio "rope_divider_01_reference_ref"
	studio "rope_divider_02_reference_ref"
	studio "rope_divider_03_reference_ref"
}



$hbox 0 "pole" -7.270000 -7.180000 0.000000 45.070000 7.450000 41.780000

$sequence "idle1" "idle1_ani"
