$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "secret_documents.mdl"

$root "Bone01"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "generator"
{
	studio "secret_documents_01_reference_ref"
	blank
}



$hbox 0 "Bone01" -10.000000 -10.000000 5.000000 10.000000 10.000000 0.000000

$sequence "idle1" "idle1_ani"
$sequence "open" "open_ani"
$sequence "idle_open" "idle_open_ani"
