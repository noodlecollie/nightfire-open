$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "drake_ss.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "drake_ss_reference_ref"
	blank
}
$bodygroup "heads"
{
	blank
}
$bodygroup "accessories"
{
	blank
	studio "drake_ss_glass_reference_ref"
}
$bodygroup "weapons"
{
	studio "drake_ss_laser_rifle_reference_ref"
}
$bodygroup "skeleton"
{
	blank
	blank
}

$attachment "" "Bip01 R Hand" 24.000000 -7.000000 4.000000
$attachment "" "Bip01 R Hand" 24.000000 -7.000000 4.000000
$attachment "" "Bip01 R Hand" 36.000000 -12.000000 4.000000
$attachment "" "Bip01" 0.000000 0.000000 48.000000
$attachment "" "Bip01 Head" 5.000000 0.000000 0.000000
$attachment "" "Bip01 Head" 5.000000 30.000000 0.000000
$attachment "" "Bip01 Spine3" 0.000000 -12.000000 0.000000
$attachment "" "Bip01 L Hand" 6.700000 0.000000 -9.000000

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

$sequence "walk1" "walk1_ani" ACT_WALK 1 LX fps 20 loop
$sequence "walkcasual" "walkcasual_ani" ACT_WALK 1 LX fps 20 loop
$sequence "turn_L" "turn_L_ani" ACT_TURN_LEFT 1
$sequence "turn_R" "turn_R_ani" ACT_TURN_RIGHT 1
$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 15
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 20
$sequence "idle3" "idle3_ani" ACT_IDLE 1 fps 15
$sequence "limpingwalk" "limpingwalk_ani" ACT_WALK_HURT 1 LX fps 20 loop
$sequence "limpingrun" "limpingrun_ani" ACT_RUN_HURT 1 LX fps 20 loop
$sequence "airback" "airback_ani" fps 15
$sequence "airdown" "airdown_ani" fps 15
$sequence "airforward" "airforward_ani" fps 15
$sequence "airup" "airup_ani" fps 15
$sequence "airleft" "airleft_ani" fps 15
$sequence "airright" "airright_ani" fps 15
$sequence "airdodgeback" "airdodgeback_ani"
$sequence "airdodgedown" "airdodgedown_ani"
$sequence "airdodgeforward" "airdodgeforward_ani"
$sequence "airdodgeup" "airdodgeup_ani"
$sequence "airdodgeleft" "airdodgeleft_ani"
$sequence "airdodgeright" "airdodgeright_ani"
$sequence "airchargeloop" "airchargeloop_ani" loop
$sequence "airchargestart" "airchargestart_ani"
$sequence "airchargeend" "airchargeend_ani"
$sequence "airbodyhit" "airbodyhit_ani"
$sequence "airgrenadedown" "airgrenadedown_ani" {
	event 8 0 ""
	event 9 11 ""
}
$sequence "airgrenademid" "airgrenademid_ani" {
	event 8 3 ""
	event 9 14 ""
}
$sequence "airgrenadeup" "airgrenadeup_ani" {
	event 8 4 ""
	event 9 15 ""
}
$sequence "airgroinhit" "airgroinhit_ani"
$sequence "airheadhit" "airheadhit_ani"
$sequence "airland" "airland_ani"
$sequence "airloop" "airloop_ani" loop
$sequence "airshootdown" "airshootdown_ani" {
	event 4 16 ""
}
$sequence "airshootmid" "airshootmid_ani" {
	event 4 16 ""
}
$sequence "airshootup" "airshootup_ani" {
	event 4 16 ""
}
$sequence "airspraydown" "airspraydown_ani" {
	event 4 16 ""
	event 5001 16 "34"
	event 5 20 ""
	event 5001 20 "34"
	event 4 25 ""
	event 5001 25 "34"
	event 4 31 ""
	event 5001 31 "34"
	event 5 36 ""
	event 5001 36 "34"
	event 6 42 ""
	event 5001 42 "34"
	event 4 49 ""
	event 5001 49 "34"
}
$sequence "airspraymid" "airspraymid_ani" {
	event 4 16 ""
	event 5001 16 "34"
	event 5 20 ""
	event 5001 20 "34"
	event 4 25 ""
	event 5001 25 "34"
	event 4 31 ""
	event 5001 31 "34"
	event 5 36 ""
	event 5001 36 "34"
	event 6 42 ""
	event 5001 42 "34"
	event 4 49 ""
	event 5001 49 "34"
}
$sequence "airsprayup" "airsprayup_ani" {
	event 4 16 ""
	event 5001 16 "34"
	event 5 20 ""
	event 5001 20 "34"
	event 4 25 ""
	event 5001 25 "34"
	event 4 31 ""
	event 5001 31 "34"
	event 5 36 ""
	event 5001 36 "34"
	event 6 42 ""
	event 5001 42 "34"
	event 4 49 ""
	event 5001 49 "34"
}
$sequence "airtakeoff" "airtakeoff_ani"
$sequence "airtorch" "airtorch_ani" {
	event 3 10 ""
	event 3 25 ""
}
$sequence "airturnl" "airturnl_ani" fps 15
$sequence "airturnr" "airturnr_ani" fps 15
$sequence "flinchhead" "flinchhead_ani" ACT_FLINCH_HEAD 1
$sequence "flinchback" "flinchback_ani" ACT_INVALID 1
$sequence "flinchlarmhand" "flinchlarmhand_ani" ACT_INVALID 1
$sequence "flinchlarmupper" "flinchlarmupper_ani" ACT_FLINCH_RIGHTARM 1
$sequence "flinchlleglower" "flinchlleglower_ani" ACT_INVALID 1
$sequence "flinchllegupper" "flinchllegupper_ani" ACT_FLINCH_RIGHTLEG 1
$sequence "flinchrarmhand" "flinchrarmhand_ani" ACT_INVALID 1
$sequence "flinchrarmupper" "flinchrarmupper_ani" ACT_FLINCH_LEFTLEG 1
$sequence "flinchrleglower" "flinchrleglower_ani" ACT_INVALID 1
$sequence "flinchrlegupper" "flinchrlegupper_ani" ACT_INVALID 1
$sequence "flinchstomach" "flinchstomach_ani" ACT_FLINCH_STOMACH 1
$sequence "grenadedown" "grenadedown_ani" ACT_RANGE_ATTACK2 1 {
	event 8 0 ""
	event 9 16 ""
}
$sequence "grenadedown" "grenadedown_ani" ACT_RANGE_ATTACK2 1 {
	event 8 0 ""
	event 9 16 ""
}
$sequence "grenadeup" "grenadeup_ani" ACT_RANGE_ATTACK2 1 {
	event 8 0 ""
	event 9 16 ""
}
$sequence "run" "run_ani"
$sequence "runfire" "runfire_ani" ACT_RANGE_ATTACK1 1
$sequence "shootdown" "shootdown_ani" ACT_RANGE_ATTACK1 1 {
	event 4 1 ""
	event 5001 1 "34"
}
$sequence "shootmid" "shootmid_ani" ACT_RANGE_ATTACK1 1 {
	event 4 1 ""
	event 5001 1 "34"
}
$sequence "shootup" "shootup_ani" ACT_RANGE_ATTACK1 1 {
	event 4 1 ""
	event 5001 1 "34"
}
$sequence "turnl" "turnl_ani"
$sequence "turnr" "turnr_ani"
$sequence "airdeathimpact" "airdeathimpact_ani"
$sequence "airdeathland" "airdeathland_ani"
$sequence "airdeathloop" "airdeathloop_ani"
$sequence "airdeathstart" "airdeathstart_ani"
$sequence "run" "run_ani" ACT_RUN 1 LX fps 35 loop
$sequence "reload_laser" "reload_laser_ani" ACT_RELOAD 1 fps 20 {
	event 2 55 ""
}
$sequence "frontkick" "frontkick_ani" ACT_MELEE_ATTACK1 1 fps 28 {
	event 7 8 ""
}
$sequence "run2" "run2_ani" ACT_RUN 1 LX fps 25 loop
$sequence "jump" "jump_ani" LX
$sequence "run_jump" "run_jump_ani"
