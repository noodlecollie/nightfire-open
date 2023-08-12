$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "street_light_01.mdl"

$root "light_pole"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "street_light_ref"
}



$hbox 0 "light_pole" -35.870000 -19.320000 0.000000 106.730000 9.780000 300.000000

$sequence "idle1" "idle1_ani" ACT_IDLE 1
