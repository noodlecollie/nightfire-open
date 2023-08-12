$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "shuttle_holder.mdl"

$root "cage map"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "shuttle_holder_ref"
}



$hbox 0 "cage map" -409.760000 -524.910000 -712.830000 414.070000 424.480000 0.000000
$hbox 0 "shutlle_holder_clamps" -128.420000 -492.050000 -331.340000 126.120000 390.090000 0.000000

$sequence "idle1" "idle1_ani"
$sequence "idle2" "idle2_ani"
