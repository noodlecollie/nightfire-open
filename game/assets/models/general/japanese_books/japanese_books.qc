$cdtexture "../../../../afterburner/textures"
$cd "."
$modelname "japanese_books.mdl"

$root "open book"

$flags 2048
$origin 0.000000 0.000000 0.000000
$eyeposition 0.000000 0.000000 0.000000
$bbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000
$cbox 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000



$bodygroup "books"
{
	studio "japanese_book1_ref"
	studio "japanese_book2_ref"
	studio "japanese_book3_ref"
	studio "japanese_book4_ref"
}



$hbox 0 "open book" -12.060000 -6.800000 -1.510000 12.160000 7.100000 1.000000
$hbox 0 "Open book" -5.970000 -6.910000 0.000000 5.970000 6.910000 3.180000

$sequence "idle1" "idle1_ani" ACT_IDLE 1 fps 25 loop
