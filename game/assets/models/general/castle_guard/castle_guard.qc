$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "castle_guard.mdl"

$root "Bip01"

$flags 18432
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "castle_guard_body_lod0_ref"
	studio "castle_guard_body_lod1_ref"
	studio "castle_guard_body_lod2_ref"
	studio "castle_guard_body_lod3_ref"
	studio "castle_guard_body_lod4_ref"
	blank
}
$bodygroup "heads"
{
	studio "castle_guard_head1_lod0_ref"
	studio "castle_guard_head1_lod1_ref"
	studio "castle_guard_head1_lod2_ref"
	studio "castle_guard_head1_lod3_ref"
	studio "castle_guard_head1_lod4_ref"
	studio "castle_guard_head2_lod0_ref"
	studio "castle_guard_head2_lod1_ref"
	studio "castle_guard_head2_lod2_ref"
	studio "castle_guard_head2_lod3_ref"
	studio "castle_guard_head2_lod4_ref"
	studio "castle_guard_head3_lod0_ref"
	studio "castle_guard_head3_lod1_ref"
	studio "castle_guard_head3_lod2_ref"
	studio "castle_guard_head3_lod3_ref"
	studio "castle_guard_head3_lod4_ref"
	blank
}
$bodygroup "accessories"
{
	blank
	studio "castle_guard_glasses_lod0_ref"
	studio "castle_guard_glasses_lod1_ref"
	studio "castle_guard_glasses_lod2_ref"
	studio "castle_guard_glasses_lod3_ref"
}
$bodygroup "weapons"
{
	blank
	studio "castle_guard_glock_lod0_ref"
	studio "castle_guard_glock_lod1_ref"
	studio "castle_guard_glock_lod2_ref"
	studio "castle_guard_glock_lod4_ref"
	studio "castle_guard_glock_drawn_lod0_ref"
	studio "castle_guard_glock_drawn_lod1_ref"
	studio "castle_guard_glock_drawn_lod2_ref"
	studio "castle_guard_glock_drawn_lod4_ref"
}
$bodygroup "skeleton"
{
	blank
	studio "castle_guard_skeleton_reference_ref"
}

$attachment "" "Bip01 R Hand" 10.000000 1.000000 4.000000
$attachment "" "Bip01 R Hand" 9.000000 1.000000 2.000000
$attachment "" "Bip01 R Hand" 20.000000 1.000000 2.000000
$attachment "" "Bip01" 0.000000 0.000000 48.000000
$attachment "" "Bip01 Head" 5.000000 0.000000 0.000000
$attachment "" "Bip01 Head" 5.000000 30.000000 0.000000

$controller 0 "Bone39" 32800 0 0
$controller 1 "Bone21" 32800 0 0
$controller 2 "Bip01 Head" XR -50 50

$hbox 0 "Bip01 Head" 0.000000 -3.700000 -3.000000 8.000000 3.700000 3.000000
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
$sequence "walk1" "walk1_ani" ACT_WALK 1 LX fps 35 loop
$sequence "turn_L" "turn_L_ani" ACT_TURN_LEFT 1 fps 40
$sequence "turn_R" "turn_R_ani" ACT_TURN_RIGHT 1 fps 40
$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 12
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 15
$sequence "idleheadscratch" "idleheadscratch_ani" ACT_IDLE 1 fps 15
$sequence "idleheadscratch2" "idleheadscratch2_ani" ACT_IDLE 1 fps 15
$sequence "idle3" "idle3_ani" ACT_IDLE 1 fps 15
$sequence "inspect_body" "inspect_body_ani" fps 14
$sequence "limpingwalk" "limpingwalk_ani" ACT_WALK_HURT 1 LX loop
$sequence "limpingrun" "limpingrun_ani" ACT_RUN_HURT 1 LX fps 40 loop
$sequence "alerted_walk" "alerted_walk_ani" ACT_WALK_SCARED 1 LX loop
$sequence "alerted_walkpatrol1" "alerted_walkpatrol1_ani" ACT_WALK_SCARED 2 LX loop
$sequence "alerted_walkpatrol2" "alerted_walkpatrol2_ani" ACT_WALK_SCARED 2 LX loop
$sequence "alerted_walksneaking" "alerted_walksneaking_ani" ACT_WALK_SCARED 2 LX loop
$sequence "run" "run_ani" ACT_RUN 1 LX fps 32 loop
$sequence "alerted_idle1" "alerted_idle1_ani" ACT_IDLE_ANGRY 1 fps 25 loop
$sequence "alerted_idle2" "alerted_idle2_ani" ACT_IDLE_ANGRY 1 loop
$sequence "alerted_idle3" "alerted_idle3_ani" ACT_IDLE_ANGRY 1 loop
$sequence "alerted_idlesearching" "alerted_idlesearching_ani" ACT_IDLE_ANGRY 2 loop
$sequence "alerted_idlesearching2" "alerted_idlesearching2_ani" ACT_IDLE_ANGRY 2 loop
$sequence "draw_gun" "draw_gun_ani" ACT_ARM 1 fps 45 {
	event 16 19 ""
}
$sequence "holster_gun" "holster_gun_ani" ACT_DISARM 1
$sequence "combatidle" "combatidle_ani" ACT_COMBAT_IDLE 1 fps 15
$sequence "crouching_glock" "crouching_glock_ani" "crouching_glock_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 fps 27 {
	event 4 2 ""
	event 5001 2 "51"
}
$sequence "standing_glock" "standing_glock_ani" "standing_glock_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 fps 27 {
	event 4 1 ""
	event 5001 1 "51"
}
$sequence "reload_glock" "reload_glock_ani" ACT_RELOAD 1 fps 20 {
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
$sequence "kick_grenade" "kick_grenade_ani" fps 28 {
	event 14 9 ""
}
$sequence "kick_door" "kick_door_ani"
$sequence "frontkick" "frontkick_ani" ACT_MELEE_ATTACK1 1 {
	event 3 13 ""
	event 2010 13 ""
}
$sequence "vent_peek" "vent_peek_ani" ACT_INSPECT_WALL 1
$sequence "vent_idle" "vent_idle_ani" ACT_INSPECT_WALL 1
$sequence "vent_stand" "vent_stand_ani" ACT_INSPECT_WALL 1
$sequence "vent_grenade" "vent_grenade_ani" ACT_INSPECT_WALL 1 {
	event 7 53 ""
}
$sequence "vent_shoot" "vent_shoot_ani" ACT_INSPECT_WALL 1 fps 20 loop {
	event 4 1 ""
	event 5001 1 "51"
}
$sequence "strafe_L_fire" "strafe_L_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 50 loop {
	event 4 8 ""
	event 5001 8 "51"
	event 5 16 ""
	event 5001 16 "51"
	event 6 24 ""
	event 5001 24 "51"
	event 4 32 ""
	event 5001 32 "51"
}
$sequence "strafe_R_fire" "strafe_R_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 40 loop {
	event 4 2 ""
	event 5001 2 "51"
	event 5 9 ""
	event 5001 9 "51"
	event 6 17 ""
	event 5001 17 "51"
	event 4 24 ""
	event 5001 24 "51"
}
$sequence "walkingback_fire" "walkingback_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 28 loop {
	event 4 2 ""
	event 5001 2 "51"
	event 5 8 ""
	event 5001 8 "51"
	event 6 14 ""
	event 5001 14 "51"
}
$sequence "running_fire" "running_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 35 loop {
	event 4 3 ""
	event 5001 3 "51"
	event 5 8 ""
	event 5001 8 "51"
	event 6 13 ""
	event 5001 13 "51"
}
$sequence "roll_left" "roll_left_ani" ACT_ROLL_LEFT 1 LX LY AZ fps 35
$sequence "roll_right" "roll_right_ani" ACT_ROLL_RIGHT 1 LX LY AZ fps 35
$sequence "cower" "cower_ani" ACT_COWER 1 fps 32
$sequence "surrender" "surrender_ani" ACT_FEAR_DISPLAY 1 fps 15
$sequence "surprised" "surprised_ani" ACT_THREAT_DISPLAY 1
$sequence "grenadecower" "grenadecower_ani" ACT_COWER 1 fps 35
$sequence "leftlegupperflinch" "leftlegupperflinch_ani" ACT_FLINCH_RIGHTLEG 1 fps 20
$sequence "leftleglowerflinch" "leftleglowerflinch_ani" ACT_INVALID 1 fps 20
$sequence "rightlegupperflinch" "rightlegupperflinch_ani" ACT_INVALID 1 fps 20
$sequence "rightleglowerflinch" "rightleglowerflinch_ani" ACT_INVALID 1 fps 20
$sequence "electrocution" "electrocution_ani" ACT_BIG_FLINCH 1 loop
$sequence "rightarmlowerflinch" "rightarmlowerflinch_ani" ACT_INVALID 1 fps 22
$sequence "rightarmhandflinch" "rightarmhandflinch_ani" ACT_INVALID 1 fps 22
$sequence "rightarmupperflinch" "rightarmupperflinch_ani" ACT_FLINCH_LEFTLEG 1 fps 22
$sequence "leftarmlowerflinch" "leftarmlowerflinch_ani" ACT_INVALID 1 fps 22
$sequence "leftarmhandflinch" "leftarmhandflinch_ani" ACT_INVALID 1
$sequence "leftarmupperflinch" "leftarmupperflinch_ani" ACT_FLINCH_RIGHTARM 1 fps 22
$sequence "stomachflinch" "stomachflinch_ani" ACT_FLINCH_STOMACH 1 fps 28
$sequence "chestflinch" "chestflinch_ani" ACT_FLINCH_CHEST 1 fps 28
$sequence "groinflinch" "groinflinch_ani" ACT_FLINCH_LEFTARM 1 fps 28
$sequence "headflinch" "headflinch_ani" ACT_FLINCH_HEAD 1 fps 22
$sequence "backflinch" "backflinch_ani" ACT_INVALID 1 fps 26
$sequence "blinded" "blinded_ani" ACT_INVALID 1 fps 27 loop
$sequence "blinded_noweapon" "blinded_noweapon_ani" fps 12 loop
$sequence "knockedout" "knockedout_ani" ACT_SLEEP 1 {
	event 11 10 ""
	event 2001 61 ""
	event 2002 61 ""
}
$sequence "knockedoutidle" "knockedoutidle_ani" fps 20 loop
$sequence "knockedout2" "knockedout2_ani" ACT_SLEEP 1 fps 35 {
	event 11 10 ""
	event 2001 109 ""
	event 2002 109 ""
}
$sequence "knockedout2idle" "knockedout2idle_ani" LX LY fps 20 loop
$sequence "knockedout_wakeup" "knockedout_wakeup_ani"
$sequence "blindedfire" "blindedfire_ani" ACT_INVALID 1 loop {
	event 4 15 ""
	event 5001 15 "51"
	event 4 25 ""
	event 5001 25 "51"
	event 4 35 ""
	event 5001 35 "51"
	event 4 51 ""
	event 5001 51 "51"
	event 4 61 ""
	event 5001 61 "51"
}
$sequence "choking" "choking_ani" ACT_BARNACLE_PULL 1 loop
$sequence "choking_stager" "choking_stager_ani" ACT_BARNACLE_PULL 1 loop
$sequence "dieback1" "dieback1_ani" ACT_DIEBACKWARD 1 fps 35 {
	event 11 0 ""
	event 2001 17 ""
	event 2002 50 ""
}
$sequence "dieback1_pose" "dieback1_pose_ani" fps 35
$sequence "dieforward" "dieforward_ani" ACT_DIEFORWARD 1 fps 35 {
	event 11 0 ""
	event 2001 20 ""
	event 2002 37 ""
}
$sequence "dieforward_pose" "dieforward_pose_ani" fps 35
$sequence "diesimple" "diesimple_ani" ACT_DIESIMPLE 1 fps 35 {
	event 11 0 ""
	event 2001 20 ""
	event 2002 37 ""
}
$sequence "diesimple_pose" "diesimple_pose_ani"
$sequence "diebackwards" "diebackwards_ani" ACT_DIEBACKWARD 1 fps 35 {
	event 11 0 ""
	event 2001 12 ""
	event 2002 50 ""
}
$sequence "diebackwards_pose" "diebackwards_pose_ani" fps 35
$sequence "dieheadshot" "dieheadshot_ani" ACT_DIE_HEADSHOT 1 fps 25 {
	event 11 0 ""
	event 2001 20 ""
	event 2002 45 ""
}
$sequence "dieheadshot_pose" "dieheadshot_pose_ani"
$sequence "diegutshot" "diegutshot_ani" ACT_DIE_GUTSHOT 1 fps 25 {
	event 11 0 ""
	event 2001 39 ""
	event 2002 37 ""
}
$sequence "diegutshot_pose" "diegutshot_pose_ani" fps 25
$sequence "nutshot" "nutshot_ani" ACT_DIE_GUTSHOT 1 fps 35 {
	event 11 0 ""
	event 2001 39 ""
	event 2002 37 ""
}
$sequence "nutshot_pose" "nutshot_pose_ani" fps 35
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
	event 11 5 ""
	event 2002 22 ""
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
$sequence "l_corner_idle" "l_corner_idle_ani" fps 15
$sequence "l_corner_peek" "l_corner_peek_ani"
$sequence "l_corner_throwgrenade" "l_corner_throwgrenade_ani"
$sequence "l_corner_fire_death" "l_corner_fire_death_ani" fps 35 {
	event 11 0 ""
	event 2001 39 ""
	event 2002 37 ""
}
$sequence "l_corner_fire_step" "l_corner_fire_step_ani"
$sequence "l_corner_fire_return" "l_corner_fire_return_ani"
$sequence "l_corner_fire" "l_corner_fire_ani" {
	event 4 6 ""
	event 5001 6 "51"
	event 4 13 ""
	event 5001 13 "51"
	event 4 22 ""
	event 5001 22 "51"
}
$sequence "r_corner_idle" "r_corner_idle_ani" fps 15
$sequence "r_corner_peek" "r_corner_peek_ani"
$sequence "r_corner_throwgrenade" "r_corner_throwgrenade_ani"
$sequence "r_corner_fire_death" "r_corner_fire_death_ani" {
	event 11 0 ""
	event 2001 39 ""
	event 2002 37 ""
}
$sequence "r_corner_fire_step" "r_corner_fire_step_ani"
$sequence "r_corner_fire_return" "r_corner_fire_return_ani"
$sequence "r_corner_fire" "r_corner_fire_ani" {
	event 4 1 ""
	event 5001 1 "51"
	event 4 11 ""
	event 5001 11 "51"
	event 4 20 ""
	event 5001 20 "51"
}
$sequence "l_corner_roll" "l_corner_roll_ani" ACT_ROLL_RIGHT 1 LX LY AZ fps 35
$sequence "roll_right_fire" "roll_right_fire_ani" {
	event 4 1 ""
	event 5001 1 "51"
}
$sequence "r_corner_roll" "r_corner_roll_ani" ACT_ROLL_LEFT 1 LX LY AZ fps 35
$sequence "roll_left_fire" "roll_left_fire_ani" {
	event 4 1 ""
	event 5001 1 "51"
}
