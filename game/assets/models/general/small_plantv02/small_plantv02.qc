$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "small_plantv02.mdl"

$root "plantv02"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "small_plantv02_ref_ref"
	studio "small_plantv021_ref_ref"
	studio "small_plantv022_ref_ref"
}



$hbox 0 "plantv02" -27.400000 -22.930000 -14.150000 27.400000 22.930000 5.790000
$hbox 0 "plantv03" -49.600000 -41.500000 -25.600000 49.600000 41.500000 10.510000
$hbox 0 "plantv04" -20.600000 -17.240000 -10.940000 20.600000 17.240000 4.050000

$sequence "idle" "idle_ani" fps 20
