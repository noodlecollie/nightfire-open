$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "incense.mdl"

$root "Box03"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "parts"
{
	studio "incense_ref1_ref"
	studio "incense_ref2_ref"
}

$attachment "" "Box03" 0.000000 0.000000 4.400000


$hbox 0 "Box03" -5.330000 -5.330000 0.000000 5.340000 5.330000 4.700000
$hbox 0 "Box01" -5.160000 -5.180000 0.000000 5.180000 5.160000 11.980000

$sequence "idle1" "idle1_ani"
$sequence "idle2" "idle2_ani"
