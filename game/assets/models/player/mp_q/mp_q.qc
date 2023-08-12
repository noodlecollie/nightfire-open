cd "."
$cdtexture "../../../../afterburner/textures"
$modelname "mp_q.mdl"

$root "Bip01"

$flags 0
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "q_low_ref"
}

$attachment "" "Bip01 R Hand" 34.700000 3.000000 1.800000
$attachment "" "Bip01 R Hand" 28.000000 -1.500000 6.000000
$attachment "" "Bip01 R Hand" 47.000000 2.900000 2.000000
$attachment "" "Bip01 R Hand" 9.500000 1.000000 4.100000
$attachment "" "Bip01 R Hand" 13.500000 0.900000 4.200000

$controller 0 "Bip01 Spine" XR -30 30
$controller 1 "Bip01 Spine1" XR -30 30
$controller 2 "Bip01 Spine2" XR -30 30
$controller 3 "Bip01 Spine3" XR -30 30

$hbox 0 "Bip01 Head" 0.000000 -4.000000 -4.000000 9.000000 5.000000 4.000000
$hbox 1 "Bip01 Neck" -2.000000 -3.000000 -3.000000 4.000000 3.000000 3.000000
$hbox 2 "Bip01 Spine2" -2.000000 0.000000 -6.000000 10.000000 6.000000 7.000000
$hbox 3 "Bip01 Spine2" -2.000000 -6.000000 -7.000000 10.000000 0.000000 7.000000
$hbox 4 "Bip01 Spine" -4.000000 0.000000 -7.000000 6.000000 6.000000 7.000000
$hbox 3 "Bip01 Spine" -6.000000 -6.000000 -7.000000 6.000000 0.000000 7.000000
$hbox 5 "Bip01 Pelvis" -5.000000 1.000000 -3.000000 1.000000 7.000000 3.000000
$hbox 14 "Bip01 L Thigh" 0.000000 -4.000000 -3.000000 20.000000 3.000000 3.000000
$hbox 15 "Bip01 L Calf" 2.000000 -4.000000 -3.000000 20.000000 3.000000 3.000000
$hbox 15 "Bip01 L Foot" 1.000000 -4.000000 -2.000000 4.000000 8.000000 2.000000
$hbox 12 "Bip01 R Thigh" 0.000000 -4.000000 -3.000000 20.000000 3.000000 3.000000
$hbox 13 "Bip01 R Calf" 2.000000 -4.000000 -3.000000 20.000000 3.000000 3.000000
$hbox 13 "Bip01 R Foot" 1.000000 -4.000000 -2.000000 4.000000 8.000000 2.000000
$hbox 9 "Bip01 L UpperArm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 10 "Bip01 L Forearm" 0.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 11 "Bip01 L Hand" 0.000000 -1.000000 -2.000000 6.000000 2.000000 2.000000
$hbox 6 "Bip01 R UpperArm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 7 "Bip01 R Forearm" 0.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 8 "Bip01 R Hand" 0.000000 -1.000000 -2.000000 6.000000 2.000000 2.000000

$sequence "look_idle" "look_idle_ani" ACT_IDLE 2 fps 15 loop
$sequence "idle" "idle_ani" ACT_IDLE 5 fps 15 loop
$sequence "deep_idle" "deep_idle_ani" ACT_IDLE 1 fps 15 loop
$sequence "run2" "run2_ani" ACT_RUN 1 LX loop
$sequence "walk2handed" "walk2handed_ani" ACT_WALK 1 LX fps 28 loop
$sequence "2handshoot" "2handshoot_ani" ACT_RANGE_ATTACK1 1 fps 20
$sequence "crawl" "crawl_ani" ACT_CROUCH 1 LX fps 35 loop
$sequence "crouch_idle" "crouch_idle_ani" ACT_CROUCHIDLE 1 fps 15 loop
$sequence "jump" "jump_ani" ACT_HOP 1
$sequence "long_jump" "long_jump_ani" ACT_LEAP 1 LX fps 24
$sequence "swim" "swim_ani" ACT_SWIM 1 loop
$sequence "treadwater" "treadwater_ani" ACT_HOVER 1 fps 20 loop
$sequence "run" "run_ani" LX loop
$sequence "walk" "walk_ani" LX loop
$sequence "aim_2" "aim_2_ani" "aim_2_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "shoot_2" "shoot_2_ani" "shoot_2_blend1_ani" blend XR -45 45 loop
$sequence "aim_1" "aim_1_ani" "aim_1_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "shoot_1" "shoot_1_ani" "shoot_1_blend1_ani" blend XR -45 45 loop
$sequence "die_simple" "die_simple_ani" ACT_DIESIMPLE 1 fps 35 {
	event 2001 7 ""
}
$sequence "die_backwards1" "die_backwards1_ani" ACT_DIEBACKWARD 1 fps 40 {
	event 2001 14 ""
}
$sequence "die_backwards" "die_backwards_ani" ACT_DIEBACKWARD 1 fps 27 {
	event 2001 15 ""
}
$sequence "die_forwards" "die_forwards_ani" ACT_DIEFORWARD 1 fps 38 {
	event 2001 9 ""
}
$sequence "die_headshot" "die_headshot_ani" ACT_DIE_HEADSHOT 4 {
	event 2001 22 ""
}
$sequence "die_slow" "die_slow_ani" ACT_DIE_HEADSHOT 1 fps 43 {
	event 2001 16 ""
}
$sequence "die_gutshot" "die_gutshot_ani" ACT_DIE_GUTSHOT 4 {
	event 2001 23 ""
}
$sequence "ref_aim_grenade" "ref_aim_grenade_ani" "ref_aim_grenade_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_grenade" "ref_shoot_grenade_ani" "ref_shoot_grenade_blend1_ani" blend XR -45 45 fps 25
$sequence "crouch_aim_grenade" "crouch_aim_grenade_ani" "crouch_aim_grenade_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "crouch_shoot_grenade" "crouch_shoot_grenade_ani" "crouch_shoot_grenade_blend1_ani" blend XR -45 45 fps 25
$sequence "ref_aim_trip" "ref_aim_trip_ani" "ref_aim_trip_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_trip" "ref_shoot_trip_ani" "ref_shoot_trip_blend1_ani" blend XR -45 45 fps 20
$sequence "crouch_aim_trip" "crouch_aim_trip_ani" "crouch_aim_trip_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "crouch_shoot_trip" "crouch_shoot_trip_ani" "crouch_shoot_trip_blend1_ani" blend XR -45 45 fps 20
$sequence "ref_aim_pistol" "ref_aim_pistol_ani" "ref_aim_pistol_blend1_ani" blend XR -50 35 loop
$sequence "ref_shoot_pistol" "ref_shoot_pistol_ani" "ref_shoot_pistol_blend1_ani" blend XR -50 35 {
	event 5031 0 "22"
}
$sequence "crouch_aim_pistol" "crouch_aim_pistol_ani" "crouch_aim_pistol_blend1_ani" blend XR -50 35 fps 15 loop
$sequence "crouch_shoot_pistol" "crouch_shoot_pistol_ani" "crouch_shoot_pistol_blend1_ani" blend XR -50 35 {
	event 5011 0 "22"
}
$sequence "ref_aim_grapple" "ref_aim_grapple_ani" "ref_aim_grapple_blend1_ani" blend XR -50 35 fps 15 loop
$sequence "ref_shoot_grapple" "ref_shoot_grapple_ani" "ref_shoot_grapple_blend1_ani" blend XR -50 35
$sequence "crouch_aim_grapple" "crouch_aim_grapple_ani" "crouch_aim_grapple_blend1_ani" blend XR -50 35 fps 15 loop
$sequence "crouch_shoot_grapple" "crouch_shoot_grapple_ani" "crouch_shoot_grapple_blend1_ani" blend XR -50 35 {
	event 5011 0 "31"
}
$sequence "ref_aim_shotgun" "ref_aim_shotgun_ani" "ref_aim_shotgun_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_shotgun" "ref_shoot_shotgun_ani" "ref_shoot_shotgun_blend1_ani" blend XR -45 45 fps 50 loop {
	event 5001 0 "33"
}
$sequence "crouch_aim_shotgun" "crouch_aim_shotgun_ani" "crouch_aim_shotgun_blend1_ani" blend XR -30 40 fps 15 loop
$sequence "crouch_shoot_shotgun" "crouch_shoot_shotgun_ani" "crouch_shoot_shotgun_blend1_ani" blend XR -30 40 fps 50 loop {
	event 5001 0 "33"
}
$sequence "ref_aim_sniper_rifle" "ref_aim_sniper_rifle_ani" "ref_aim_sniper_rifle_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_sniper_rifle" "ref_shoot_sniper_rifle_ani" "ref_shoot_sniper_rifle_blend1_ani" blend XR -45 45 fps 20 {
	event 5021 0 "22"
}
$sequence "crouch_aim_sniper_rifle" "crouch_aim_sniper_rifle_ani" "crouch_aim_sniper_rifle_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "crouch_shoot_sniper_rifle" "crouch_shoot_sniper_rifle_ani" "crouch_shoot_sniper_rifle_blend1_ani" blend XR -45 45 fps 20 {
	event 5021 0 "22"
}
$sequence "ref_aim_sig552" "ref_aim_sig552_ani" "ref_aim_sig552_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_sig552" "ref_shoot_sig552_ani" "ref_shoot_sig552_blend1_ani" blend XR -45 45 fps 50 loop {
	event 5001 0 "50"
}
$sequence "crouch_aim_sig552" "crouch_aim_sig552_ani" "crouch_aim_sig552_blend1_ani" blend XR -30 40 fps 15 loop
$sequence "crouch_shoot_sig552" "crouch_shoot_sig552_ani" "crouch_shoot_sig552_blend1_ani" blend XR -30 40 fps 50 loop {
	event 5001 0 "50"
}
$sequence "ref_aim_rpg" "ref_aim_rpg_ani" "ref_aim_rpg_blend1_ani" blend XR -45 40 fps 15 loop
$sequence "ref_shoot_rpg" "ref_shoot_rpg_ani" "ref_shoot_rpg_blend1_ani" blend XR -45 40 fps 18
$sequence "crouch_aim_rpg" "crouch_aim_rpg_ani" "crouch_aim_rpg_blend1_ani" blend XR -45 40 fps 15 loop
$sequence "crouch_shoot_rpg" "crouch_shoot_rpg_ani" "crouch_shoot_rpg_blend1_ani" blend XR -45 40 fps 18
$sequence "ref_aim_mp9" "ref_aim_mp9_ani" "ref_aim_mp9_blend1_ani" blend XR -45 35 fps 15 loop
$sequence "ref_shoot_mp9" "ref_shoot_mp9_ani" "ref_shoot_mp9_blend1_ani" blend XR -45 35 fps 55 loop {
	event 5031 0 "22"
}
$sequence "crouch_aim_mp9" "crouch_aim_mp9_ani" "crouch_aim_mp9_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "crouch_shoot_mp9" "crouch_shoot_mp9_ani" "crouch_shoot_mp9_blend1_ani" blend XR -45 45 fps 55 loop {
	event 5031 0 "22"
}
$sequence "ref_aim_pdw90" "ref_aim_pdw90_ani" "ref_aim_pdw90_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_pdw90" "ref_shoot_pdw90_ani" "ref_shoot_pdw90_blend1_ani" blend XR -45 45 fps 50 {
	event 5041 0 "32"
}
$sequence "crouch_aim_pdw90" "crouch_aim_pdw90_ani" "crouch_aim_pdw90_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "crouch_shoot_pdw90" "crouch_shoot_pdw90_ani" "crouch_shoot_pdw90_blend1_ani" blend XR -45 45 fps 50 {
	event 5041 0 "32"
}
$sequence "ref_aim_grenadelauncher" "ref_aim_grenadelauncher_ani" "ref_aim_grenadelauncher_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_grenadelauncher" "ref_shoot_grenadelauncher_ani" "ref_shoot_grenadelauncher_blend1_ani" blend XR -45 45 fps 20 {
	event 5011 0 "32"
}
$sequence "crouch_aim_grenadelauncher" "crouch_aim_grenadelauncher_ani" "crouch_aim_grenadelauncher_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "crouch_shoot_grenadelauncher" "crouch_shoot_grenadelauncher_ani" "crouch_shoot_grenadelauncher_blend1_ani" blend XR -45 45 fps 20 {
	event 5011 0 "32"
}
$sequence "ref_aim_dukes" "ref_aim_dukes_ani" "ref_aim_dukes_blend1_ani" blend XR -45 45 fps 12 loop
$sequence "ref_shoot_dukes" "ref_shoot_dukes_ani" "ref_shoot_dukes_blend1_ani" blend XR -45 45
$sequence "crouch_aim_dukes" "crouch_aim_dukes_ani" "crouch_aim_dukes_blend1_ani" blend XR -45 45 fps 12 loop
$sequence "crouch_shoot_dukes" "crouch_shoot_dukes_ani" "crouch_shoot_dukes_blend1_ani" blend XR -45 45
$sequence "ref_aim_dartpen" "ref_aim_dartpen_ani" "ref_aim_dartpen_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_dartpen" "ref_shoot_dartpen_ani" "ref_shoot_dartpen_blend1_ani" blend XR -45 45
$sequence "crouch_aim_dartpen" "crouch_aim_dartpen_ani" "crouch_aim_dartpen_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "crouch_shoot_dartpen" "crouch_shoot_dartpen_ani" "crouch_shoot_dartpen_blend1_ani" blend XR -45 45
$sequence "ref_aim_ronin" "ref_aim_ronin_ani" "ref_aim_ronin_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_ronin" "ref_shoot_ronin_ani" "ref_shoot_ronin_blend1_ani" blend XR -45 45 fps 15
$sequence "crouch_aim_ronin" "crouch_aim_ronin_ani" "crouch_aim_ronin_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "crouch_shoot_ronin" "crouch_shoot_ronin_ani" "crouch_shoot_ronin_blend1_ani" blend XR -45 45 fps 15
$sequence "ref_aim_detonator" "ref_aim_detonator_ani" "ref_aim_detonator_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_detonator" "ref_shoot_detonator_ani" "ref_shoot_detonator_blend1_ani" blend XR -45 45 fps 15
$sequence "crouch_aim_detonator" "crouch_aim_detonator_ani" "crouch_aim_detonator_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "crouch_shoot_detonator" "crouch_shoot_detonator_ani" "crouch_shoot_detonator_blend1_ani" blend XR -45 45 fps 15
$sequence "ref_aim_mini" "ref_aim_mini_ani" "ref_aim_mini_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_mini" "ref_shoot_mini_ani" "ref_shoot_mini_blend1_ani" blend XR -45 45 {
	event 5011 0 "32"
}
$sequence "crouch_aim_mini" "crouch_aim_mini_ani" "crouch_aim_mini_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "crouch_shoot_mini" "crouch_shoot_mini_ani" "crouch_shoot_mini_blend1_ani" blend XR -45 45 {
	event 5011 0 "32"
}
$sequence "ref_aim_laser" "ref_aim_laser_ani" "ref_aim_laser_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_laser" "ref_shoot_laser_ani" "ref_shoot_laser_blend1_ani" blend XR -45 45
$sequence "crouch_aim_laser" "crouch_aim_laser_ani" "crouch_aim_laser_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "crouch_shoot_laser" "crouch_shoot_laser_ani" "crouch_shoot_laser_blend1_ani" blend XR -45 45
$sequence "ref_aim_hat" "ref_aim_hat_ani" "ref_aim_hat_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "ref_shoot_hat" "ref_shoot_hat_ani" "ref_shoot_hat_blend1_ani" blend XR -45 45
$sequence "crouch_aim_hat" "crouch_aim_hat_ani" "crouch_aim_hat_blend1_ani" blend XR -45 45 fps 15 loop
$sequence "crouch_shoot_hat" "crouch_shoot_hat_ani" "crouch_shoot_hat_blend1_ani" blend XR -45 45
