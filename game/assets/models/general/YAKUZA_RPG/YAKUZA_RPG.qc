$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "YAKUZA_RPG.mdl"

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
	studio "yakuza_GUN_rpg_ref"
}
$bodygroup "skeleton"
{
	blank
	studio "skeleton_REFERENCE_ref"
}

$attachment "" "Bip01 R Hand" 30.400000 0.500000 3.600000
$attachment "" "Bip01 R Hand" 29.000000 0.000000 0.000000
$attachment "" "Bip01 R Hand" 45.000000 0.000000 0.000000
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
$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 15
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 15
$sequence "idlecasual1" "idlecasual1_ani" ACT_IDLE 1 fps 25
$sequence "inspect_body" "inspect_body_ani" fps 12
$sequence "signal_help" "signal_help_ani" ACT_SIGNAL1 1 fps 55
$sequence "limpingwalk" "limpingwalk_ani" ACT_WALK_HURT 1 LX loop
$sequence "limpingrun" "limpingrun_ani" ACT_RUN_HURT 1 LX fps 40 loop
$sequence "alerted_patrol" "alerted_patrol_ani" ACT_WALK_SCARED 1 LX loop
$sequence "alerted_sneak1" "alerted_sneak1_ani" ACT_WALK_SCARED 1 LX loop
$sequence "run" "run_ani" ACT_RUN 1 LX loop
$sequence "run_nogun" "run_nogun_ani" LX loop
$sequence "run_thruwall" "run_thruwall_ani"
$sequence "alerted_idle1" "alerted_idle1_ani" ACT_IDLE_ANGRY 2 fps 15
$sequence "alerted_idle2" "alerted_idle2_ani" ACT_IDLE_ANGRY 2 fps 15
$sequence "combatidle" "combatidle_ani" ACT_COMBAT_IDLE 1 fps 15
$sequence "crouching_rpg" "crouching_rpg_ani" "crouching_rpg_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 fps 10 {
	event 4 1 ""
	event 5001 1 "50"
}
$sequence "standing_rpg" "standing_rpg_ani" "standing_rpg_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 fps 10 {
	event 4 1 ""
	event 5001 1 "50"
}
$sequence "reload_rpg" "reload_rpg_ani" ACT_RELOAD 1 fps 15 {
	event 2 68 ""
}
$sequence "crouching_idle" "crouching_idle_ani" fps 15
$sequence "crouching_wait" "crouching_wait_ani" ACT_CROUCH 1 fps 15
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
	event 2001 61 ""
	event 2002 61 ""
}
$sequence "knockedout_wakeup" "knockedout_wakeup_ani" fps 20
$sequence "knockedoutidle" "knockedoutidle_ani" fps 15 loop
$sequence "knockedout2" "knockedout2_ani" ACT_SLEEP 1 {
	event 2001 109 ""
	event 2002 109 ""
}
$sequence "knockedout2idle" "knockedout2idle_ani" fps 15
$sequence "dieback1" "dieback1_ani" ACT_INVALID 1 fps 35 {
	event 11 5 ""
	event 2002 42 ""
}
$sequence "dieforward" "dieforward_ani" ACT_DIE_GUTSHOT 1 fps 35 {
	event 11 13 ""
	event 2002 31 ""
}
$sequence "diesimple" "diesimple_ani" ACT_DIESIMPLE 1 fps 28 {
	event 11 0 ""
	event 2002 35 ""
}
$sequence "diebackwards" "diebackwards_ani" ACT_DIEBACKWARD 1 fps 35 {
	event 11 6 ""
	event 2002 44 ""
}
$sequence "dieheadshot" "dieheadshot_ani" ACT_DIE_HEADSHOT 1 fps 25 {
	event 11 1 ""
	event 2002 45 ""
}
$sequence "diegutshot" "diegutshot_ani" ACT_DIE_GUTSHOT 1 {
	event 11 10 ""
	event 2002 60 ""
}
$sequence "dienutshot" "dienutshot_ani" ACT_INVALID 1 {
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
