$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "small_pan.mdl"

$root "Hanging lantern"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "pan_ref"
}



$hbox 0 "Hanging lantern" -4.000000 -14.700000 -0.580000 4.000000 0.340000 0.680000

$sequence "idle1" "idle1_ani" fps 10 loop
$sequence "idle2" "idle2_ani" fps 10 loop
$sequence "idle3" "idle3_ani" fps 10 loop
$sequence "f_hit" "f_hit_ani" {
	event 5006 0 "pots"
}
$sequence "b_hit" "b_hit_ani" {
	event 5006 0 "pots"
}
$sequence "l_hit" "l_hit_ani" fps 36 {
	event 5006 0 "pots"
}
$sequence "r_hit" "r_hit_ani" fps 36 {
	event 5006 0 "pots"
}
$sequence "flat" "flat_ani" fps 10 loop
