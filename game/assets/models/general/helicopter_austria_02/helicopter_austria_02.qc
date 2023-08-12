$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "helicopter_austria_02.mdl"

$root "Master"

$flags 2560
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "helicopter"
{
	studio "helicopter2_body_reference_ref"
	blank
}
$bodygroup "enemy"
{
	studio "helicopter2_enemy_lod0_reference_ref"
	studio "helicopter2_enemy_lod1_reference_ref"
	studio "helicopter2_enemy_lod2_reference_ref"
	studio "helicopter2_enemy_lod3_reference_ref"
	studio "helicopter2_enemy_lod4_reference_ref"
	blank
}
$bodygroup "guns"
{
	studio "helicopter2_mgun_reference_ref"
	studio "helicopter2_stinger_reference_ref"
	blank
}
$bodygroup "crew"
{
	studio "helicopter2_crew_1_reference_ref"
	studio "helicopter2_crew_2_reference_ref"
	studio "helicopter2_crew_3_reference_ref"
	studio "helicopter2_crew_4_reference_ref"
	blank
}
$bodygroup "glass"
{
	studio "helicopter2_glass_reference_ref"
	blank
}

$attachment "" "W_Gun" 30.400000 0.000000 0.000000
$attachment "" "Master" 0.000000 20.000000 0.000000

$controller 0 "blade_rotator" 32784 0 360
$controller 0 "Dummy12" 32800 0 360
$controller 1 "Origin" 32784 0 360

$hbox 0 "Master" -100.000000 -50.000000 -50.000000 -40.000000 60.000000 50.000000
$hbox 0 "Master" -130.000000 -50.000000 -50.000000 -100.000000 30.000000 50.000000
$hbox 0 "Master" -150.000000 -50.000000 -45.000000 -130.000000 10.000000 45.000000
$hbox 0 "Master" -190.000000 -45.000000 -40.000000 -150.000000 -10.000000 40.000000
$hbox 0 "Master" -40.000000 -50.000000 -50.000000 30.000000 -41.000000 50.000000
$hbox 0 "Master" -40.000000 50.000000 -50.000000 30.000000 80.000000 50.000000
$hbox 0 "Master" 30.000000 -50.000000 -50.000000 70.000000 80.000000 50.000000
$hbox 0 "Master" 70.000000 -50.000000 -50.000000 110.000000 60.000000 50.000000
$hbox 0 "Master" 110.000000 -40.000000 -45.000000 160.000000 40.000000 45.000000
$hbox 0 "Master" 160.000000 -25.000000 -30.000000 390.000000 30.000000 30.000000
$hbox 0 "Master" 300.000000 30.000000 -10.000000 370.000000 70.000000 10.000000
$hbox 0 "Master" 320.000000 70.000000 -10.000000 385.000000 125.000000 10.000000
$hbox 0 "L_Bdoor" -7.090000 -41.970000 -31.510000 4.760000 39.750000 33.950000
$hbox 0 "R_Bdoor" -4.520000 -41.790000 -31.520000 2.340000 39.930000 33.940000
$hbox 1 "Bip01 Head" 0.000000 -4.000000 -4.000000 9.000000 5.000000 4.000000
$hbox 1 "Bip01 Neck" -2.000000 -3.000000 -3.000000 4.000000 3.000000 3.000000
$hbox 1 "Bip01 Spine2" -2.000000 0.000000 -6.000000 10.000000 6.000000 7.000000
$hbox 1 "Bip01 Spine2" -2.000000 -6.000000 -7.000000 10.000000 0.000000 7.000000
$hbox 1 "Bip01 Spine" -4.000000 0.000000 -7.000000 6.000000 6.000000 7.000000
$hbox 1 "Bip01 Spine" -6.000000 -6.000000 -7.000000 6.000000 0.000000 7.000000
$hbox 1 "Bip01 Pelvis" -5.000000 1.000000 -3.000000 1.000000 7.000000 3.000000
$hbox 1 "Bip01 L Thigh" 0.000000 -4.000000 -3.000000 20.000000 3.000000 3.000000
$hbox 1 "Bip01 L Calf" 0.000000 -4.000000 -3.000000 17.000000 3.000000 3.000000
$hbox 1 "Bip01 L Foot" 1.000000 -4.000000 -2.000000 4.000000 8.000000 2.000000
$hbox 1 "Bip01 R Thigh" 0.000000 -4.000000 -3.000000 20.000000 3.000000 3.000000
$hbox 1 "Bip01 R Calf" 0.000000 -4.000000 -3.000000 17.000000 3.000000 3.000000
$hbox 1 "Bip01 R Foot" 1.000000 -4.000000 -2.000000 4.000000 8.000000 2.000000
$hbox 1 "Bip01 L UpperArm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 1 "Bip01 L Forearm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 1 "Bip01 L Hand" 0.000000 -1.000000 -2.000000 6.000000 2.000000 2.000000
$hbox 1 "Bip01 R UpperArm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 1 "Bip01 R Forearm" -2.000000 -3.000000 -2.000000 11.000000 2.000000 2.000000
$hbox 1 "Bip01 R Hand" 0.000000 -1.000000 -2.000000 6.000000 2.000000 2.000000

$sequence "idle_rifle" "idle_rifle_ani"
$sequence "idle_stinger" "idle_stinger_ani"
$sequence "fire_rifle" "fire_rifle_ani"
$sequence "fire_stinger" "fire_stinger_ani"
$sequence "death1" "death1_ani" {
	event 11 1 ""
	event 5006 1 "fall"
}
$sequence "death2" "death2_ani" {
	event 11 1 ""
	event 5006 1 "fall"
}
