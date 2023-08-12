$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "alura_combat.mdl"

$root "Bip01"

$flags 18432
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "alura_combat_lod0_ref"
	studio "alura_combat_lod1_ref"
	studio "alura_combat_lod2_ref"
	studio "alura_combat_lod3_ref"
	studio "alura_combat_lod4_ref"
	blank
}
$bodygroup "heads"
{
	blank
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
	studio "alura_combat_sniper_reference_ref"
}
$bodygroup "skeleton"
{
	blank
	studio "alura_underwear_ref"
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
$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 15
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 15
$sequence "crouch" "crouch_ani" ACT_CROUCH 1 fps 15 loop
$sequence "combatidle" "combatidle_ani" ACT_COMBAT_IDLE 1 fps 15
$sequence "standing_sniper" "standing_sniper_ani" "standing_sniper_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 fps 25 {
	event 4 3 ""
	event 5001 3 "50"
}
$sequence "crouching_sniper" "crouching_sniper_ani" "crouching_sniper_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 fps 25 {
	event 4 3 ""
	event 5001 3 "50"
}
$sequence "crouch_idle" "crouch_idle_ani"
$sequence "reload_sniper" "reload_sniper_ani" ACT_RELOAD 1 fps 28 {
	event 2 32 ""
}
$sequence "walk" "walk_ani" ACT_WALK 1 LX LY fps 33 loop {
	event 1004 1 "player/pl_dirt1.wav"
	event 1008 24 "player/pl_dirt1.wav"
}
$sequence "run" "run_ani" ACT_RUN 1 LX LY fps 29 loop {
	event 1004 1 "player/pl_dirt1.wav"
	event 1008 8 "player/pl_dirt1.wav"
}
$sequence "death" "death_ani" ACT_DIESIMPLE 1 {
	event 2002 46 ""
}
$sequence "jump" "jump_ani" {
	event 1004 51 "zoe/pain3.wav"
}
$sequence "radio_tinker" "radio_tinker_ani" fps 15 loop
$sequence "comm_tinker" "comm_tinker_ani" fps 35 loop
$sequence "comm_tinker_transition" "comm_tinker_transition_ani"
$sequence "jump_rockside" "jump_rockside_ani" fps 35 {
	event 1004 31 "alura/rocks2.wav"
}
$sequence "climb_fence" "climb_fence_ani" fps 20 {
	event 1004 20 "alura/fence_climb1.wav"
	event 1008 36 "alura/fence_climb3.wav"
	event 1004 58 "alura/fence_climb2.wav"
	event 1008 72 "alura/fence_climb1.wav"
	event 1004 93 "alura/fence_climb2.wav"
	event 1008 110 "alura/fence_climb3.wav"
	event 1004 127 "alura/fence_climb2.wav"
	event 1008 147 "alura/fence_climb3.wav"
	event 1004 166 "alura/fence_climb1.wav"
	event 1008 182 "alura/fence_climb3.wav"
	event 1004 214 "alura/fence_climb2.wav"
	event 1008 242 "alura/fence_climb1.wav"
	event 1004 262 "alura/fence_climb3.wav"
	event 1008 281 "alura/fence_climb2.wav"
	event 1004 300 "alura/rocks3.wav"
}
$sequence "cell_idle" "cell_idle_ani" fps 40 loop
$sequence "cell_pickup_gun" "cell_pickup_gun_ani" fps 33 {
	event 1003 135 "alura_pickup_gun"
}
