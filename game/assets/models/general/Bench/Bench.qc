$cdtexture "../../../../content/nfopen/textures"
$cd "."
$modelname "Bench.mdl"
$noembeddedtextures

$root "Bench"

$flags 0
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "Bench_ref"
}



$hbox 0 "Bench" -17.800000 -15.380000 -53.940000 17.800000 15.380000 53.940000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
