$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "small_plantv03.mdl"

$root "smallplantv03"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "small_plantv03_ref_ref"
	studio "small_plantv032_ref_ref"
	studio "small_plantv033_ref_ref"
}



$hbox 0 "smallplantv03" -38.890000 -31.000000 -12.340000 38.890000 31.000000 0.030000
$hbox 0 "smallplantv04" -26.640000 -21.240000 -8.660000 26.640000 21.240000 0.000000
$hbox 0 "smallplantv05" -14.320000 -11.420000 -4.890000 14.320000 11.420000 0.000000

$sequence "idle" "idle_ani" fps 20
