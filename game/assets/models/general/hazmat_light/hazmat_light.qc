$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "hazmat_light.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "hazmat_light_lod0_reference_ref"
	studio "hazmat_light_lod1_reference_ref"
	studio "hazmat_light_lod2_reference_ref"
	studio "hazmat_light_lod3_reference_ref"
	studio "hazmat_light_lod4_reference_ref"
	blank
}
$bodygroup "heads"
{
	studio "hazmat_light_head_1_lod0_ref"
	studio "hazmat_light_head_1_lod2_ref"
	studio "hazmat_light_head_1_lod3_ref"
	studio "hazmat_light_head_1_lod4_ref"
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
	studio "hazmat_pdw90_ref"
	studio "hazmat_sig_ref"
}
$bodygroup "skeleton"
{
	blank
	studio "hazmat_light_skeleton_ref"
}

$attachment "" "Bip01 R Hand" 32.000000 -7.800000 5.000000
$attachment "" "Bip01 R Hand" 30.000000 -6.500000 1.500000
$attachment "" "Bip01 R Hand" 48.000000 -10.500000 2.000000
$attachment "" "Bip01" 0.000000 0.000000 48.000000
$attachment "" "Bip01 Head" 5.000000 0.000000 0.000000
$attachment "" "Bip01 Head" 5.000000 30.000000 0.000000


$hbox 0 "Bip01 Head" 0.000000 -3.700000 -3.000000 8.000000 3.700000 3.000000
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

$sequence "walk1" "walk1_ani" ACT_WALK 1 LX loop
$sequence "walkcasual" "walkcasual_ani" ACT_WALK 1 LX loop
$sequence "turn_L" "turn_L_ani" ACT_TURN_LEFT 1 fps 40
$sequence "turn_R" "turn_R_ani" ACT_TURN_RIGHT 1 fps 40
$sequence "idle1" "idle1_ani" ACT_IDLE 2 fps 15
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 15
$sequence "idleheadscratch" "idleheadscratch_ani" ACT_IDLE 1 fps 15
$sequence "idleheadscratch2" "idleheadscratch2_ani" ACT_IDLE 1 fps 15
$sequence "idlecasual1" "idlecasual1_ani" ACT_IDLE 1 fps 25
$sequence "idlecasual2" "idlecasual2_ani" ACT_IDLE 1
$sequence "idlekickrocks" "idlekickrocks_ani" ACT_IDLE 1 fps 20
$sequence "inspect_body" "inspect_body_ani" fps 12
$sequence "limpingwalk" "limpingwalk_ani" ACT_WALK_HURT 1 LX loop
$sequence "limpingrun" "limpingrun_ani" ACT_RUN_HURT 1 LX fps 40 loop
$sequence "alerted_walkpatrol" "alerted_walkpatrol_ani" ACT_WALK_SCARED 2 LX fps 20 loop
$sequence "alerted_walksneak1" "alerted_walksneak1_ani" ACT_WALK_SCARED 2 LX fps 20 loop
$sequence "alerted_walksneak2" "alerted_walksneak2_ani" ACT_WALK_SCARED 2 LX fps 25 loop
$sequence "run" "run_ani" ACT_RUN 1 LX loop
$sequence "run_nogun" "run_nogun_ani" LX loop
$sequence "alerted_idle_nervous" "alerted_idle_nervous_ani" ACT_IDLE_ANGRY 2 fps 20 loop
$sequence "alerted_idlesearching" "alerted_idlesearching_ani" ACT_IDLE_ANGRY 2 fps 22 loop
$sequence "alerted_idlefrantic" "alerted_idlefrantic_ani" ACT_IDLE_ANGRY 2 fps 22 loop
$sequence "combatidle" "combatidle_ani" ACT_COMBAT_IDLE 1 fps 15
$sequence "crouching_mgun" "crouching_mgun_ani" "crouching_mgun_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 fps 55 {
	event 4 1 ""
	event 5001 1 "50"
}
$sequence "standing_mgun" "standing_mgun_ani" "standing_mgun_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 fps 55 {
	event 4 1 ""
	event 5001 1 "50"
}
$sequence "standing_sniper" "standing_sniper_ani" "standing_sniper_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "50"
}
$sequence "sniper_searching" "sniper_searching_ani" ACT_COMBAT_IDLE 1 fps 15
$sequence "reload_mgun" "reload_mgun_ani" ACT_RELOAD 1 fps 20 {
	event 2 28 ""
}
$sequence "crouching_idle" "crouching_idle_ani" fps 15
$sequence "crouching_wait" "crouching_wait_ani" ACT_CROUCH 2 fps 15
$sequence "crouching_wait2" "crouching_wait2_ani" ACT_CROUCH 1 fps 15
$sequence "throwgrenade" "throwgrenade_ani" ACT_RANGE_ATTACK2 1 fps 20 {
	event 7 24 ""
}
$sequence "toss_grenade" "toss_grenade_ani" ACT_RANGE_ATTACK2 1 fps 20 {
	event 7 26 ""
}
$sequence "throwback_grenade" "throwback_grenade_ani" fps 20 {
	event 13 8 ""
	event 7 24 ""
}
$sequence "kick_grenade" "kick_grenade_ani" fps 20 {
	event 14 9 ""
}
$sequence "frontkick" "frontkick_ani" ACT_MELEE_ATTACK1 1 fps 28 {
	event 3 8 ""
	event 2010 8 ""
}
$sequence "frontkick_nogun" "frontkick_nogun_ani" ACT_MELEE_ATTACK2 1 fps 28 {
	event 3 8 ""
	event 2010 8 ""
}
$sequence "vent_peek" "vent_peek_ani" ACT_INSPECT_WALL 1 LX LY
$sequence "vent_idle" "vent_idle_ani" ACT_INSPECT_WALL 1
$sequence "vent_shoot" "vent_shoot_ani" ACT_INSPECT_WALL 1 {
	event 4 0 ""
	event 5001 0 "50"
}
$sequence "vent_stand" "vent_stand_ani" ACT_INSPECT_WALL 1
$sequence "vent_grenade" "vent_grenade_ani" ACT_INSPECT_WALL 1 {
	event 7 53 ""
}
$sequence "kickdoor" "kickdoor_ani"
$sequence "strafe_L_fire" "strafe_L_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 55 loop {
	event 5001 5 "50"
	event 4 5 ""
	event 5001 15 "50"
	event 5 15 ""
	event 5001 25 "50"
	event 6 25 ""
}
$sequence "strafe_R_fire" "strafe_R_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 40 loop {
	event 4 5 ""
	event 5001 5 "50"
	event 5 15 ""
	event 5001 15 "50"
	event 6 25 ""
	event 5001 25 "50"
}
$sequence "walkingback_fire" "walkingback_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 28 loop {
	event 4 5 ""
	event 5001 5 "50"
	event 5 15 ""
	event 5001 15 "50"
}
$sequence "running_fire" "running_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 25 loop {
	event 4 5 ""
	event 5001 5 "50"
	event 6 15 ""
	event 5001 15 "50"
}
$sequence "roll_left" "roll_left_ani" ACT_ROLL_LEFT 1 LX LY AZ fps 35
$sequence "roll_right" "roll_right_ani" ACT_ROLL_RIGHT 1 LX LY AZ fps 35
$sequence "cower" "cower_ani" ACT_COWER 1 fps 32
$sequence "surrender" "surrender_ani" ACT_FEAR_DISPLAY 1 fps 15
$sequence "surprised" "surprised_ani" ACT_THREAT_DISPLAY 1
$sequence "grenadecower" "grenadecower_ani" ACT_COWER 1 fps 35
$sequence "leftlegsmflinch" "leftlegsmflinch_ani" ACT_FLINCH_RIGHTLEG 1
$sequence "leftlegsmflinch2" "leftlegsmflinch2_ani" ACT_INVALID 1
$sequence "rightlegsmflinch" "rightlegsmflinch_ani" ACT_INVALID 1
$sequence "rightlegsmflinch2" "rightlegsmflinch2_ani" ACT_INVALID 1
$sequence "electrocution" "electrocution_ani" ACT_BIG_FLINCH 1 fps 45 loop
$sequence "rightarmlowerflinch" "rightarmlowerflinch_ani" ACT_INVALID 1
$sequence "rightarmhandflinch" "rightarmhandflinch_ani" ACT_INVALID 1
$sequence "rightarmupperflinch" "rightarmupperflinch_ani" ACT_FLINCH_LEFTLEG 1
$sequence "leftarmlowerflinch" "leftarmlowerflinch_ani" ACT_INVALID 1
$sequence "leftarmhandflinch" "leftarmhandflinch_ani" ACT_INVALID 1
$sequence "leftarmupperflinch" "leftarmupperflinch_ani" ACT_FLINCH_RIGHTARM 1
$sequence "stomachflinch" "stomachflinch_ani" ACT_FLINCH_STOMACH 1
$sequence "chestflinch" "chestflinch_ani" ACT_FLINCH_CHEST 1
$sequence "groinflinch" "groinflinch_ani" ACT_FLINCH_LEFTARM 1
$sequence "headflinch" "headflinch_ani" ACT_FLINCH_HEAD 1
$sequence "backflinch" "backflinch_ani" ACT_INVALID 1
$sequence "choking" "choking_ani" ACT_BARNACLE_PULL 1 loop
$sequence "blinded" "blinded_ani" ACT_INVALID 1 loop
$sequence "blinded_noweapon" "blinded_noweapon_ani" fps 12 loop
$sequence "blindedfire" "blindedfire_ani" ACT_INVALID 1 fps 34 loop {
	event 4 13 ""
	event 5001 13 "50"
	event 5 22 ""
	event 5001 22 "50"
	event 6 28 ""
	event 5001 28 "50"
	event 4 34 ""
	event 5001 34 "50"
	event 5 70 ""
	event 5001 70 "50"
	event 6 78 ""
	event 5001 78 "50"
	event 4 89 ""
	event 5001 89 "50"
}
$sequence "knockedout" "knockedout_ani" ACT_SLEEP 1 {
	event 11 10 ""
	event 2002 61 ""
}
$sequence "knockedoutidle" "knockedoutidle_ani" fps 15 loop
$sequence "knockedout2" "knockedout2_ani" ACT_SLEEP 1 {
	event 11 10 ""
	event 2002 109 ""
}
$sequence "knockedout2idle" "knockedout2idle_ani" fps 15 loop
$sequence "knockedout_wakeup" "knockedout_wakeup_ani"
$sequence "dieback1" "dieback1_ani" ACT_DIEBACKWARD 1 {
	event 11 0 ""
	event 2002 50 ""
}
$sequence "dieforward" "dieforward_ani" ACT_DIEFORWARD 1 fps 35 {
	event 11 0 ""
	event 2002 37 ""
}
$sequence "diesimple" "diesimple_ani" ACT_DIESIMPLE 1 fps 28 {
	event 11 0 ""
	event 2002 40 ""
}
$sequence "diebackwards" "diebackwards_ani" ACT_DIEBACKWARD 1 {
	event 11 0 ""
	event 2002 50 ""
}
$sequence "dieheadshot" "dieheadshot_ani" ACT_DIE_HEADSHOT 1 fps 25 {
	event 11 0 ""
	event 2002 45 ""
}
$sequence "diegutshot" "diegutshot_ani" ACT_DIE_GUTSHOT 1 fps 25 {
	event 11 0 ""
	event 2002 37 ""
}
$sequence "dienutshot" "dienutshot_ani" ACT_DIESIMPLE 1 fps 35 {
	event 11 0 ""
	event 2002 37 ""
}
$sequence "die_explosion1" "die_explosion1_ani" ACT_DIEVIOLENT 1 {
	event 11 0 ""
	event 2002 32 ""
}
$sequence "die_explosion2" "die_explosion2_ani" ACT_DIEVIOLENT 1 {
	event 11 0 ""
	event 2002 27 ""
}
$sequence "die_explosion3" "die_explosion3_ani" ACT_DIEVIOLENT 1 {
	event 11 0 ""
	event 2002 36 ""
}
$sequence "die_explosion4" "die_explosion4_ani" ACT_DIEVIOLENT 1 {
	event 11 0 ""
	event 2002 17 ""
}
$sequence "die_explosion5" "die_explosion5_ani" ACT_DIEVIOLENT 1 fps 40 {
	event 11 0 ""
	event 2002 32 ""
}
$sequence "die_explosion6" "die_explosion6_ani" ACT_DIEVIOLENT 1 fps 45 {
	event 11 5 ""
	event 2002 72 ""
}
$sequence "die_shotgun1" "die_shotgun1_ani" ACT_DIEVIOLENT 1 fps 25 {
	event 11 0 ""
	event 2002 16 ""
}
$sequence "die_shotgun2" "die_shotgun2_ani" ACT_DIEVIOLENT 1 fps 25 {
	event 11 0 ""
	event 2002 16 ""
}
$sequence "die_falling_loop" "die_falling_loop_ani" ACT_DIEVIOLENT 1 fps 45 loop {
	event 11 0 ""
}
$sequence "l_corner_idle" "l_corner_idle_ani" fps 15
$sequence "l_corner_peek" "l_corner_peek_ani" fps 25
$sequence "l_corner_fire_death" "l_corner_fire_death_ani" {
	event 11 0 ""
	event 2002 37 ""
}
$sequence "l_corner_fire_step" "l_corner_fire_step_ani"
$sequence "l_corner_fire_return" "l_corner_fire_return_ani"
$sequence "l_corner_fire" "l_corner_fire_ani" {
	event 4 2 ""
	event 5001 2 "50"
	event 5 8 ""
	event 5001 8 "50"
	event 6 14 ""
	event 5001 14 "50"
}
$sequence "l_corner_grenade_throw" "l_corner_grenade_throw_ani" fps 25 {
	event 7 27 ""
}
$sequence "l_corner_roll" "l_corner_roll_ani" ACT_ROLL_RIGHT 1 LX LY AZ fps 35
$sequence "roll_right_fire" "roll_right_fire_ani"
$sequence "r_corner_idle" "r_corner_idle_ani" fps 15
$sequence "r_corner_peek" "r_corner_peek_ani" fps 25
$sequence "r_corner_fire_death" "r_corner_fire_death_ani" {
	event 11 0 ""
	event 2002 37 ""
}
$sequence "r_corner_fire_step" "r_corner_fire_step_ani"
$sequence "r_corner_fire_return" "r_corner_fire_return_ani"
$sequence "r_corner_fire" "r_corner_fire_ani" {
	event 4 6 ""
	event 5001 6 "50"
	event 5 12 ""
	event 5001 12 "50"
	event 6 20 ""
	event 5001 20 "50"
}
$sequence "r_corner_grenade_throw" "r_corner_grenade_throw_ani" fps 25 {
	event 7 31 ""
}
$sequence "r_corner_roll" "r_corner_roll_ani" ACT_ROLL_LEFT 1 LX LY AZ fps 35
$sequence "roll_left_fire" "roll_left_fire_ani"
$sequence "alarm_trigger" "alarm_trigger_ani" {
	event 1003 59 "alarmbutton"
	event 5004 17 "tower/alarm_keypad.wav"
	event 5004 27 "tower/alarm_keypad.wav"
	event 5004 33 "tower/alarm_keypad.wav"
	event 5004 43 "tower/alarm_keypad.wav"
	event 5004 60 "tower/alarm_keypad2.wav"
}
$sequence "alarm_trigger_multiple" "alarm_trigger_multiple_ani" {
	event 5004 17 "tower/alarm_keypad.wav"
	event 5004 27 "tower/alarm_keypad.wav"
	event 5004 33 "tower/alarm_keypad.wav"
	event 5004 43 "tower/alarm_keypad.wav"
	event 5004 57 "tower/alarm_keypad2.wav"
	event 1003 58 "alarmbutton"
	event 1003 59 "alarmbutton"
	event 1003 60 "alarmbutton"
}
