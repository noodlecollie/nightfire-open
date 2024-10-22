########################################
# QC for: w_ronin.mdl
# Decompiled with mdlv14decompiler
#
# 1 body groups
# 1 attachments
# 2 bone controllers
# 6 hit boxes
########################################

$cd .
$cdtexture "../../../../../content/nfopen/textures"
$modelname "w_ronin.mdl"

$root "Dummy01"
$noembeddedtextures
$texrendermode mdl/w_ronin.png masked

$flags 0

# For some reason the Ronin model is oriented on local X,
# when in the engine the conversion to world space means
# the gun points along world Y. In a cool addition to
# $origin that I was not aware of, you can provide a Z
# rotation as a fourth argument.
$origin 0 0 0 -90

$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000

$bodygroup "studio"
{
	studio "w_ronin_reference_ref"
}

$texturegroup activityskin
{
	{ "mdl/w_ronin.png" "mdl/ronin_light_red.png" }
	{ "mdl/w_ronin.png" "mdl/ronin_light_off.png" }
}

$attachment "" "barrel" 0.000000 1.000000 9.300000

$controller 0 "gun_yaw" LZR 0 360
$controller 1 "gun_pitch" YR 0 30

$hbox 0 "case_01" -12.820000 -1.450000 0.000000 12.870000 1.353852 21.260000
$hbox 0 "gun_yaw" -2.980000 -2.830000 0.000000 2.970000 2.830000 19.510000
$hbox 0 "gun_pitch" -10.820000 -2.480000 -3.380000 10.360000 2.480000 7.430000
$hbox 0 "barrel" -1.840000 -1.590000 0.000000 1.840000 1.590000 10.830000
$hbox 0 "case_02" -12.470000 -0.010000 0.000000 12.470000 2.180000 20.700001
$hbox 0 "case_03" -12.470000 -2.180000 0.000000 12.470000 0.010000 20.700001

$sequence "idle1" "idle1_ani" fps 30
$sequence "spin" "spin_ani" fps 30
$sequence "deploy" "deploy_ani" fps 30
$sequence "fire" "fire_ani" fps 30
$sequence "deploy_idle" "deploy_idle_ani" fps 30
