$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "paltreev01.mdl"

$root "tree2"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "paltreev01_ref_ref"
	studio "paltreev02_ref_ref"
}



$hbox 0 "tree2" -90.770000 -163.640000 -338.290000 90.770000 91.060000 8.230000
$hbox 0 "treecopy" -95.360000 -173.960000 -339.980000 93.140000 93.370000 2.720000

$sequence "idle" "idle_ani" fps 20
