$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "Casino_Wall_Lamp.mdl"

$root "csino_SmPainting_lamp"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "Casino_Wall_Lamp_ref"
}



$hbox 0 "csino_SmPainting_lamp" -11.150000 -23.470000 -11.630000 10.450000 0.000000 3.940000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
