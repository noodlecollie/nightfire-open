$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "elevator_machine.mdl"

$root "base"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "studio"
{
	studio "elevator_machine_ref_ref"
}



$hbox 0 "base" -24.810000 -49.230000 -35.550000 24.620000 113.530000 35.440000
$hbox 0 "wheel" -18.060000 -18.730000 -16.680000 18.060000 18.190000 16.680000
$hbox 0 "stick 1" -2.570000 -24.730000 -11.950000 2.570000 12.530000 24.700000
$hbox 0 "stick 2" -2.570000 -19.830000 -2.800000 2.570000 16.010000 2.800000

$sequence "idle1" "idle1_ani" fps 20 loop
