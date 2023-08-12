$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "field_radio.mdl"

$root "0"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "gibs"
{
	studio "field_radio_01_ref"
	studio "field_radio_01_broken_ref"
	studio "field_radio_02_ref"
	studio "field_radio_02_broken_ref"
	studio "field_radio_03_ref"
	studio "field_radio_03_broken_ref"
	studio "field_radio_04_ref"
	studio "field_radio_04_broken_ref"
	studio "field_radio_05_ref"
	studio "field_radio_05_broken_ref"
	studio "field_radio_06_ref"
	studio "field_radio_06_broken_ref"
	studio "field_radio_mic_ref"
}



$hbox 0 "0" -11.160000 0.000000 -6.080000 11.160000 9.910000 6.600000
$hbox 0 "01" -5.240000 0.000000 -21.130000 5.050000 2.340000 21.140000

$sequence "idle1" "idle1_ani"
