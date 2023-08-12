$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "zoe_snow.mdl"

$root "Bip01"

$flags 18432
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "zoe_snow_lod0_reference_ref"
	studio "zoe_snow_lod1_reference_ref"
	studio "zoe_snow_lod2_reference_ref"
	studio "zoe_snow_lod3_reference_ref"
	studio "zoe_snow_lod4_reference_ref"
	blank
}
$bodygroup "heads"
{
	blank
}
$bodygroup "accessories"
{
	blank
	blank
}
$bodygroup "weapons"
{
	blank
	studio "zoe_snow_gun_reference_ref"
}
$bodygroup "skeleton"
{
	blank
	studio "zoe_party_underwear_ref"
}

$attachment "" "Bip01 R Hand" 40.000000 -8.000000 7.000000
$attachment "" "Bip01 R Hand" 9.000000 1.000000 2.000000
$attachment "" "Bip01 R Hand" 20.000000 1.000000 2.000000
$attachment "" "Bip01" 0.000000 0.000000 48.000000
$attachment "" "Bip01 Head" 5.000000 0.000000 0.000000
$attachment "" "Bip01 Head" 5.000000 30.000000 0.000000

$controller 0 "Bone39" 32800 0 0
$controller 1 "Bone21" 32800 0 0
$controller 2 "Bip01 Head" XR -50 50

$hbox 0 "Bip01 Head" 0.000000 -4.000000 -4.000000 9.000000 5.000000 4.000000
$hbox 1 "Bip01 Neck" -2.000000 -3.000000 -3.000000 4.000000 3.000000 3.000000
$hbox 2 "Bip01 Spine2" -2.000000 0.000000 -6.000000 10.000000 6.000000 7.000000
$hbox 3 "Bip01 Spine2" -2.000000 -6.000000 -7.000000 10.000000 0.000000 7.000000
$hbox 4 "Bip01 Spine" -4.000000 0.000000 -7.000000 8.000000 6.000000 7.000000
$hbox 3 "Bip01 Spine" -4.000000 -6.000000 -7.000000 8.000000 0.000000 7.000000
$hbox 5 "Bip01 Pelvis" -5.000000 1.000000 -3.000000 1.000000 7.000000 3.000000
$hbox 14 "Bip01 L Thigh" 0.000000 -4.000000 -3.000000 18.000000 3.000000 3.000000
$hbox 15 "Bip01 L Calf" 0.000000 -4.000000 -3.000000 19.000000 3.000000 3.000000
$hbox 15 "Bip01 L Foot" 1.000000 -4.000000 -2.000000 4.000000 8.000000 2.000000
$hbox 12 "Bip01 R Thigh" 0.000000 -4.000000 -3.000000 18.000000 3.000000 3.000000
$hbox 13 "Bip01 R Calf" 0.000000 -4.000000 -3.000000 19.000000 3.000000 3.000000
$hbox 13 "Bip01 R Foot" 1.000000 -4.000000 -2.000000 4.000000 8.000000 2.000000
$hbox 9 "Bip01 L UpperArm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 10 "Bip01 L Forearm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 11 "Bip01 L Hand" 0.000000 -1.000000 -2.000000 6.000000 2.000000 2.000000
$hbox 6 "Bip01 R UpperArm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 7 "Bip01 R Forearm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 8 "Bip01 R Hand" 0.000000 -1.000000 -2.000000 6.000000 2.000000 2.000000

$sequence "null" "null_ani" loop
$sequence "a" "a_ani" loop
$sequence "e" "e_ani" loop
$sequence "o" "o_ani" loop
$sequence "f" "f_ani" loop
$sequence "s" "s_ani" loop
$sequence "m" "m_ani" loop
$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 20
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 25
$sequence "idle3" "idle3_ani" ACT_IDLE 1 fps 25
$sequence "duck" "duck_ani" ACT_CROUCH 1 fps 15 loop
$sequence "fire" "fire_ani" ACT_RANGE_ATTACK1 1
$sequence "walk" "walk_ani" ACT_WALK 1 LX LY fps 33 loop {
	event 1004 18 "zoe/snow2.wav"
	event 1004 41 "zoe/snow1.wav"
}
$sequence "run" "run_ani" ACT_RUN 1 LX LY fps 25 loop {
	event 1004 4 "zoe/snow1.wav"
	event 1004 12 "zoe/snow2.wav"
}
$sequence "run_cower" "run_cower_ani" ACT_RUN_SCARED 1 LX LY fps 28 loop
$sequence "corner_idle1" "corner_idle1_ani" LX LY fps 15 loop
$sequence "corner_idle2" "corner_idle2_ani" LX LY fps 15 loop
$sequence "corner_idle3" "corner_idle3_ani" LX LY fps 15 loop
$sequence "corner_crouch_idle1" "corner_crouch_idle1_ani" LX LY fps 15 loop
$sequence "corner_crouch_idle2" "corner_crouch_idle2_ani" LX LY fps 20 loop
$sequence "corner_crouch_idle3" "corner_crouch_idle3_ani" LX LY fps 15 loop
$sequence "corner_peek_L" "corner_peek_L_ani" LX LY
$sequence "corner_peek_R" "corner_peek_R_ani" LX LY fps 33
$sequence "death" "death_ani" ACT_DIESIMPLE 1 LX LY
$sequence "L_wing_check" "L_wing_check_ani" fps 15
$sequence "L_tail_check" "L_tail_check_ani" fps 15
$sequence "R_wing_check" "R_wing_check_ani" fps 15
$sequence "R_tail_check" "R_tail_check_ani" fps 15
$sequence "rudder_check" "rudder_check_ani" fps 15
$sequence "propeller_check" "propeller_check_ani" fps 15
$sequence "get_in_plane" "get_in_plane_ani" fps 28
$sequence "jump_railing" "jump_railing_ani"
$sequence "fuel_line_attach" "fuel_line_attach_ani" LX LY
$sequence "fuel_line_idle" "fuel_line_idle_ani" loop
$sequence "fuel_line_detach" "fuel_line_detach_ani"
