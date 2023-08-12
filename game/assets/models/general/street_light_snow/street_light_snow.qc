$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "street_light_snow.mdl"

$root "light_pole"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "rocks"
{
	studio "street_light_01_unmasked_referen_ref"
	studio "street_light_02_unmasked_referen_ref"
	studio "street_light_03_unmasked_referen_ref"
	studio "street_light_snow_04_reference_ref"
	studio "street_light_snow_05_reference_ref"
	studio "street_light_06_unmasked_referen_ref"
	studio "street_light_snow_07_reference_ref"
	studio "power_box_reference_ref"
}



$hbox 0 "light_pole" -36.330000 -19.320000 0.000000 106.730000 9.980000 300.000000
$hbox 0 "power_box" -12.570000 -0.010000 -15.420000 12.340000 9.180000 30.670000

$sequence "idle1" "idle1_ani"
