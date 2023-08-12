$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "maintenance_worker.mdl"

$root "Bip01"

$flags 18432
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "maint_worker_lod0_ref"
	studio "maint_worker_lod1_ref"
	studio "maint_worker_lod2_ref"
	studio "maint_worker_lod3_ref"
	studio "maint_worker_lod4_ref"
	blank
}
$bodygroup "heads"
{
	studio "maint_worker_head1_lod0_ref"
	studio "maint_worker_head1_lod1_ref"
	studio "maint_worker_head1_lod2_ref"
	studio "maint_worker_head1_lod3_ref"
	studio "maint_worker_head1_lod4_ref"
	studio "maint_worker_head2_lod0_ref"
	studio "maint_worker_head2_lod1_ref"
	studio "maint_worker_head2_lod2_ref"
	studio "maint_worker_head2_lod3_ref"
	studio "maint_worker_head2_lod4_ref"
	studio "maint_worker_head3_lod0_ref"
	studio "maint_worker_head3_lod1_ref"
	studio "maint_worker_head3_lod2_ref"
	studio "maint_worker_head3_lod3_ref"
	studio "maint_worker_head3_lod4_ref"
	blank
}
$bodygroup "accessories"
{
	blank
	studio "maint_worker_hat_lod0_ref"
	studio "maint_worker_hat_lod1_ref"
	studio "maint_worker_hat_lod2_ref"
	studio "maint_worker_hat_lod3_ref"
	studio "maint_worker_hat_lod4_ref"
}
$bodygroup "weapons"
{
	blank
	studio "maint_worker_wrench_reference_ref"
}
$bodygroup "skeleton"
{
	blank
	studio "maint_worker_skeleton_ref"
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

$hbox 0 "Bip01 Head" 0.000000 -4.000000 -4.000000 9.000000 5.000000 4.000000
$hbox 1 "Bip01 Neck" -2.000000 -3.000000 -3.000000 4.000000 3.000000 3.000000
$hbox 2 "Bip01 Spine2" -2.000000 0.000000 -6.000000 10.000000 6.000000 7.000000
$hbox 3 "Bip01 Spine2" -2.000000 0.000000 -7.000000 10.000000 -6.000000 7.000000
$hbox 4 "Bip01 Spine" -4.000000 0.000000 -7.000000 8.000000 6.000000 7.000000
$hbox 3 "Bip01 Spine" -4.000000 0.000000 -7.000000 8.000000 -6.000000 7.000000
$hbox 5 "Bip01 Pelvis" -5.000000 1.000000 -3.000000 1.000000 7.000000 3.000000
$hbox 14 "Bip01 L Thigh" 0.000000 -4.000000 -3.000000 20.000000 3.000000 3.000000
$hbox 15 "Bip01 L Calf" 0.000000 -4.000000 -3.000000 18.000000 3.000000 3.000000
$hbox 15 "Bip01 L Foot" 1.000000 -4.000000 -2.000000 4.000000 8.000000 2.000000
$hbox 12 "Bip01 R Thigh" 0.000000 -4.000000 -3.000000 20.000000 3.000000 3.000000
$hbox 13 "Bip01 R Calf" 0.000000 -4.000000 -3.000000 18.000000 3.000000 3.000000
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
$sequence "idle_1" "idle_1_ani" ACT_IDLE 1 fps 15
$sequence "idle_2" "idle_2_ani" ACT_IDLE 1 fps 15
$sequence "idle_3" "idle_3_ani" ACT_IDLE 1 fps 15
$sequence "idle_4" "idle_4_ani" ACT_IDLE 1 fps 15
$sequence "idle_5" "idle_5_ani" ACT_IDLE 1 fps 15
$sequence "walk" "walk_ani" ACT_WALK 1 LX fps 25 loop
$sequence "alerted_patrol1" "alerted_patrol1_ani" ACT_WALK_SCARED 1 LX fps 25 loop
$sequence "alerted1" "alerted1_ani" ACT_IDLE_ANGRY 1 loop
$sequence "alerted2" "alerted2_ani" ACT_IDLE_ANGRY 1 loop
$sequence "cower" "cower_ani" ACT_COWER 1 fps 20 loop
$sequence "cowerrun" "cowerrun_ani" ACT_EXCITED 1 LX LY fps 25 loop
$sequence "surrender" "surrender_ani" ACT_FEAR_DISPLAY 1 fps 15
$sequence "run" "run_ani" ACT_RUN 1 LX LY loop
$sequence "melee" "melee_ani" ACT_MELEE_ATTACK1 1 {
	event 3 13 ""
}
$sequence "alarm_trigger" "alarm_trigger_ani" {
	event 5004 17 "tower/alarm_keypad.wav"
	event 5004 27 "tower/alarm_keypad.wav"
	event 5004 33 "tower/alarm_keypad.wav"
	event 5004 43 "tower/alarm_keypad.wav"
	event 1003 57 "alarmbutton"
	event 1003 58 "alarmbutton"
	event 1003 59 "alarmbutton"
	event 1003 60 "alarmbutton"
	event 5004 60 "tower/alarm_keypad2.wav"
}
$sequence "kick_grenade" "kick_grenade_ani" fps 20 {
	event 14 9 ""
}
$sequence "limpingwalk" "limpingwalk_ani" ACT_WALK_HURT 1 LX loop
$sequence "limpingrun" "limpingrun_ani" ACT_RUN_HURT 1 LX fps 40 loop
$sequence "machineroom_ledge_idle" "machineroom_ledge_idle_ani" fps 15
$sequence "machineroom_ledge_alerted" "machineroom_ledge_alerted_ani"
$sequence "machineroom_ledge_investigate" "machineroom_ledge_investigate_ani" fps 20
$sequence "machineroom_ledge_yelling" "machineroom_ledge_yelling_ani"
$sequence "machineroom_acknowledge" "machineroom_acknowledge_ani" fps 27
$sequence "machineroom_tinkering" "machineroom_tinkering_ani" fps 27
$sequence "chatting1" "chatting1_ani" fps 20
$sequence "chatting2" "chatting2_ani" fps 20
$sequence "chatting3" "chatting3_ani" fps 20
$sequence "chatting4" "chatting4_ani" fps 20
$sequence "inspect_body" "inspect_body_ani" fps 15
$sequence "flinch" "flinch_ani" ACT_FLINCH_HEAD 1 fps 28
$sequence "knockedout" "knockedout_ani" ACT_SLEEP 1 LX LY {
	event 2002 61 ""
}
$sequence "knockedoutidle" "knockedoutidle_ani" fps 15 loop
$sequence "knockedout2" "knockedout2_ani" ACT_SLEEP 1 LX LY {
	event 2002 101 ""
}
$sequence "knockedout2idle" "knockedout2idle_ani" fps 15 loop
$sequence "knockedout_wakeup" "knockedout_wakeup_ani"
$sequence "blindedstager" "blindedstager_ani" ACT_INVALID 1 LX LY loop
$sequence "electrocution" "electrocution_ani" ACT_BIG_FLINCH 1 fps 35
$sequence "dieheadshot" "dieheadshot_ani" ACT_DIE_HEADSHOT 1 fps 40
$sequence "dienutshot" "dienutshot_ani" ACT_DIE_GUTSHOT 1 fps 35
$sequence "diebackwards" "diebackwards_ani" ACT_DIEBACKWARD 1 fps 40
$sequence "dieforward" "dieforward_ani" ACT_DIESIMPLE 1 fps 40
$sequence "diegutshot" "diegutshot_ani" ACT_DIE_GUTSHOT 1 fps 25
