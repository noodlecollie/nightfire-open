$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "YAKUZA.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "yakuza_body_coat_lod0_ref"
	studio "yakuza_body_coat_lod1_ref"
	studio "yakuza_body_coat_lod2_ref"
	studio "yakuza_body_coat_lod3_ref"
	studio "yakuza_body_coat_lod4_ref"
	studio "yakuza_body_vest_lod0_ref"
	studio "yakuza_body_vest_lod1_ref"
	studio "yakuza_body_vest_lod2_ref"
	studio "yakuza_body_vest_lod3_ref"
	studio "yakuza_body_vest_lod4_ref"
	blank
}
$bodygroup "heads"
{
	studio "yakuza_head_01_lod0_ref"
	studio "yakuza_head_01_lod1_ref"
	studio "yakuza_head_01_lod2_ref"
	studio "yakuza_head_01_lod3_ref"
	studio "yakuza_head_01_lod4_ref"
	studio "yakuza_head_02_lod0_ref"
	studio "yakuza_head_02_lod1_ref"
	studio "yakuza_head_02_lod2_ref"
	studio "yakuza_head_02_lod3_ref"
	studio "yakuza_head_02_lod4_ref"
	studio "yakuza_head_04_lod0_ref"
	studio "yakuza_head_04_lod1_ref"
	studio "yakuza_head_04_lod2_ref"
	studio "yakuza_head_04_lod3_ref"
	studio "yakuza_head_04_lod4_ref"
	blank
}
$bodygroup "accessories"
{
	blank
	studio "yakuza_glasses_ref"
}
$bodygroup "weapons"
{
	blank
	studio "yakuza_GUN_commando_lod0_ref"
	studio "yakuza_GUN_frinesi_ref"
	studio "yakuza_GUN_sniper_ref"
}
$bodygroup "skeleton"
{
	blank
	studio "skeleton_REFERENCE_ref"
}

$attachment "" "Bip01 R Hand" 30.400000 0.500000 3.600000
$attachment "" "W_SIG551" 29.000000 0.000000 0.000000
$attachment "" "W_SIG551" 45.000000 0.000000 0.000000
$attachment "" "Bip01" 0.000000 0.000000 48.000000
$attachment "" "Bip01 Head" 5.000000 0.000000 0.000000
$attachment "" "Bip01 Head" 5.000000 30.000000 0.000000

$controller 0 "Mouth_LINK" 32800 0 0
$controller 1 "Bone13" 32800 0 0
$controller 2 "Bip01 Head" XR -50 50

$hbox 0 "Bip01 Head" 0.000000 -3.700000 -3.000000 8.000000 3.700000 3.000000
$hbox 1 "Bip01 Neck" -2.000000 -3.000000 -3.000000 4.000000 3.000000 3.000000
$hbox 2 "Bip01 Spine2" -1.000000 2.000000 -7.000000 12.000000 7.000000 7.000000
$hbox 3 "Bip01 Spine2" -1.000000 2.000000 -7.000000 12.000000 -3.000000 7.000000
$hbox 4 "Bip01 Spine" -1.000000 1.000000 -7.000000 12.000000 6.000000 7.000000
$hbox 3 "Bip01 Spine" -1.000000 1.000000 -7.000000 12.000000 -4.000000 7.000000
$hbox 5 "Bip01 Pelvis" -5.000000 1.000000 -3.000000 1.000000 7.000000 3.000000
$hbox 14 "Bip01 L Thigh" 0.000000 -4.000000 -3.000000 18.000000 3.000000 3.000000
$hbox 15 "Bip01 L Calf" 0.000000 -4.000000 -3.000000 20.000000 3.000000 3.000000
$hbox 15 "Bip01 L Foot" 1.000000 -4.000000 -2.000000 4.000000 8.000000 2.000000
$hbox 12 "Bip01 R Thigh" 0.000000 -4.000000 -3.000000 18.000000 3.000000 3.000000
$hbox 13 "Bip01 R Calf" 0.000000 -4.000000 -3.000000 20.000000 3.000000 3.000000
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
$sequence "walk1" "walk1_ani" ACT_WALK 1 LX loop
$sequence "walkcasual" "walkcasual_ani" ACT_WALK 1 LX loop
$sequence "idle1" "idle1_ani" ACT_IDLE 2 fps 15
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 15
$sequence "idleheadscratch" "idleheadscratch_ani" ACT_IDLE 1 fps 15
$sequence "idleheadscratch2" "idleheadscratch2_ani" ACT_IDLE 1 fps 15
$sequence "idlecasual1" "idlecasual1_ani" ACT_IDLE 1 fps 25
$sequence "idlecasual2" "idlecasual2_ani" ACT_IDLE 1
$sequence "inspect_body" "inspect_body_ani" fps 12
$sequence "limpingwalk" "limpingwalk_ani" ACT_WALK_HURT 1 LX loop
$sequence "limpingrun" "limpingrun_ani" ACT_RUN_HURT 1 LX fps 40 loop
$sequence "alerted_patrol" "alerted_patrol_ani" ACT_WALK_SCARED 1 LX loop
$sequence "alerted_sneak1" "alerted_sneak1_ani" ACT_WALK_SCARED 1 LX loop
$sequence "alerted_sneak2" "alerted_sneak2_ani" ACT_WALK_SCARED 1 LX loop
$sequence "run" "run_ani" ACT_RUN 1 LX loop
$sequence "run_nogun" "run_nogun_ani" LX loop
$sequence "run_thruwall" "run_thruwall_ani" LX LY
$sequence "alerted_idle1" "alerted_idle1_ani" ACT_IDLE_ANGRY 2 fps 15
$sequence "alerted_idle2" "alerted_idle2_ani" ACT_IDLE_ANGRY 2 fps 15
$sequence "alerted_idle_nervous" "alerted_idle_nervous_ani" ACT_IDLE_ANGRY 1 fps 25
$sequence "alerted_idle_frantic" "alerted_idle_frantic_ani" ACT_IDLE_ANGRY 2 fps 25
$sequence "alerted_idle_searching" "alerted_idle_searching_ani" ACT_IDLE_ANGRY 2 fps 25
$sequence "combatidle" "combatidle_ani" ACT_COMBAT_IDLE 1 fps 15
$sequence "crouching_mgun_burst" "crouching_mgun_burst_ani" "crouching_mgun_burst_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "50"
	event 5 4 ""
	event 5001 4 "50"
	event 6 7 ""
	event 5001 7 "50"
}
$sequence "crouching_mgun_full" "crouching_mgun_full_ani" "crouching_mgun_full_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "50"
	event 5 4 ""
	event 5001 4 "50"
	event 6 7 ""
	event 5001 7 "50"
	event 4 10 ""
	event 5001 10 "50"
	event 5 13 ""
	event 5001 13 "50"
	event 6 16 ""
	event 5001 16 "50"
	event 4 19 ""
	event 5001 19 "50"
	event 5 22 ""
	event 5001 22 "50"
	event 6 25 ""
	event 5001 25 "50"
	event 4 28 ""
	event 5001 28 "50"
	event 5 31 ""
	event 5001 31 "50"
}
$sequence "standing_mgun_burst" "standing_mgun_burst_ani" "standing_mgun_burst_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "50"
	event 5 4 ""
	event 5001 4 "50"
	event 6 7 ""
	event 5001 7 "50"
}
$sequence "standing_mgun_full" "standing_mgun_full_ani" "standing_mgun_full_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "50"
	event 5 4 ""
	event 5001 4 "50"
	event 6 7 ""
	event 5001 7 "50"
	event 4 10 ""
	event 5001 10 "50"
	event 5 13 ""
	event 5001 13 "50"
	event 6 16 ""
	event 5001 16 "50"
	event 4 19 ""
	event 5001 19 "50"
	event 5 22 ""
	event 5001 22 "50"
	event 6 25 ""
	event 5001 25 "50"
	event 4 28 ""
	event 5001 28 "50"
	event 5 31 ""
	event 5001 31 "50"
}
$sequence "reload_mgun" "reload_mgun_ani" ACT_RELOAD 1 fps 20 {
	event 2 28 ""
}
$sequence "standing_sniper" "standing_sniper_ani" "standing_sniper_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 fps 20 {
	event 4 1 ""
	event 5001 1 "11"
}
$sequence "crouching_sniper" "crouching_sniper_ani" "crouching_sniper_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 fps 15 {
	event 4 1 ""
	event 5001 1 "11"
}
$sequence "reload_sniper" "reload_sniper_ani" ACT_RELOAD 1 fps 15 {
	event 2 28 ""
}
$sequence "crouching_sgun" "crouching_sgun_ani" "crouching_sgun_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "43"
}
$sequence "standing_sgun" "standing_sgun_ani" "standing_sgun_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "43"
}
$sequence "reload_Sgun" "reload_Sgun_ani" ACT_RELOAD 1 fps 20 {
	event 2 28 ""
}
$sequence "strafe_L_fire" "strafe_L_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 55 loop {
	event 5001 5 "11"
	event 4 5 ""
	event 5001 15 "11"
	event 5 15 ""
	event 5001 25 "11"
	event 6 25 ""
}
$sequence "strafe_R_fire" "strafe_R_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 55 loop {
	event 4 5 ""
	event 5001 5 "11"
	event 5 15 ""
	event 5001 15 "11"
	event 6 25 ""
	event 5001 25 "11"
}
$sequence "walkingback_fire" "walkingback_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 28 loop {
	event 4 5 ""
	event 5001 5 "11"
	event 5 15 ""
	event 5001 15 "11"
}
$sequence "running_fire" "running_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 25 loop {
	event 4 5 ""
	event 5001 5 "11"
	event 6 15 ""
	event 5001 15 "11"
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
$sequence "kick_grenade" "kick_grenade_ani" fps 35 {
	event 14 9 ""
}
$sequence "kick_door" "kick_door_ani"
$sequence "frontkick" "frontkick_ani" ACT_MELEE_ATTACK1 1 fps 25 {
	event 3 8 ""
	event 2010 8 ""
}
$sequence "frontkick_nogun" "frontkick_nogun_ani" ACT_MELEE_ATTACK2 1 fps 25 {
	event 3 6 ""
	event 2010 6 ""
}
$sequence "drop_grenade" "drop_grenade_ani" ACT_SPECIAL_ATTACK1 1 fps 40
$sequence "cower" "cower_ani" ACT_COWER 1 fps 20
$sequence "vent_peek" "vent_peek_ani" ACT_INSPECT_WALL 1
$sequence "vent_idle" "vent_idle_ani" ACT_INSPECT_WALL 1
$sequence "vent_stand" "vent_stand_ani" ACT_INSPECT_WALL 1
$sequence "vent_grenade" "vent_grenade_ani" ACT_INSPECT_WALL 1 {
	event 7 55 ""
}
$sequence "vent_shoot" "vent_shoot_ani" ACT_INSPECT_WALL 1 loop {
	event 4 1 ""
	event 5001 1 "50"
}
$sequence "roll_left" "roll_left_ani" ACT_ROLL_LEFT 1 LX LY AZ
$sequence "roll_right" "roll_right_ani" ACT_ROLL_RIGHT 1 LX LY AZ
$sequence "cower" "cower_ani" ACT_COWER 1 fps 32
$sequence "grenadecower" "grenadecower_ani" ACT_COWER 1 fps 35
$sequence "surrender" "surrender_ani" ACT_FEAR_DISPLAY 1 fps 15
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
$sequence "headflinch" "headflinch_ani" ACT_FLINCH_HEAD 1 fps 22
$sequence "backflinch" "backflinch_ani" ACT_INVALID 1 fps 26
$sequence "chestflinch" "chestflinch_ani" ACT_FLINCH_CHEST 1 fps 26
$sequence "blinded" "blinded_ani" ACT_INVALID 1 fps 27 loop
$sequence "blinded_noweapon" "blinded_noweapon_ani" fps 12 loop
$sequence "knockedout" "knockedout_ani" ACT_SLEEP 1 {
	event 11 0 ""
	event 2002 61 ""
}
$sequence "knockedout_wakeup" "knockedout_wakeup_ani" fps 20
$sequence "knockedoutidle" "knockedoutidle_ani" fps 15 loop
$sequence "knockedout2" "knockedout2_ani" ACT_SLEEP 1 {
	event 11 0 ""
	event 2002 109 ""
}
$sequence "knockedout2idle" "knockedout2idle_ani" fps 15
$sequence "blindedfire" "blindedfire_ani" ACT_INVALID 1 fps 34 loop {
	event 4 17 ""
	event 5001 17 "50"
	event 5 20 ""
	event 5001 20 "50"
	event 6 23 ""
	event 5001 23 "50"
	event 4 27 ""
	event 5001 27 "50"
	event 5 30 ""
	event 5001 30 "50"
	event 6 33 ""
	event 5001 33 "50"
	event 4 36 ""
	event 5001 36 "50"
	event 5 70 ""
	event 5001 70 "50"
	event 6 73 ""
	event 5001 73 "50"
	event 4 77 ""
	event 5001 77 "50"
}
$sequence "dieback1" "dieback1_ani" ACT_INVALID 3 fps 35 {
	event 11 5 ""
	event 2002 42 ""
}
$sequence "dieforward" "dieforward_ani" ACT_DIE_GUTSHOT 3 fps 35 {
	event 11 13 ""
	event 2002 31 ""
}
$sequence "dieforward2" "dieforward2_ani" ACT_DIESIMPLE 1 fps 35 {
	event 11 13 ""
	event 2002 31 ""
}
$sequence "diesimple" "diesimple_ani" ACT_DIESIMPLE 1 fps 25 {
	event 11 0 ""
	event 2002 35 ""
}
$sequence "diebackwards" "diebackwards_ani" ACT_DIEBACKWARD 2 fps 35 {
	event 11 6 ""
	event 2002 44 ""
}
$sequence "dieheadshot" "dieheadshot_ani" ACT_DIE_HEADSHOT 3 fps 25 {
	event 11 1 ""
	event 2002 45 ""
}
$sequence "diegutshot" "diegutshot_ani" ACT_DIE_GUTSHOT 3 {
	event 11 10 ""
	event 2002 60 ""
}
$sequence "dienutshot" "dienutshot_ani" ACT_INVALID 3 {
	event 11 1 ""
	event 2002 142 ""
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
$sequence "die_shotgun1" "die_shotgun1_ani" ACT_DIEVIOLENT 1 fps 20 {
	event 11 0 ""
	event 2002 16 ""
}
$sequence "die_shotgun2" "die_shotgun2_ani" ACT_DIEVIOLENT 1 fps 25 {
	event 11 0 ""
	event 2002 16 ""
}
$sequence "l_corner_idle" "l_corner_idle_ani" fps 15
$sequence "l_corner_peek" "l_corner_peek_ani"
$sequence "l_roll_left" "l_roll_left_ani" ACT_ROLL_RIGHT 1 LX LY AZ
$sequence "l_corner_fire_death" "l_corner_fire_death_ani" {
	event 11 10 ""
	event 2002 60 ""
}
$sequence "l_corner_fire_step" "l_corner_fire_step_ani"
$sequence "l_corner_fire_return" "l_corner_fire_return_ani"
$sequence "l_corner_fire" "l_corner_fire_ani" {
	event 4 6 ""
	event 5001 6 "50"
	event 5 14 ""
	event 5001 14 "50"
	event 6 21 ""
	event 5001 21 "50"
}
$sequence "r_corner_idle" "r_corner_idle_ani" fps 15
$sequence "r_corner_peek" "r_corner_peek_ani"
$sequence "r_roll_right" "r_roll_right_ani" ACT_ROLL_LEFT 1 LX LY AZ
$sequence "r_corner_fire_death" "r_corner_fire_death_ani" {
	event 11 10 ""
	event 2002 60 ""
}
$sequence "r_corner_fire_step" "r_corner_fire_step_ani"
$sequence "r_corner_fire_return" "r_corner_fire_return_ani"
$sequence "r_corner_fire" "r_corner_fire_ani" {
	event 4 0 ""
	event 5001 0 "50"
	event 5 7 ""
	event 5001 7 "50"
	event 6 14 ""
	event 5001 14 "50"
}
$sequence "roll_right_fire" "roll_right_fire_ani"
$sequence "roll_left_fire" "roll_left_fire_ani"
$sequence "hostage" "hostage_ani" ACT_INVALID 1 fps 20 loop
$sequence "hostage_threat" "hostage_threat_ani" ACT_INVALID 1 loop {
	event 5006 1 "hostage"
}
$sequence "hostage_kill" "hostage_kill_ani" {
	event 4 5 ""
	event 5001 5 "50"
	event 5 12 ""
	event 5001 12 "50"
}
$sequence "death_idle" "death_idle_ani" fps 20 loop
$sequence "window_jump" "window_jump_ani" {
	event 4 0 ""
	event 5001 0 "50"
	event 5 7 ""
	event 5001 7 "50"
	event 6 14 ""
	event 5001 14 "50"
	event 4 21 ""
	event 5001 21 "50"
	event 5 28 ""
	event 5001 28 "50"
}
$sequence "garden_snipe" "garden_snipe_ani" fps 15
$sequence "bridgedie1" "bridgedie1_ani" fps 20 {
	event 11 5 ""
}
$sequence "bridgedie1idle" "bridgedie1idle_ani" fps 20 loop {
	event 2001 0 ""
}
$sequence "typing" "typing_ani" loop
$sequence "ronin_toss" "ronin_toss_ani"
$sequence "escapedie_center1" "escapedie_center1_ani" fps 15 {
	event 11 5 ""
}
$sequence "escapedie_center2" "escapedie_center2_ani" fps 15 {
	event 11 5 ""
}
$sequence "escapedie_center3" "escapedie_center3_ani" fps 15 {
	event 11 5 ""
}
$sequence "escapedie_center4" "escapedie_center4_ani" fps 15 {
	event 11 5 ""
}
$sequence "escapedie_sides1" "escapedie_sides1_ani" fps 15 {
	event 11 5 ""
}
$sequence "escapedie_sides2" "escapedie_sides2_ani" fps 15 {
	event 11 5 ""
}
$sequence "escapedie_sides3" "escapedie_sides3_ani" fps 15 {
	event 11 5 ""
}
$sequence "escapedie_sides4" "escapedie_sides4_ani" fps 15 {
	event 11 5 ""
}
$sequence "escapedie_sides5" "escapedie_sides5_ani" fps 15 {
	event 11 5 ""
}
$sequence "escapedie_sides6" "escapedie_sides6_ani" fps 15 {
	event 11 5 ""
}
$sequence "blown_back" "blown_back_ani"
