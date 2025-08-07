#include "SurfaceSoundResources.h"
#include "standard_includes.h"

static constexpr const char* const HitConcrete[] = {"surfaceprop/hit_concrete1.wav", "surfaceprop/hit_concrete2.wav"};

static constexpr const char* const HitVentDuct[] = {
	"surfaceprop/hit_ventduct1.wav",
	"surfaceprop/hit_ventduct2.wav",
	"surfaceprop/hit_ventduct3.wav"};

static constexpr const char* const HitFlesh[] = {
	"surfaceprop/hit_flesh1.wav",
	"surfaceprop/hit_flesh2.wav",
	"surfaceprop/hit_flesh3.wav",
	"surfaceprop/hit_flesh4.wav",
	"surfaceprop/hit_flesh5.wav",
	"surfaceprop/hit_flesh6.wav",
	"surfaceprop/hit_flesh7.wav",
	"surfaceprop/hit_flesh8.wav",
};

static constexpr const char* const HitFleshCritical[] = {
	"surfaceprop/hit_flesh_crit1.wav",
	"surfaceprop/hit_flesh_crit2.wav"};

static constexpr const char* const HitGlassA[] = {
	"surfaceprop/hit_glass1.wav",
	"surfaceprop/hit_glass2.wav",
	"surfaceprop/hit_glass3.wav"};

static constexpr const char* const HitGlassB[] = {
	"surfaceprop/hit_glassb1.wav",
	"surfaceprop/hit_glassb2.wav",
	"surfaceprop/hit_glassb3.wav"};

static constexpr const char* const HitMetal[] = {
	"surfaceprop/hit_metal1.wav",
	"surfaceprop/hit_metal2.wav",
	"surfaceprop/hit_metal3.wav"};

static constexpr const char* const HitMetalGrate[] = {
	"surfaceprop/hit_metalgrate1.wav",
	"surfaceprop/hit_metalgrate2.wav",
	"surfaceprop/hit_metalgrate3.wav"};

static constexpr const char* const HitSnow[] = {"surfaceprop/hit_snow1.wav"};

static constexpr const char* const HitSand[] = {
	"surfaceprop/hit_sand1.wav",
	"surfaceprop/hit_sand2.wav",
	"surfaceprop/hit_sand3.wav",
	"surfaceprop/hit_sand4.wav"};

static constexpr const char* const HitWood[] = {
	"surfaceprop/hit_wood1.wav",
	"surfaceprop/hit_wood2.wav",
	"surfaceprop/hit_wood3.wav",
	"surfaceprop/hit_wood4.wav",
	"surfaceprop/hit_wood5.wav"};

static constexpr const char* const HitTile[] = {
	"surfaceprop/hit_tile1.wav",
	"surfaceprop/hit_tile2.wav",
	"surfaceprop/hit_tile3.wav",
	"surfaceprop/hit_tile4.wav"};

static constexpr const char* const HitWater[] = {"surfaceprop/hit_water1.wav", "surfaceprop/hit_water2.wav"};

static constexpr const char* const HitPlaster[] = {
	"surfaceprop/hit_plaster1.wav",
	"surfaceprop/hit_plaster2.wav",
	"surfaceprop/hit_plaster3.wav"};

static constexpr const char* const HitPlastic[] = {
	"surfaceprop/hit_plastic1.wav",
	"surfaceprop/hit_plastic2.wav",
	"surfaceprop/hit_plastic3.wav"};

static constexpr const char* const HitPaper[] = {"surfaceprop/hit_paper1.wav", "surfaceprop/hit_paper2.wav"};

CSurfaceSoundResources::CSurfaceSoundResources() :
	CBaseResourceCollection<SurfaceSoundId, static_cast<size_t>(SurfaceSoundId::Count)>()
{
	InitialiseResources(SurfaceSoundId::HitConcrete, HitConcrete);
	InitialiseResources(SurfaceSoundId::HitVentDuct, HitVentDuct);
	InitialiseResources(SurfaceSoundId::HitFlesh, HitFlesh);
	InitialiseResources(SurfaceSoundId::HitFleshCritical, HitFleshCritical);
	InitialiseResources(SurfaceSoundId::HitGlassA, HitGlassA);
	InitialiseResources(SurfaceSoundId::HitGlassB, HitGlassB);
	InitialiseResources(SurfaceSoundId::HitMetal, HitMetal);
	InitialiseResources(SurfaceSoundId::HitMetalGrate, HitMetalGrate);
	InitialiseResources(SurfaceSoundId::HitSnow, HitSnow);
	InitialiseResources(SurfaceSoundId::HitSand, HitSand);
	InitialiseResources(SurfaceSoundId::HitWood, HitWood);
	InitialiseResources(SurfaceSoundId::HitTile, HitTile);
	InitialiseResources(SurfaceSoundId::HitWater, HitWater);
	InitialiseResources(SurfaceSoundId::HitPlaster, HitPlaster);
	InitialiseResources(SurfaceSoundId::HitPlastic, HitPlastic);
	InitialiseResources(SurfaceSoundId::HitPaper, HitPaper);
}
