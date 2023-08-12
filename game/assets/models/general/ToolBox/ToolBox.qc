$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "ToolBox.mdl"

$root "ToolBox_Box"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "ToolBox_ref"
}



$hbox 0 "ToolBox_Box" -9.020000 -3.910000 0.000000 9.020000 3.910000 9.860000
$hbox 0 "ToolBox_Clasp" -1.120000 -0.150000 0.000000 1.120000 0.890000 3.650000

$sequence "idle1" "idle1_ani"
