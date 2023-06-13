#include "ShellImpactSoundResources.h"
#include "standard_includes.h"

static constexpr const char* const BrassShellOnHardSurface[] = {
	"shells/brass_conc1.wav",
	"shells/brass_conc2.wav",
	"shells/brass_conc3.wav",
	"shells/brass_conc4.wav"};

static constexpr const char* const HollowShellOnHardSurface[] = {
	"shells/hollow_conc1.wav",
	"shells/hollow_conc2.wav",
	"shells/hollow_conc3.wav",
	"shells/hollow_conc4.wav",
	"shells/hollow_conc5.wav",
	"shells/hollow_conc6.wav"};

static constexpr const char* const ShellOnMetalSurface[] = {
	"shells/metal1.wav",
	"shells/metal2.wav",
	"shells/metal3.wav",
	"shells/metal4.wav"};

static constexpr const char* const SolidShellOnConcreteSurface[] = {"shells/solid_conc1.wav", "shells/solid_conc2.wav"};

static constexpr const char* const SolidShellOnTileSurface[] = {"shells/tile1.wav", "shells/tile2.wav"};

static constexpr const char* const ShellWaterImpact[] = {"shells/water1.wav", "shells/water2.wav"};

static constexpr const char* const SolidShellOnWoodSurface[] = {"shells/wood1.wav", "shells/wood2.wav"};

CShellImpactSoundResources::CShellImpactSoundResources() :
	CBaseResourceCollection<ShellImpactSoundId, static_cast<size_t>(ShellImpactSoundId::Count)>()
{
	InitialiseResources(ShellImpactSoundId::BrassOnHardSurface, BrassShellOnHardSurface);
	InitialiseResources(ShellImpactSoundId::HollowOnHardSurface, HollowShellOnHardSurface);
	InitialiseResources(ShellImpactSoundId::IntoWater, ShellWaterImpact);
	InitialiseResources(ShellImpactSoundId::SolidOnHardSurface, SolidShellOnTileSurface);
	InitialiseResources(ShellImpactSoundId::SolidOnMediumSurface, SolidShellOnConcreteSurface);
	InitialiseResources(ShellImpactSoundId::SolidOnMetalSurface, ShellOnMetalSurface);
	InitialiseResources(ShellImpactSoundId::SolidOnWoodSurface, SolidShellOnWoodSurface);
}

const char* CShellImpactSoundResources::RandomResourcePathForImpact(ShellType shell, SurfaceProp surfaceProp) const
{
	if ( surfaceProp == SurfaceProp_None )
	{
		return nullptr;
	}

	if ( surfaceProp == SurfaceProp_Water )
	{
		return RandomResourcePath(ShellImpactSoundId::IntoWater);
	}

	switch ( shell )
	{
		case ShellType::Default:
		{
			return DefaultShellImpact(surfaceProp);
		}

		case ShellType::Shotgun:
		{
			return ShotgunShellImpact(surfaceProp);
		}

		case ShellType::Heavy:
		{
			return HeavyShellImpact(surfaceProp);
		}

		default:
		{
			return nullptr;
		}
	}
}

const char* CShellImpactSoundResources::DefaultShellImpact(SurfaceProp) const
{
	return RandomResourcePath(ShellImpactSoundId::BrassOnHardSurface);
}

const char* CShellImpactSoundResources::ShotgunShellImpact(SurfaceProp) const
{
	return RandomResourcePath(ShellImpactSoundId::HollowOnHardSurface);
}

const char* CShellImpactSoundResources::HeavyShellImpact(SurfaceProp surfaceProp) const
{
	switch ( surfaceProp )
	{
		case SurfaceProp_Carpet:
		case SurfaceProp_Cloth:
		case SurfaceProp_Dirt:
		case SurfaceProp_Flesh:
		case SurfaceProp_Grass:
		case SurfaceProp_Gravel:
		case SurfaceProp_Paper:
		case SurfaceProp_Plaster:
		case SurfaceProp_Plastic:
		case SurfaceProp_Rubber:
		case SurfaceProp_Sand:
		case SurfaceProp_Snow:
		{
			return RandomResourcePath(ShellImpactSoundId::SolidOnMediumSurface);
		}

		case SurfaceProp_Gold:
		case SurfaceProp_Metal:
		case SurfaceProp_VentDuct:
		case SurfaceProp_MetalGrate:
		{
			return RandomResourcePath(ShellImpactSoundId::SolidOnMetalSurface);
		}

		case SurfaceProp_Wood:
		{
			return RandomResourcePath(ShellImpactSoundId::SolidOnWoodSurface);
		}

		default:
		{
			return RandomResourcePath(ShellImpactSoundId::SolidOnHardSurface);
		}
	}
}
