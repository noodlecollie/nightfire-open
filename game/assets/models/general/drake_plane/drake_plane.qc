$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "drake_plane.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "zoe"
{
	blank
	studio "zoe_snow_lod0_reference_ref"
	studio "zoe_snow_lod1_reference_ref"
	studio "zoe_snow_lod2_reference_ref"
	studio "zoe_snow_lod3_reference_ref"
	studio "zoe_snow_lod4_reference_ref"
}
$bodygroup "plane"
{
	studio "drake_plane_reference_ref"
}



$hbox 0 "plane" -20.000000 -20.000000 -95.000000 20.000000 25.000000 137.000000
$hbox 0 "plane" -165.000000 -15.000000 25.000000 165.000000 0.000000 90.000000
$hbox 0 "plane" -70.000000 -5.000000 -115.000000 70.000000 5.000000 -75.000000
$hbox 0 "plane" -5.000000 -5.000000 -130.000000 5.000000 60.000000 -95.000000

$sequence "idle1" "idle1_ani" loop
$sequence "zoe_idle1" "zoe_idle1_ani" ACT_IDLE 1 fps 20 loop
$sequence "zoe_idle2" "zoe_idle2_ani" ACT_IDLE 1 fps 20 loop
$sequence "running" "running_ani" loop
