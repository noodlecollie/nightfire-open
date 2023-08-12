$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "round_ac_vent.mdl"

$root "ac_vent"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "sizes"
{
	studio "round_ac_vent_ref"
	studio "round_ac_vent2_ref"
	studio "round_ac_vent3_ref"
	studio "round_ac_vent4_ref"
}



$hbox 0 "ac_vent" -69.110000 -61.790000 -103.340000 69.110000 61.790000 0.000000

$sequence "idle1" "idle1_ani" fps 10 loop
$sequence "idle2" "idle2_ani" fps 10 loop
$sequence "idle3" "idle3_ani" fps 10 loop
$sequence "idle4" "idle4_ani" fps 10 loop
