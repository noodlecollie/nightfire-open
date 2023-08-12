$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "fire_hose.mdl"

$root "default01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "body"
{
	studio "fire_hose_ref"
}



$hbox 0 "default01" -4.940000 -4.710000 -12.490000 4.940000 4.710000 12.490000
$hbox 0 "Cylinder01" -4.570000 -9.470000 0.000000 4.570000 9.470000 3.000000

$sequence "idle1" "idle1_ani"
