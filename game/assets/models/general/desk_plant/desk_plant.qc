$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "desk_plant.mdl"

$root "desk_plant"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "main"
{
	studio "desk_plant_ref"
}



$hbox 0 "desk_plant" -18.350000 -15.940000 -10.790000 12.560000 17.760000 14.110000

$sequence "idle1" "idle1_ani"
$sequence "shot" "shot_ani"
