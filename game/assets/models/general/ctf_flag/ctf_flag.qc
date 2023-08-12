$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "ctf_flag.mdl"

$root "flag_master"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "ctf_flag_phoenix_reference_ref"
}

$attachment "" "flag_master" 0.000000 0.000000 64.000000


$hbox 0 "flag_master" -2.370000 -2.780000 -0.220000 4.665957 2.750000 91.980000
$hbox 0 "flag_11" 0.000000 -1.100000 -13.610000 5.200000 0.490000 13.370000
$hbox 0 "flag_12" 0.000000 -1.650000 -13.470000 4.960000 0.390000 13.500000

$sequence "on_ground" "on_ground_ani" fps 45 loop
$sequence "not_carried" "not_carried_ani" fps 45 loop
$sequence "carried" "carried_ani" fps 45 loop
$sequence "wave_idle" "wave_idle_ani" fps 45 loop
$sequence "flag_positioned" "flag_positioned_ani" fps 45 loop
