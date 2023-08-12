$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "rook_ss.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "rook_ss_ref"
	blank
}
$bodygroup "heads"
{
	blank
}
$bodygroup "accessories"
{
	blank
	studio "rook_ss_visor_ref"
}
$bodygroup "weapons"
{
	blank
	studio "rook_ss_laser_rifle_reference_ref"
}
$bodygroup "skeleton"
{
	blank
}

$attachment "" "Bip01 R Hand" 24.000000 -7.000000 4.000000
$attachment "" "Bip01 R Hand" 24.000000 -7.000000 4.000000
$attachment "" "Bip01 R Hand" 36.000000 -12.000000 4.000000
$attachment "" "Bip01" 0.000000 0.000000 48.000000
$attachment "" "Bip01 Head" 5.000000 0.000000 0.000000
$attachment "" "Bip01 Head" 5.000000 30.000000 0.000000
$attachment "" "Bip01 Spine3" 0.000000 -12.000000 0.000000

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

$sequence "turn_L" "turn_L_ani" ACT_TURN_LEFT 1
$sequence "turn_R" "turn_R_ani" ACT_TURN_RIGHT 1
$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 15
$sequence "run" "run_ani" ACT_RUN 1
$sequence "fire_up" "fire_up_ani" ACT_RANGE_ATTACK1 10 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "34"
}
$sequence "fire_down" "fire_down_ani" ACT_RANGE_ATTACK1 10 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "34"
}
$sequence "fire_mid" "fire_mid_ani" ACT_RANGE_ATTACK1 10 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "34"
}
$sequence "fire_fast" "fire_fast_ani" ACT_RANGE_ATTACK1 10 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "34"
}
$sequence "closeattack" "closeattack_ani" ACT_MELEE_ATTACK1 10 blend XR -45 45 {
	event 16 8 ""
	event 2010 8 ""
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
$sequence "running_fire" "running_fire_ani" ACT_RANGE_ATTACK1 1 LX LY fps 25 loop {
	event 4 8 ""
	event 5001 8 "34"
	event 5 16 ""
	event 5001 16 "34"
	event 6 24 ""
	event 5001 24 "34"
	event 4 32 ""
	event 5001 32 "34"
}
$sequence "airturnl" "airturnl_ani" fps 15
$sequence "airturnr" "airturnr_ani" fps 15
$sequence "airloop" "airloop_ani"
$sequence "airback" "airback_ani" fps 15
$sequence "airdown" "airdown_ani" fps 15
$sequence "airforward" "airforward_ani" fps 15
$sequence "airup" "airup_ani" fps 15
$sequence "airleft" "airleft_ani" fps 15
$sequence "airright" "airright_ani" fps 15
$sequence "airbackloop" "airbackloop_ani" fps 15
$sequence "airdownloop" "airdownloop_ani" fps 15
$sequence "airforwardloop" "airforwardloop_ani" fps 15
$sequence "airuploop" "airuploop_ani" fps 15
$sequence "airleftloop" "airleftloop_ani" fps 15
$sequence "airrightloop" "airrightloop_ani" fps 15
$sequence "airdodgeback" "airdodgeback_ani"
$sequence "airdodgedown" "airdodgedown_ani"
$sequence "airdodgeforward" "airdodgeforward_ani"
$sequence "airdodgeup" "airdodgeup_ani"
$sequence "airdodgeleft" "airdodgeleft_ani"
$sequence "airdodgeright" "airdodgeright_ani"
$sequence "airtakeoff" "airtakeoff_ani"
$sequence "airland" "airland_ani"
$sequence "airspraydown" "airspraydown_ani" {
	event 4 15 ""
	event 5001 15 "34"
	event 5 21 ""
	event 5001 21 "34"
	event 6 27 ""
	event 5001 27 "34"
	event 4 34 ""
	event 5001 34 "34"
	event 4 41 ""
	event 5001 41 "34"
	event 4 48 ""
	event 5001 58 "34"
	event 4 55 ""
	event 5001 55 "34"
}
$sequence "airspraymid" "airspraymid_ani" {
	event 4 15 ""
	event 5001 15 "34"
	event 5 21 ""
	event 5001 21 "34"
	event 6 27 ""
	event 5001 27 "34"
	event 4 33 ""
	event 5001 33 "34"
	event 4 39 ""
	event 5001 39 "34"
	event 4 45 ""
	event 5001 45 "34"
	event 4 51 ""
	event 5001 51 "34"
	event 4 57 ""
	event 5001 57 "34"
	event 4 63 ""
	event 5001 63 "34"
}
$sequence "airsprayup" "airsprayup_ani" {
	event 4 15 ""
	event 5001 15 "34"
	event 5 21 ""
	event 5001 21 "34"
	event 6 27 ""
	event 5001 27 "34"
	event 4 33 ""
	event 5001 33 "34"
	event 4 39 ""
	event 5001 39 "34"
	event 4 45 ""
	event 5001 45 "34"
	event 4 51 ""
	event 5001 51 "34"
	event 4 57 ""
	event 5001 57 "34"
	event 4 63 ""
	event 5001 63 "34"
}
$sequence "airshootdown" "airshootdown_ani" {
	event 4 15 ""
	event 5001 15 "34"
}
$sequence "airshootmid" "airshootmid_ani" {
	event 4 15 ""
	event 5001 15 "34"
}
$sequence "airshootup" "airshootup_ani" {
	event 4 15 ""
	event 5001 15 "34"
}
$sequence "airshootfast" "airshootfast_ani" {
	event 4 3 ""
	event 5001 3 "34"
}
$sequence "aircloseattack" "aircloseattack_ani" {
	event 17 17 ""
}
$sequence "airchargestart" "airchargestart_ani"
$sequence "airchargeloopup" "airchargeloopup_ani" loop
$sequence "airchargeloopmid" "airchargeloopmid_ani" loop
$sequence "airchargeloopdown" "airchargeloopdown_ani" loop
$sequence "airchargeend" "airchargeend_ani" {
	event 17 8 ""
	event 2010 8 ""
}
$sequence "airbodyhit" "airbodyhit_ani"
$sequence "airgroinhit" "airgroinhit_ani"
$sequence "leftlegsmflinch" "leftlegsmflinch_ani" ACT_FLINCH_RIGHTLEG 1 fps 25
$sequence "leftlegsmflinch2" "leftlegsmflinch2_ani" ACT_INVALID 1 fps 25
$sequence "rightlegsmflinch" "rightlegsmflinch_ani" ACT_INVALID 1 fps 25
$sequence "rightlegsmflinch2" "rightlegsmflinch2_ani" ACT_INVALID 1 fps 25
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
$sequence "taunt" "taunt_ani" {
	event 18 1 ""
}
$sequence "airtaunt" "airtaunt_ani" {
	event 19 1 ""
}
