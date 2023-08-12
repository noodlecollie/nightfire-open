$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "grunt_ss.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "grunt_ss_lod0_ref"
	studio "grunt_ss_lod1_ref"
	studio "grunt_ss_lod2_ref"
	studio "grunt_ss_lod3_ref"
	studio "grunt_ss_lod4_ref"
	blank
}
$bodygroup "heads"
{
	studio "grunt_ss_head_1_lod0_ref"
	studio "grunt_ss_head_1_lod1_ref"
	studio "grunt_ss_head_1_lod2_ref"
	studio "grunt_ss_head_1_lod3_ref"
	studio "grunt_ss_head_1_lod4_ref"
	studio "grunt_ss_head_2_lod0_ref"
	studio "grunt_ss_head_2_lod1_ref"
	studio "grunt_ss_head_2_lod2_ref"
	studio "grunt_ss_head_2_lod3_ref"
	studio "grunt_ss_head_2_lod4_ref"
	studio "grunt_ss_head_3_lod0_ref"
	studio "grunt_ss_head_3_lod1_ref"
	studio "grunt_ss_head_3_lod2_ref"
	studio "grunt_ss_head_3_lod3_ref"
	studio "grunt_ss_head_3_lod4_ref"
	blank
}
$bodygroup "accessories"
{
	studio "grunt_ss_glass_lod0_ref"
	studio "grunt_ss_glass_lod1_ref"
	studio "grunt_ss_glass_lod2_ref"
	studio "grunt_ss_glass_lod3_ref"
	studio "grunt_ss_glass_lod4_ref"
	blank
}
$bodygroup "weapons"
{
	blank
	studio "grunt_ss_laserrifle_lod0_ref"
}
$bodygroup "skeleton"
{
	blank
	blank
}

$attachment "" "Bip01 R Hand" 24.000000 -7.500000 4.000000
$attachment "" "Bip01 R Hand" 24.000000 -7.500000 4.000000
$attachment "" "Bip01 R Hand" 36.000000 -12.000000 4.000000
$attachment "" "Bip01" 0.000000 0.000000 48.000000
$attachment "" "Bip01 Head" 5.000000 0.000000 0.000000
$attachment "" "Bip01 Head" 5.000000 30.000000 0.000000


$hbox 0 "Bip01 Head" 0.000000 -4.000000 -4.000000 9.000000 5.000000 4.000000
$hbox 1 "Bip01 Neck" -2.000000 -3.000000 -3.000000 4.000000 3.000000 3.000000
$hbox 2 "Bip01 Spine2" -2.000000 0.000000 -6.000000 10.000000 6.000000 7.000000
$hbox 3 "Bip01 Spine2" -2.000000 -6.000000 -7.000000 10.000000 0.000000 7.000000
$hbox 4 "Bip01 Spine" -4.000000 0.000000 -7.000000 6.000000 6.000000 7.000000
$hbox 3 "Bip01 Spine" -6.000000 -6.000000 -7.000000 6.000000 0.000000 7.000000
$hbox 5 "Bip01 Pelvis" -5.000000 1.000000 -3.000000 1.000000 7.000000 3.000000
$hbox 14 "Bip01 L Thigh" 0.000000 -4.000000 -3.000000 20.000000 3.000000 3.000000
$hbox 15 "Bip01 L Calf" 0.000000 -4.000000 -3.000000 17.000000 3.000000 3.000000
$hbox 15 "Bip01 L Foot" 1.000000 -4.000000 -2.000000 4.000000 8.000000 2.000000
$hbox 12 "Bip01 R Thigh" 0.000000 -4.000000 -3.000000 20.000000 3.000000 3.000000
$hbox 13 "Bip01 R Calf" 0.000000 -4.000000 -3.000000 17.000000 3.000000 3.000000
$hbox 13 "Bip01 R Foot" 1.000000 -4.000000 -2.000000 4.000000 8.000000 2.000000
$hbox 9 "Bip01 L UpperArm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 10 "Bip01 L Forearm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 11 "Bip01 L Hand" 0.000000 -1.000000 -2.000000 6.000000 2.000000 2.000000
$hbox 6 "Bip01 R UpperArm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 7 "Bip01 R Forearm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 8 "Bip01 R Hand" 0.000000 -1.000000 -2.000000 6.000000 2.000000 2.000000

$sequence "walk1" "walk1_ani" ACT_WALK 1 LX fps 20 loop
$sequence "walkcasual" "walkcasual_ani" ACT_WALK 1 LX fps 20 loop
$sequence "turn_L" "turn_L_ani" ACT_TURN_LEFT 1
$sequence "turn_R" "turn_R_ani" ACT_TURN_RIGHT 1
$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 15
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 20
$sequence "idle3" "idle3_ani" ACT_IDLE 1 fps 15
$sequence "inspect_body" "inspect_body_ani" fps 12
$sequence "limpingwalk" "limpingwalk_ani" ACT_WALK_HURT 1 LX fps 20 loop
$sequence "limpingrun" "limpingrun_ani" ACT_RUN_HURT 1 LX fps 20 loop
$sequence "alerted_walkpatrol" "alerted_walkpatrol_ani" ACT_WALK_SCARED 2 LX fps 15 loop
$sequence "alerted_walksneak1" "alerted_walksneak1_ani" ACT_WALK_SCARED 2 LX fps 15 loop
$sequence "alerted_walksneak2" "alerted_walksneak2_ani" ACT_WALK_SCARED 2 LX fps 15 loop
$sequence "run" "run_ani" ACT_RUN 1 LX fps 25 loop
$sequence "alerted_idlesearching" "alerted_idlesearching_ani" ACT_IDLE_ANGRY 2 fps 20 loop
$sequence "alerted_idlefrantic" "alerted_idlefrantic_ani" ACT_IDLE_ANGRY 2 fps 20 loop
$sequence "combatidle" "combatidle_ani" ACT_COMBAT_IDLE 1 fps 15
$sequence "crouching_mgun" "crouching_mgun_ani" "crouching_mgun_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "34"
}
$sequence "standing_mgun" "standing_mgun_ani" "standing_mgun_blend1_ani" ACT_RANGE_ATTACK1 10 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "34"
}
$sequence "reload_mgun" "reload_mgun_ani" ACT_RELOAD 1 fps 15 {
	event 2 55 ""
}
$sequence "crouching_idle" "crouching_idle_ani" fps 15
$sequence "crouching_wait" "crouching_wait_ani" ACT_CROUCH 1 fps 15
$sequence "crouching_wait2" "crouching_wait2_ani" ACT_CROUCH 1 fps 15
$sequence "frontkick" "frontkick_ani" ACT_MELEE_ATTACK1 1 fps 28 {
	event 3 8 ""
}
$sequence "strafe_L_fire" "strafe_L_fire_ani" ACT_RANGE_ATTACK1 1 LX LY loop {
	event 4 10 ""
	event 5001 10 "34"
	event 5 30 ""
	event 5001 30 "34"
	event 6 40 ""
	event 5001 40 "34"
}
$sequence "strafe_R_fire" "strafe_R_fire_ani" ACT_RANGE_ATTACK1 1 LX LY loop {
	event 4 2 ""
	event 5001 2 "34"
	event 5 11 ""
	event 5001 11 "34"
	event 6 23 ""
	event 5001 23 "34"
}
$sequence "walkingback_fire" "walkingback_fire_ani" ACT_RANGE_ATTACK1 1 LX LY loop {
	event 4 4 ""
	event 5001 4 "34"
	event 5 9 ""
	event 5001 9 "34"
	event 6 14 ""
	event 5001 14 "34"
}
$sequence "running_fire" "running_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 25 loop {
	event 4 8 ""
	event 5001 8 "34"
	event 5 16 ""
	event 5001 16 "34"
	event 6 24 ""
	event 5001 24 "34"
}
$sequence "cower" "cower_ani" ACT_COWER 1 fps 20
$sequence "surrender" "surrender_ani" fps 15
$sequence "surprised" "surprised_ani" ACT_THREAT_DISPLAY 1
$sequence "grenadecower" "grenadecower_ani" ACT_COWER 1 fps 25
$sequence "leftlegsmflinch" "leftlegsmflinch_ani" ACT_FLINCH_RIGHTLEG 1 fps 25
$sequence "leftlegsmflinch2" "leftlegsmflinch2_ani" ACT_INVALID 1 fps 25
$sequence "rightlegsmflinch" "rightlegsmflinch_ani" ACT_INVALID 1 fps 25
$sequence "rightlegsmflinch2" "rightlegsmflinch2_ani" ACT_INVALID 1 fps 25
$sequence "electrocution" "electrocution_ani" ACT_BIG_FLINCH 1 fps 25 loop
$sequence "rightarmlowerflinch" "rightarmlowerflinch_ani" ACT_INVALID 1 fps 25
$sequence "rightarmhandflinch" "rightarmhandflinch_ani" ACT_INVALID 1 fps 25
$sequence "rightarmupperflinch" "rightarmupperflinch_ani" ACT_FLINCH_LEFTLEG 1 fps 25
$sequence "leftarmlowerflinch" "leftarmlowerflinch_ani" ACT_INVALID 1 fps 25
$sequence "leftarmhandflinch" "leftarmhandflinch_ani" ACT_INVALID 1 fps 25
$sequence "leftarmupperflinch" "leftarmupperflinch_ani" ACT_FLINCH_RIGHTARM 1 fps 25
$sequence "stomachflinch" "stomachflinch_ani" ACT_FLINCH_STOMACH 1 fps 25
$sequence "chestflinch" "chestflinch_ani" ACT_FLINCH_CHEST 1 fps 25
$sequence "groinflinch" "groinflinch_ani" ACT_FLINCH_LEFTARM 1 fps 25
$sequence "headflinch" "headflinch_ani" ACT_FLINCH_HEAD 1 fps 25
$sequence "backflinch" "backflinch_ani" ACT_INVALID 1 fps 25
$sequence "blinded" "blinded_ani" ACT_INVALID 1 fps 20 loop
$sequence "blindedfire" "blindedfire_ani" ACT_INVALID 1 fps 34 loop {
	event 4 13 ""
	event 5001 13 "34"
	event 5 18 ""
	event 5001 18 "34"
	event 6 22 ""
	event 5001 22 "34"
	event 4 28 ""
	event 5001 28 "34"
	event 5 34 ""
	event 5001 34 "34"
	event 6 62 ""
	event 5001 62 "34"
	event 4 70 ""
	event 5001 70 "34"
	event 5 78 ""
	event 5001 78 "34"
	event 6 83 ""
	event 5001 83 "34"
	event 4 89 ""
	event 5001 89 "34"
}
$sequence "knockedout" "knockedout_ani" ACT_SLEEP 1 fps 20 {
	event 11 10 ""
	event 2002 61 ""
}
$sequence "knockedoutidle" "knockedoutidle_ani" fps 15 loop
$sequence "knockedout_wakeup" "knockedout_wakeup_ani" fps 20
$sequence "dieback1" "dieback1_ani" ACT_INVALID 1 fps 20 {
	event 11 0 ""
	event 2002 50 ""
}
$sequence "dieforward" "dieforward_ani" ACT_INVALID 1 fps 25 {
	event 11 0 ""
	event 2002 37 ""
}
$sequence "diesimple" "diesimple_ani" ACT_DIESIMPLE 1 fps 20 {
	event 11 0 ""
	event 2002 50 ""
}
$sequence "diebackwards" "diebackwards_ani" ACT_DIEBACKWARD 1 fps 20 {
	event 11 0 ""
	event 2002 50 ""
}
$sequence "dieheadshot" "dieheadshot_ani" ACT_DIE_HEADSHOT 1 fps 20 {
	event 11 0 ""
	event 2002 37 ""
}
$sequence "diegutshot" "diegutshot_ani" ACT_DIE_GUTSHOT 1 fps 25 {
	event 11 0 ""
	event 2002 146 ""
}
$sequence "dienutshot" "dienutshot_ani" ACT_INVALID 1 fps 25 {
	event 11 0 ""
	event 2002 146 ""
}
$sequence "die_explosion1" "die_explosion1_ani" ACT_DIEVIOLENT 1 fps 20 {
	event 11 0 ""
}
$sequence "die_explosion2" "die_explosion2_ani" ACT_DIEVIOLENT 1 fps 20 {
	event 11 0 ""
}
$sequence "die_shotgun1" "die_shotgun1_ani" ACT_DIEVIOLENT 1 fps 20 {
	event 11 0 ""
}
$sequence "die_shotgun2" "die_shotgun2_ani" ACT_DIEVIOLENT 1 fps 20 {
	event 11 0 ""
}
$sequence "l_corner_idle" "l_corner_idle_ani" fps 15
$sequence "l_corner_peek" "l_corner_peek_ani" fps 20
$sequence "l_corner_fire_death" "l_corner_fire_death_ani" fps 25 {
	event 11 0 ""
	event 2002 37 ""
}
$sequence "l_corner_fire_step" "l_corner_fire_step_ani" fps 25
$sequence "l_corner_fire_return" "l_corner_fire_return_ani" fps 25
$sequence "l_corner_fire" "l_corner_fire_ani" {
	event 4 1 ""
	event 5001 1 "34"
	event 5 5 ""
	event 5001 5 "34"
	event 6 10 ""
	event 5001 10 "34"
}
$sequence "l_corner_roll" "l_corner_roll_ani" ACT_ROLL_RIGHT 1 LX LY AZ fps 25
$sequence "roll_right_fire" "roll_right_fire_ani"
$sequence "r_corner_idle" "r_corner_idle_ani" fps 15
$sequence "r_corner_peek" "r_corner_peek_ani" fps 20
$sequence "r_corner_fire_death" "r_corner_fire_death_ani" fps 25 {
	event 11 0 ""
	event 2002 37 ""
}
$sequence "r_corner_fire_step" "r_corner_fire_step_ani"
$sequence "r_corner_fire_return" "r_corner_fire_return_ani"
$sequence "r_corner_fire" "r_corner_fire_ani" {
	event 4 1 ""
	event 5001 1 "34"
	event 5 5 ""
	event 5001 5 "34"
	event 6 10 ""
	event 5001 10 "34"
}
$sequence "r_corner_roll" "r_corner_roll_ani" ACT_ROLL_LEFT 1 LX LY AZ fps 25
$sequence "roll_left_fire" "roll_left_fire_ani"
