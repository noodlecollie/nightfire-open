$cd .
$cdtexture "../../../../../content/nfopen/textures"
$modelname "v_mp9.mdl"

$root "Bip01"

$noembeddedtextures

$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

$bodygroup "studio"
{
	studio "v_MP9_reference_ref"
}

$attachment "" "R_GUN" -9.500000 4.300000 0.000000


$hbox 0 "Bip01 L Hand" -0.240000 -0.820000 -2.000000 3.820000 1.560000 1.690000
$hbox 0 "Bip01 L Finger0" 0.000000 -0.580000 -0.530000 1.630000 0.740000 0.690000
$hbox 0 "L_Arm_bone" -1.790000 -1.950000 -3.230000 1.940000 1.690000 10.710000
$hbox 0 "Bip01 R Hand" -0.230000 -0.930000 -1.690000 3.820000 1.460000 2.000000
$hbox 0 "Bip01 R Finger0" 0.000000 -0.520000 -0.750000 1.560000 0.790000 0.480000
$hbox 0 "R_GUN" -9.440000 -1.990000 -1.050000 8.790000 6.310000 1.050000
$hbox 0 "R_Arm_bone" -1.800000 -2.010000 -2.540000 1.830000 1.620000 11.360000

$sequence "2handed_idle1" "2handed_idle1_ani" fps 15
$sequence "2handed_idle2" "2handed_idle2_ani" fps 15
$sequence "2handed_idle3" "2handed_idle3_ani" fps 15
$sequence "2handed_fire" "2handed_fire_ani" {
	event 5001 0 "21"
}
$sequence "2handed_alt_fire" "2handed_alt_fire_ani"
$sequence "2handed_reload_full" "2handed_reload_full_ani" fps 45 {
	event 5005 0 "weapons/weapon_mp9/mp9_reload.wav"
}
$sequence "2handed_reload_empty" "2handed_reload_empty_ani" fps 45 {
	event 5005 0 "weapons/weapon_mp9/mp9_reload_empty.wav"
}
$sequence "2handed_draw" "2handed_draw_ani" {
	event 5005 0 "weapons/weapon_mp9/mp9_draw.wav"
}
$sequence "2handed_holster" "2handed_holster_ani"
$sequence "idle1" "idle1_ani" fps 15
$sequence "idle2" "idle2_ani" fps 15
$sequence "idle3" "idle3_ani" fps 15
$sequence "fire" "fire_ani" {
	event 5001 0 "21"
}
$sequence "alt_fire" "alt_fire_ani" fps 15 {
	event 1 0 ""
}
$sequence "reload_full" "reload_full_ani" fps 45 {
	event 5005 0 "weapons/weapon_mp9/mp9_reload.wav"
}
$sequence "reload_empty" "reload_empty_ani" fps 45 {
	event 5005 0 "weapons/weapon_mp9/mp9_reload_empty.wav"
}
$sequence "draw" "draw_ani" {
	event 5005 0 "weapons/weapon_mp9/mp9_draw.wav"
}
$sequence "holster" "holster_ani" fps 15
