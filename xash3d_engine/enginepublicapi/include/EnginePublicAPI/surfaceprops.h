#pragma once

#define SURFACEPROP_ENTRY_LIST \
	LIST_ITEM(SurfaceProp_None = 0, "none") \
	LIST_ITEM(SurfaceProp_BulletproofGlass, "bulletproofglass") \
	LIST_ITEM(SurfaceProp_Carpet, "carpet") \
	LIST_ITEM(SurfaceProp_Cloth, "cloth") \
	LIST_ITEM(SurfaceProp_Computer, "computer") \
	LIST_ITEM(SurfaceProp_Concrete, "concrete") \
	LIST_ITEM(SurfaceProp_Dirt, "dirt") \
	LIST_ITEM(SurfaceProp_Flesh, "flesh") \
	LIST_ITEM(SurfaceProp_Glass, "glass") \
	LIST_ITEM(SurfaceProp_Gold, "gold") \
	LIST_ITEM(SurfaceProp_Grass, "grass") \
	LIST_ITEM(SurfaceProp_Gravel, "gravel") \
	LIST_ITEM(SurfaceProp_Metal, "metal") \
	LIST_ITEM(SurfaceProp_MetalGrate, "metalgrate") \
	LIST_ITEM(SurfaceProp_Paper, "paper") \
	LIST_ITEM(SurfaceProp_Plaster, "plaster") \
	LIST_ITEM(SurfaceProp_Plastic, "plastic") \
	LIST_ITEM(SurfaceProp_Rock, "rock") \
	LIST_ITEM(SurfaceProp_Rubber, "rubber") \
	LIST_ITEM(SurfaceProp_Sand, "sand") \
	LIST_ITEM(SurfaceProp_Snow, "snow") \
	LIST_ITEM(SurfaceProp_Tile, "tile") \
	LIST_ITEM(SurfaceProp_VentDuct, "ventduct") \
	LIST_ITEM(SurfaceProp_Water, "water") \
	LIST_ITEM(SurfaceProp_Wood, "wood") \
	LIST_ITEM(SurfaceProp_SpecialLadder, "special_ladder")

#define LIST_ITEM(enum, name) enum,
typedef enum _SurfaceProp
{
	SURFACEPROP_ENTRY_LIST SurfaceProp__Count
} SurfaceProp;
#undef LIST_ITEM
