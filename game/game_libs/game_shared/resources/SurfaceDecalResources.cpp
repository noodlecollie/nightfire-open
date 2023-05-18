#include "SurfaceDecalResources.h"

static constexpr const char* const CarpetDecals[] = {"{d_carpet_01", "{d_carpet_02", "{d_carpet_03", "{d_carpet_04"};

static constexpr const char* const DirtDecals[] = {"{d_dirt_01", "{d_dirt_02", "{d_dirt_03", "{d_dirt_04"};

static constexpr const char* const GenericDecals[] = {
	"{d_generic_01",
	"{d_generic_02",
	"{d_generic_03",
	"{d_generic_04"};

static constexpr const char* const GlassDecals[] = {
	"{d_glass_break_01",
	"{d_glass_break_02",
	"{d_glass_break_03",
	"{d_glass_break_04"};

static constexpr const char* const BulletproofGlassDecals[] = {
	"{d_glass_unbreak_01",
	"{d_glass_unbreak_02",
	"{d_glass_unbreak_03",
	"{d_glass_unbreak_04"};

static constexpr const char* const GrassDecals[] = {"{d_grass_01", "{d_grass_02", "{d_grass_03", "{d_grass_04"};

static constexpr const char* const MetalDecals[] = {"{d_metal_01", "{d_metal_02", "{d_metal_03", "{d_metal_04"};

static constexpr const char* const GoldDecals[] = {"{d_gold_01", "{d_gold_02", "{d_gold_03", "{d_gold_04"};

static constexpr const char* const PlasterDecals[] = {
	"{d_plaster_01",
	"{d_plaster_02",
	"{d_plaster_03",
	"{d_plaster_04"};

static constexpr const char* const SandDecals[] = {"{d_sand_01", "{d_sand_02", "{d_sand_03", "{d_sand_04"};

static constexpr const char* const ScorchDecals[] = {"{d_scorch_01", "{d_scorch_02", "{d_scorch_03", "{d_scorch_04"};

static constexpr const char* const SnowDecals[] = {"{d_snow_01", "{d_snow_02", "{d_snow_03", "{d_snow_04"};

static constexpr const char* const StoneDecals[] = {"{d_stone_01", "{d_stone_01", "{d_stone_01", "{d_stone_01"};

static constexpr const char* const WoodDecals[] = {"{d_wood_01", "{d_wood_02", "{d_wood_03", "{d_wood_04"};

CSurfaceDecalResources::CSurfaceDecalResources() :
	CBaseResourceCollection<SurfaceDecalId, static_cast<size_t>(SurfaceDecalId::Count)>()
{
	InitialiseResources(SurfaceDecalId::Generic, GenericDecals);
	InitialiseResources(SurfaceDecalId::Carpet, CarpetDecals);
	InitialiseResources(SurfaceDecalId::Dirt, DirtDecals);
	InitialiseResources(SurfaceDecalId::Glass, GlassDecals);
	InitialiseResources(SurfaceDecalId::BulletproofGlass, BulletproofGlassDecals);
	InitialiseResources(SurfaceDecalId::Grass, GrassDecals);
	InitialiseResources(SurfaceDecalId::Metal, MetalDecals);
	InitialiseResources(SurfaceDecalId::Gold, GoldDecals);
	InitialiseResources(SurfaceDecalId::Plaster, PlasterDecals);
	InitialiseResources(SurfaceDecalId::Sand, SandDecals);
	InitialiseResources(SurfaceDecalId::Scorch, ScorchDecals);
	InitialiseResources(SurfaceDecalId::Snow, SnowDecals);
	InitialiseResources(SurfaceDecalId::Stone, StoneDecals);
	InitialiseResources(SurfaceDecalId::Wood, WoodDecals);

	// The larger the value, the smaller the decal.
	Atts(SurfaceDecalId::None).scale = 0.0f;
	Atts(SurfaceDecalId::Generic).scale = 6.0f;
	Atts(SurfaceDecalId::Carpet).scale = 6.0f;
	Atts(SurfaceDecalId::Dirt).scale = 6.0f;
	Atts(SurfaceDecalId::Glass).scale = 3.0f;
	Atts(SurfaceDecalId::BulletproofGlass).scale = 4.0f;
	Atts(SurfaceDecalId::Grass).scale = 4.0f;
	Atts(SurfaceDecalId::Metal).scale = 8.0f;
	Atts(SurfaceDecalId::Gold).scale = 8.0f;
	Atts(SurfaceDecalId::Plaster).scale = 8.0f;
	Atts(SurfaceDecalId::Sand).scale = 6.0f;
	Atts(SurfaceDecalId::Scorch).scale = 1.0f;
	Atts(SurfaceDecalId::Snow).scale = 6.0f;
	Atts(SurfaceDecalId::Stone).scale = 8.0f;
	Atts(SurfaceDecalId::Wood).scale = 8.0f;
}
