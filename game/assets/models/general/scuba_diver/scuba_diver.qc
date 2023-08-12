$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "scuba_diver.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 70.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "scuba_diver_ref"
	blank
}
$bodygroup "heads"
{
	studio "scuba_diver_head_1_ref"
	studio "scuba_diver_head_2_ref"
	blank
}
$bodygroup "accessories"
{
	blank
}
$bodygroup "weapons"
{
	blank
	studio "scuba_diver_gun_reference_ref"
}
$bodygroup "skeleton"
{
	blank
}

$attachment "" "Bip01 R Hand" 16.000000 -3.000000 4.000000
$attachment "" "Bip01 R Hand" 16.000000 -3.000000 4.000000
$attachment "" "Bip01 R Hand" 26.000000 -3.000000 4.000000
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

$sequence "swim_idle" "swim_idle_ani" ACT_IDLE 1 LX LY fps 25 loop
$sequence "swim_idle2" "swim_idle2_ani" ACT_IDLE 1 LX LY fps 25 loop
$sequence "swim_idle3" "swim_idle3_ani" ACT_IDLE 1 LX LY fps 25 loop
$sequence "swim_idle4" "swim_idle4_ani" ACT_IDLE 1 LX LY fps 25 loop
$sequence "swim_idle5" "swim_idle5_ani" ACT_IDLE 1 LX LY fps 25 loop
$sequence "swim_slow" "swim_slow_ani" ACT_WALK 1 LX LY fps 25 loop
$sequence "swim_fast" "swim_fast_ani" ACT_RUN 1 LX LY fps 40 loop
$sequence "fire(backwards)" "fire(backwards)_ani" ACT_RANGE_ATTACK1 1 LX LY loop {
	event 4 1 ""
	event 5 15 ""
}
$sequence "fire(forwards)" "fire(forwards)_ani" ACT_RANGE_ATTACK1 1 LX LY loop {
	event 4 2 ""
	event 5 17 ""
}
$sequence "tred_R_fire" "tred_R_fire_ani" ACT_RANGE_ATTACK2 1 LX LY fps 25 loop {
	event 4 5 ""
	event 5 18 ""
}
$sequence "tred_L_fire" "tred_L_fire_ani" ACT_RANGE_ATTACK2 1 LX LY fps 25 loop {
	event 4 5 ""
	event 5 20 ""
}
$sequence "reload" "reload_ani" ACT_RELOAD 1 LX LY
$sequence "death1" "death1_ani" ACT_DIESIMPLE 1 LX LY
$sequence "death2" "death2_ani" ACT_DIESIMPLE 1 LX LY
$sequence "death_falling_loop" "death_falling_loop_ani" fps 15
$sequence "scuba1_idle" "scuba1_idle_ani" fps 15
$sequence "scuba2_idle" "scuba2_idle_ani" fps 15
$sequence "scuba1_swim" "scuba1_swim_ani" fps 15
$sequence "scuba2_swim" "scuba2_swim_ani" fps 15
$sequence "scuba1_working" "scuba1_working_ani" fps 15
$sequence "scuba2_working" "scuba2_working_ani" fps 15
