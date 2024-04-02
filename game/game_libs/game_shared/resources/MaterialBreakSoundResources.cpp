#include "MaterialBreakSoundResources.h"

static constexpr const char* const BreakGlass[] = {
	"break/break_glass1.wav",
};

static constexpr const char* const BreakConcrete[] = {
	"break/break_concrete1.wav",
	"break/break_concrete2.wav",
};

static constexpr const char* const BreakFlesh[] = {
	"break/break_flesh1.wav",
	"break/break_flesh2.wav",
};

static constexpr const char* const BreakMetal[] = {
	"break/break_metal1.wav",
	"break/break_metal2.wav",
};

static constexpr const char* const BreakWood[] = {
	"break/break_wood1.wav",
	"break/break_wood2.wav",
	"break/break_wood3.wav",
	"break/break_wood4.wav",
	"break/break_wood5.wav",
};

CMaterialBreakSoundResources::CMaterialBreakSoundResources() :
	CBaseResourceCollection<MaterialBreakSoundId, static_cast<size_t>(MaterialBreakSoundId::Count)>()
{
	InitialiseResources(MaterialBreakSoundId::BreakGlass, BreakGlass);
	InitialiseResources(MaterialBreakSoundId::BreakWood, BreakWood);
	InitialiseResources(MaterialBreakSoundId::BreakFlesh, BreakFlesh);
	InitialiseResources(MaterialBreakSoundId::BreakMetal, BreakMetal);
	InitialiseResources(MaterialBreakSoundId::BreakConcrete, BreakConcrete);
}
