$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "padlock.mdl"

$root "padlock"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "lock"
{
	studio "padlock_lod0_ref"
	studio "padlock_lod1_ref"
	studio "padlock_lod2_ref"
	studio "padlock_lod3_ref"
	blank
}



$hbox 0 "padlock" -1.860000 -0.590000 -0.890000 1.910000 5.250000 0.900000

$sequence "idle1" "idle1_ani"
