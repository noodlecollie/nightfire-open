$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "security_guard.mdl"

$root "Bip01"

$flags 18432
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "security_guard_lod0_ref"
	studio "security_guard_lod1_ref"
	studio "security_guard_lod2_ref"
	studio "security_guard_lod3_ref"
	studio "security_guard_lod4_ref"
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
	studio "security_guard_taser_Reference_ref"
	studio "security_guard_taser_Reference_ref"
}
$bodygroup "skeleton"
{
	blank
	studio "security_guard_skeleton_ref"
}

$attachment "" "Bip01 R Hand" 7.300000 -1.500000 15.700000
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

$sequence "null" "null_ani" loop
$sequence "a" "a_ani" loop
$sequence "e" "e_ani" loop
$sequence "o" "o_ani" loop
$sequence "f" "f_ani" loop
$sequence "s" "s_ani" loop
$sequence "m" "m_ani" loop
$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 15
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 15
$sequence "idle3" "idle3_ani" ACT_IDLE 1 fps 15
$sequence "idle4" "idle4_ani" ACT_IDLE 1 fps 15
$sequence "idle5" "idle5_ani" ACT_IDLE 1 fps 15
$sequence "idle6" "idle6_ani" ACT_IDLE 1 fps 15
$sequence "idle7" "idle7_ani" ACT_IDLE 1 fps 15
$sequence "walk" "walk_ani" ACT_WALK 1 LX LY fps 38 loop {
	event 5006 1 "whistle"
}
$sequence "walk1" "walk1_ani" ACT_WALK 1 LX LY fps 41 loop {
	event 5006 1 "whistle"
}
$sequence "walk2" "walk2_ani" ACT_WALK 1 LX LY fps 39 loop {
	event 5006 36 "whistle"
}
$sequence "walk3" "walk3_ani" ACT_WALK 1 LX LY fps 40 loop {
	event 5006 37 "whistle"
}
$sequence "alerted_patrol1" "alerted_patrol1_ani" ACT_WALK_SCARED 1 LX LY loop
$sequence "alerted_patrol2" "alerted_patrol2_ani" ACT_WALK_SCARED 1 LX LY loop
$sequence "alerted1" "alerted1_ani" ACT_IDLE_ANGRY 1 LX LY loop
$sequence "alerted2" "alerted2_ani" ACT_IDLE_ANGRY 1 LX LY loop
$sequence "run" "run_ani" ACT_RUN 1 LX LY loop
$sequence "run_scared" "run_scared_ani" ACT_EXCITED 1 LX LY loop
$sequence "surrender" "surrender_ani" ACT_FEAR_DISPLAY 1 fps 15
$sequence "alarm_trigger" "alarm_trigger_ani" {
	event 5004 17 "tower/alarm_keypad.wav"
	event 5004 27 "tower/alarm_keypad.wav"
	event 5004 33 "tower/alarm_keypad.wav"
	event 5004 43 "tower/alarm_keypad.wav"
	event 5004 60 "tower/alarm_keypad2.wav"
	event 1003 54 "alarmbutton"
	event 1003 55 "alarmbutton"
	event 1003 56 "alarmbutton"
	event 1003 57 "alarmbutton"
	event 1003 58 "alarmbutton"
	event 1003 59 "alarmbutton"
	event 1003 60 "alarmbutton"
}
$sequence "melee" "melee_ani" ACT_MELEE_ATTACK1 1 {
	event 5004 12 "weapons/taser_fire.wav"
	event 3 12 ""
	event 5001 12 "24"
	event 5001 13 "24"
	event 5001 14 "24"
	event 5001 15 "24"
	event 5001 16 "24"
	event 5001 17 "24"
	event 5001 18 "24"
	event 5001 19 "24"
	event 5001 20 "24"
	event 5001 21 "24"
	event 5001 22 "24"
	event 5001 23 "24"
	event 5001 24 "24"
	event 5001 25 "24"
}
$sequence "kick_grenade" "kick_grenade_ani" fps 20 {
	event 14 9 ""
}
$sequence "limpingwalk" "limpingwalk_ani" ACT_WALK_HURT 1 LX loop
$sequence "limpingrun" "limpingrun_ani" ACT_RUN_HURT 1 LX fps 40 loop
$sequence "door_check" "door_check_ani" fps 24
$sequence "chatting1" "chatting1_ani" fps 20
$sequence "chatting2" "chatting2_ani" fps 20
$sequence "chatting3" "chatting3_ani" fps 20
$sequence "chatting4" "chatting4_ani" fps 20
$sequence "chatting_down1" "chatting_down1_ani"
$sequence "chatting_down2" "chatting_down2_ani"
$sequence "chatting_down3" "chatting_down3_ani"
$sequence "chatting_down4" "chatting_down4_ani"
$sequence "chatting_up1" "chatting_up1_ani"
$sequence "chatting_up2" "chatting_up2_ani"
$sequence "chatting_up3" "chatting_up3_ani"
$sequence "inspect_body" "inspect_body_ani" fps 15
$sequence "distracted_atrium" "distracted_atrium_ani" fps 20
$sequence "distracted_fountain" "distracted_fountain_ani" fps 28 {
	event 5004 30 "tower\water_fountain.wav"
}
$sequence "distracted_window" "distracted_window_ani"
$sequence "flinch" "flinch_ani" ACT_FLINCH_HEAD 1 fps 28
$sequence "knockedout" "knockedout_ani" ACT_SLEEP 1 LX LY {
	event 2002 70 ""
}
$sequence "knockedoutidle" "knockedoutidle_ani" LX LY fps 15 loop
$sequence "knockedout2" "knockedout2_ani" ACT_SLEEP 1 LX LY {
	event 2002 70 ""
}
$sequence "knockedout2idle" "knockedout2idle_ani" LX LY fps 15 loop
$sequence "knockedout_wakeup" "knockedout_wakeup_ani" LX LY fps 32
$sequence "blinded" "blinded_ani" ACT_INVALID 1 fps 12 loop
$sequence "electrocution" "electrocution_ani" ACT_BIG_FLINCH 1 fps 35
$sequence "sleeping_idle" "sleeping_idle_ani" fps 15 loop
$sequence "sleeping_death" "sleeping_death_ani" fps 20
$sequence "sleeping_death_idle" "sleeping_death_idle_ani" fps 20 loop
$sequence "dieheadshot" "dieheadshot_ani" ACT_DIE_HEADSHOT 1 {
	event 2002 46 ""
}
$sequence "dienutshot" "dienutshot_ani" ACT_DIE_GUTSHOT 1 {
	event 2002 46 ""
}
$sequence "diebackwards" "diebackwards_ani" ACT_DIEBACKWARD 1 {
	event 2002 46 ""
}
$sequence "dieforward" "dieforward_ani" ACT_DIESIMPLE 1 {
	event 2002 46 ""
}
$sequence "diegutshot" "diegutshot_ani" ACT_DIE_GUTSHOT 1 {
	event 2002 46 ""
}
