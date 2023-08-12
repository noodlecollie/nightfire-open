$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "pipes_sm.mdl"

$root "pipe02"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "icicle"
{
	studio "pipe_SM_01_reference_ref"
	studio "pipe_SM_02_reference_ref"
	studio "pipe_SM_03_reference_ref"
	studio "pipe_SM_04_reference_ref"
	studio "pipe_SM_05_reference_ref"
	studio "pipe_SM_06_reference_ref"
	studio "pipe_SM_07_reference_ref"
	studio "pipe_SM_08_reference_ref"
	studio "pipe_SM_09_reference_ref"
	studio "pipe_SM_10_reference_ref"
}



$hbox 0 "pipe02" -3.000000 -3.000000 -8.000000 3.000000 3.000000 9.170000
$hbox 0 "pipe" -5.750000 -4.980000 -128.000000 9.310000 3.000000 128.690000

$sequence "idle1" "idle1_ani"
