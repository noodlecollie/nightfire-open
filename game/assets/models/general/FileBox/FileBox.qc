$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "FileBox.mdl"

$root "File Box"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "FileBox_ref"
}



$hbox 0 "File Box" -7.260000 -5.710000 0.000000 7.250000 5.710000 10.730000
$hbox 0 "FileBox Lid" -7.910000 -6.230000 0.000000 7.910000 6.230000 13.500000

$sequence "idle1" "idle1_ani"
