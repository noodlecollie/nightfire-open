$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "pipes_med.mdl"

$root "pipe"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "icicle"
{
	studio "pipe_01_reference_ref"
	studio "pipe_02_reference_ref"
	studio "pipe_03_reference_ref"
	studio "pipe_04_reference_ref"
	studio "pipe_05_reference_ref"
	studio "pipe_06_reference_ref"
	studio "pipe_07_reference_ref"
	studio "pipe_08_reference_ref"
	studio "pipe_09_reference_ref"
}



$hbox 0 "pipe" -11.500000 -8.000000 -64.000000 16.850000 6.000000 65.680000
$hbox 0 "pipe01" -6.000000 -6.000000 -128.000000 6.000000 6.000000 129.960000

$sequence "idle1" "idle1_ani"
