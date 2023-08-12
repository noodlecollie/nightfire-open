$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "maintenance_worker_clipboard.mdl"

$root "Bip01"

$flags 18432
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "maint_worker_clipboard_ref"
	blank
}
$bodygroup "heads"
{
	studio "maint_worker_head_1_ref"
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
}
$bodygroup "skeleton"
{
	blank
}

$attachment "" "Bip01 Head" 10.000000 1.000000 4.000000
$attachment "" "Bip01 Head" 9.000000 1.000000 2.000000
$attachment "" "Bip01 Head" 20.000000 1.000000 2.000000
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
$sequence "idle1" "idle1_ani" ACT_IDLE 1
$sequence "idle2" "idle2_ani" ACT_IDLE 1
$sequence "idle3" "idle3_ani" ACT_IDLE 1
$sequence "cower" "cower_ani" ACT_COWER 1 fps 20
$sequence "cowerrun" "cowerrun_ani" ACT_EXCITED 1 LX LY fps 25
$sequence "alerted" "alerted_ani" ACT_FEAR_DISPLAY 1 LX LY fps 25
$sequence "alerted2" "alerted2_ani" ACT_IDLE_ANGRY 1 LX LY fps 25
$sequence "walk" "walk_ani" ACT_WALK 1 LX loop
$sequence "run" "run_ani" ACT_RUN 1 LX LY loop
$sequence "death" "death_ani" ACT_DIE_GUTSHOT 1 fps 25 {
	event 2002 45 ""
}
$sequence "alarm_trigger" "alarm_trigger_ani" {
	event 1003 60 "alarmbutton"
	event 5004 17 "tower/alarm_keypad.wav"
	event 5004 27 "tower/alarm_keypad.wav"
	event 5004 33 "tower/alarm_keypad.wav"
	event 5004 43 "tower/alarm_keypad.wav"
	event 5004 60 "tower/alarm_keypad2.wav"
}
$sequence "knockedout" "knockedout_ani" ACT_SLEEP 1 LX LY {
	event 2002 61 ""
}
$sequence "knockedoutidle" "knockedoutidle_ani" fps 15 loop
$sequence "knockedout_wakeup" "knockedout_wakeup_ani"
$sequence "blindedstager" "blindedstager_ani" ACT_INVALID 1 LX LY loop
$sequence "electrocution" "electrocution_ani" ACT_BIG_FLINCH 1 fps 35
$sequence "inspect_body" "inspect_body_ani" fps 12
$sequence "kick_grenade" "kick_grenade_ani" fps 20 {
	event 14 9 ""
}
$sequence "surrender" "surrender_ani" ACT_FEAR_DISPLAY 1 fps 15
$sequence "flinch" "flinch_ani" ACT_FLINCH_HEAD 1 fps 28
$sequence "melee" "melee_ani" ACT_MELEE_ATTACK1 1 {
	event 3 13 ""
}
