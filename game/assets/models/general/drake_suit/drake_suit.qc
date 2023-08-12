$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "drake_suit.mdl"

$root "Bip01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "drake_suit_ref"
	blank
}
$bodygroup "heads"
{
	blank
}
$bodygroup "accessories"
{
	blank
}
$bodygroup "weapons"
{
	blank
	studio "drake_suit_raptor_reference_ref"
}
$bodygroup "skeleton"
{
	blank
	studio "drake_suit_skeleton_ref"
}

$attachment "" "Bip01 L Hand" 13.500000 3.500000 -3.600000
$attachment "" "Bip01 L Hand" 13.500000 3.500000 -3.600000
$attachment "" "Bip01 L Hand" 20.000000 3.500000 -3.600000
$attachment "" "Bip01" 0.000000 0.000000 48.000000
$attachment "" "Bip01 Head" 5.000000 0.000000 0.000000
$attachment "" "Bip01 Head" 5.000000 30.000000 0.000000


$hbox 0 "Bip01 Pelvis" -64.000000 -1.000000 -1.000000 -65.000000 1.000000 1.000000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 15
$sequence "idle2" "idle2_ani" ACT_IDLE 1 fps 15
$sequence "dramatic_point" "dramatic_point_ani"
$sequence "run" "run_ani" ACT_RUN 1 LX LY fps 25 loop
$sequence "walk" "walk_ani" ACT_WALK 1 LX LY loop
$sequence "fire" "fire_ani" "fire_blend1_ani" ACT_RANGE_ATTACK1 1 blend XR -45 45 {
	event 4 1 ""
	event 5001 1 "50"
}
$sequence "fire_run" "fire_run_ani" ACT_RUN 2 LX LY fps 25 loop {
	event 4 1 ""
	event 5001 1 "50"
	event 4 10 ""
	event 5001 10 "50"
}
$sequence "fire_run2" "fire_run2_ani" ACT_RUN 2 LX LY fps 20 loop {
	event 4 10 ""
	event 5001 10 "50"
	event 4 15 ""
	event 5001 15 "50"
}
$sequence "door_slam" "door_slam_ani" {
	event 11 16 ""
	event 1003 70 "event_door_slam"
}
