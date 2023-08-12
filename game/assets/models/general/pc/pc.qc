$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "pc.mdl"

$root "Dummy01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "pc"
{
	studio "pc_01_reference_ref"
	studio "pc_broken_reference_ref"
	studio "pc_02_reference_ref"
	studio "pc_03_reference_ref"
	studio "pc_04_reference_ref"
	studio "pc_05_reference_ref"
}



$hbox 0 "keyboard" -8.300000 -3.660000 -0.560000 8.300000 3.660000 0.560000
$hbox 0 "monitor" -13.790000 -12.110000 -0.150000 11.040000 10.740000 24.780000
$hbox 0 "case" -4.730000 -8.010000 -0.200000 4.730000 8.010000 18.330000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
$sequence "hit" "hit_ani" fps 25
