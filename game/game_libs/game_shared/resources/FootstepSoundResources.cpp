#include "FootstepSoundResources.h"
#include "standard_includes.h"
#include "projectInterface/IProjectInterface.h"

static constexpr const char* const FootstepsCarpet[] = {
	"footsteps/step_carpet1.wav",
	"footsteps/step_carpet2.wav",
	"footsteps/step_carpet3.wav",
	"footsteps/step_carpet4.wav",
};

static constexpr const char* const FootstepsConcreteClean[] = {
	"footsteps/step_conc_clean1.wav",
	"footsteps/step_conc_clean2.wav",
	"footsteps/step_conc_clean3.wav",
	"footsteps/step_conc_clean4.wav",
};

static constexpr const char* const FootstepsConcreteGrit[] = {
	"footsteps/step_conc_grit1.wav",
	"footsteps/step_conc_grit2.wav",
	"footsteps/step_conc_grit3.wav",
	"footsteps/step_conc_grit4.wav",
};

static constexpr const char* const FootstepsDirt[] = {
	"footsteps/step_dirt1.wav",
	"footsteps/step_dirt2.wav",
	"footsteps/step_dirt3.wav",
	"footsteps/step_dirt4.wav",
};

static constexpr const char* const FootstepsGrass[] = {
	"footsteps/step_grass1.wav",
	"footsteps/step_grass2.wav",
	"footsteps/step_grass3.wav",
	"footsteps/step_grass4.wav",
};

static constexpr const char* const FootstepsIce[] = {
	"footsteps/step_ice1.wav",
	"footsteps/step_ice2.wav",
	"footsteps/step_ice3.wav",
};

static constexpr const char* const FootstepsLadder[] = {
	"footsteps/step_ladder1.wav",
	"footsteps/step_ladder2.wav",
	"footsteps/step_ladder3.wav",
	"footsteps/step_ladder4.wav",
};

static constexpr const char* const FootstepsMetal[] = {
	"footsteps/step_metal1.wav",
	"footsteps/step_metal2.wav",
	"footsteps/step_metal3.wav",
	"footsteps/step_metal4.wav",
};

static constexpr const char* const FootstepsSnow[] = {
	"footsteps/step_snow1.wav",
	"footsteps/step_snow2.wav",
};

static constexpr const char* const FootstepsVentDuct[] = {
	"footsteps/step_ventduct1.wav",
	"footsteps/step_ventduct2.wav",
	"footsteps/step_ventduct3.wav",
};

static constexpr const char* const FootstepsWoodClean[] = {
	"footsteps/step_wood_clean1.wav",
	"footsteps/step_wood_clean2.wav",
	"footsteps/step_wood_clean3.wav",
	"footsteps/step_wood_clean4.wav",
};

static constexpr const char* const FootstepsWoodGrit[] = {
	"footsteps/step_wood_grit1.wav",
	"footsteps/step_wood_grit2.wav",
	"footsteps/step_wood_grit3.wav",
	"footsteps/step_wood_grit4.wav",
};

static constexpr const char* const FootstepsGlass[] = {
	"footsteps/step_glass1.wav",
	"footsteps/step_glass2.wav",
	"footsteps/step_glass3.wav",
	"footsteps/step_glass4.wav",
};

static constexpr const char* const FootstepsMetalGrate[] = {
	"footsteps/step_metalgrate1.wav",
	"footsteps/step_metalgrate2.wav",
	"footsteps/step_metalgrate3.wav",
	"footsteps/step_metalgrate4.wav",
};

static constexpr const char* const FootstepsWater[] = {
	"footsteps/step_water1.wav",
	"footsteps/step_water2.wav",
	"footsteps/step_water3.wav",
	"footsteps/step_water4.wav",
};

static constexpr const char* const FootstepsWaterWade[] = {
	"footsteps/step_waterwade1.wav",
	"footsteps/step_waterwade2.wav",
	"footsteps/step_waterwade3.wav",
	"footsteps/step_waterwade4.wav",
};

static constexpr const char* const FootstepsSwim[] = {
	"footsteps/step_swim1.wav",
	"footsteps/step_swim2.wav",
	"footsteps/step_swim3.wav",
	"footsteps/step_swim4.wav",
	"footsteps/step_swim5.wav",
	"footsteps/step_swim6.wav",
};

CFootstepSoundResources::CFootstepSoundResources() :
	CBaseResourceCollection<FootstepSoundId, static_cast<size_t>(FootstepSoundId::Count)>()
{
	InitialiseResources(FootstepSoundId::Carpet, FootstepsCarpet);
	InitialiseResources(FootstepSoundId::ConcreteClean, FootstepsConcreteClean);
	InitialiseResources(FootstepSoundId::ConcreteGrit, FootstepsConcreteGrit);
	InitialiseResources(FootstepSoundId::Dirt, FootstepsDirt);
	InitialiseResources(FootstepSoundId::Grass, FootstepsGrass);
	InitialiseResources(FootstepSoundId::Ice, FootstepsIce);
	InitialiseResources(FootstepSoundId::Ladder, FootstepsLadder);
	InitialiseResources(FootstepSoundId::Metal, FootstepsMetal);
	InitialiseResources(FootstepSoundId::Snow, FootstepsSnow);
	InitialiseResources(FootstepSoundId::VentDuct, FootstepsVentDuct);
	InitialiseResources(FootstepSoundId::WoodClean, FootstepsWoodClean);
	InitialiseResources(FootstepSoundId::WoodGrit, FootstepsWoodGrit);
	InitialiseResources(FootstepSoundId::Glass, FootstepsGlass);
	InitialiseResources(FootstepSoundId::MetalGrate, FootstepsMetalGrate);
	InitialiseResources(FootstepSoundId::ShallowWater, FootstepsWater);
	InitialiseResources(FootstepSoundId::DeepWater, FootstepsWaterWade);
	InitialiseResources(FootstepSoundId::Swim, FootstepsSwim);
}

const char* CFootstepSoundResources::RandomFootstepSoundPath(FootstepSoundId id, Foot foot) const
{
	const uint32_t intId = static_cast<uint32_t>(id);

	if ( intId >= NUM_ENTRIES )
	{
		return nullptr;
	}

	const CUtlVector<CUtlString>& list = m_ResourcePaths[intId];
	const size_t numPaths = list.Count();

	// Only separate left foot sounds from right foot sounds if there are enough
	// to give each foot at least two sounds to choose from. Otherwise,
	// at least one foot would only have one sound to play.

	if ( numPaths < 4 )
	{
		return RandomResourcePath(id);
	}

	// If playing a left foot sound, even indices are used (0, 2, 4, 6, ...).
	// If playing a right foot sound, odd indices are used (1, 3, 5, 7, ...).

	size_t numIndicesToSelectFrom = numPaths / 2;

	// If the count is odd, there are more left foot sounds than right foot sounds.
	if ( (numPaths % 2) != 0 && foot == Foot::Left )
	{
		++numIndicesToSelectFrom;
	}

	// Choose a random index.
	IProjectInterface* ifc = IProjectInterface::ProjectInterfaceImpl();
	uint32_t index = ifc->RNG().GetRandomInt(0, static_cast<int32_t>(numIndicesToSelectFrom - 1));

	// If we are playing a right foot sound, the index must start from 1.
	const uint32_t offset = foot == Foot::Right ? 1 : 0;

	ASSERT(offset + index < static_cast<uint32_t>(list.Count()));

	return list[offset + index].Get();
}
