*The following was taken from the [JBN BSP Lump Tools](https://code.google.com/archive/p/jbn-bsp-lump-tools/) repository.*

# Introduction

As the decompiler is now in nearly perfect shape, it's time to make a compatibility list. Below is a list of maps and their optimal decompiler options.

One thing to keep in mind, although 87 or 72 bad solids may sound like a lot, compared to the several thousand solids in the entire level this is a relatively small number.

Note: There might be a coefficient that works better than the one listed here. Also, when using a coefficient of 1, round-off errors may cause solids to not line up properly. Use a larger coefficient if necessary (such as 10 or 100).

*Note: the coefficient value corresponds to the `Options -> Set Plane Point Coefficient` option in the decompiler.*

# Details

## Official Gearbox Maps

### Mission maps

| Map | Skip Vertex Check | Best Known Coefficient | Status | Notes |
|-----|-------------------|------------------------|--------|-------|
| m1_austria01 | no | 25 | 10 invalid |  |
| m1_austria02 | no | 10 | 14 invalid |  |
| m1_austria03 | no | 10 | 4 invalid |  |
| m1_austria04 | no | 10 | 1 invalid |  |
| m2_airfield01 | no | 100 | 87 invalid | Most of the bad solids are terrain around the map |
| m3_japan01 | no | 100 | 2 invalid |  |
| m3_japan02 | no | 50 | 1 invalid |  |
| m3_japan03 | no | 50 | 12 invalid |  |
| m3_japan04 | no | 50 | 6 invalid |  |
| m4_infiltrate01 | no | 1 | Perfect |  |
| m4_infiltrate02 | no | 5 | Perfect |  |
| m4_infiltrate03 | no | 100 | 1 invalid | It's a playerclip brush around the sculpture in the middle |
| m4_infiltrate04 | no | 10 | 6 invalid |  |
| m4_infiltrate05 | no | 1 | Perfect |  |
| m4_infiltrate06 | no | 1 | Perfect |  |
| m4_infiltrate07 | no | 20 | Perfect | Penthouse ceiling glass seems to disappear, but there's no bad solids |
| m5_power01 | no | 10 | 72 invalid |  |
| m5_power02 | no | 1 | Perfect |  |
| m6_escape01 | no | 5 | 1 invalid |  |
| m6_escape03 | no | 25 | 1 invalid |  |
| m6_escape04 | no | 10 | 8 invalid |  |
| m6_escape05 | no | 10 | 2 invalid |  |
| m6_escape06 | no | 5 | 1 invalid |  |
| m6_escape07 | no | 5 | Perfect |  |
| m7_island01 | no | 1 | 3 invalid |  |
| m7_island02 | no | 5 | 1 invalid |  |
| m7_island03 | no | 1 | 5 invalid | The biggest map in the game! |
| m7_island04 | no | 1 | Perfect |  |
| m7_island05 | no | 1 | 27 invalid |  |
| m7_island06 | no | 1 | 14 invalid |  |
| m8_missile01 | no | 10 | 2 invalid |  |
| m8_missile02 | no | 5 | 5 invalid |  |
| m8_missile03 | no | 15 | 6 invalid |  |
| m8_missile04 | no | 1 | 8 invalid |  |
| m9_space01 | no | 5 | 11 invalid |  |

### Deathmatch maps

| Map | Skip Vertex Check | Best Known Coefficient | Status | Notes |
|-----|-------------------|------------------------|--------|-------|
| dm_austria | no | 10 | 10 invalid |  |
| dm_casino | no | 1 | Perfect |  |
| dm_caviar | no | 1 | Perfect |  |
| dm_island | no | 1 | 1 invalid |  |
| dm_japan | no | 25 | 3 invalid |  |
| dm_jungle | no | 1 | 4 invalid |  |
| dm_knox | no | 1 | Perfect |  |
| dm_maint | no | 1 | Perfect |  |
| dm_office | no | 5 | Perfect |  |
| dm_power | no | 1 | 37 invalid |  |

### Capture the Flag maps

| Map | Skip Vertex Check | Best Known Coefficient | Status | Notes |
|-----|-------------------|------------------------|--------|-------|
| ctf_austria | no | 25 | 15 invalid |  |
| ctf_island | no | 1 | Perfect |  |
| ctf_japan | no | 25 | 10 invalid |  |
| ctf_jungle | no | 1 | 38 invalid |  |
| ctf_knox | no | 1 | Perfect |  |
| ctf_office | no | 25 | 3 invalid |  |
| ctf_romania | no | 25 | 1 invalid |  |
| ctf_tower | no | 1 | Perfect |  |

## Identifying Bad Solids
Does the map you're decompiling have bad solids? Wondering where or what they are? Follow these steps:

  1. Open the map in GearCraft
  2. In one of the 2D views, zoom out all the way
  3. Press Ctrl+A to select the entire map
  4. Go into vertex manipulation mode (this forces the editor to recalculate all vertices and save them)
  5. Go back into selection mode
  6. Go into Map -> Check for problems
  7. Select one of the "Invalid solid structure" entries
  8. Find the solid in the 2D and 3D views
