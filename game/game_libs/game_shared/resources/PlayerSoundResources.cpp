#include "PlayerSoundResources.h"

static constexpr const char* const FemalePain[] = {
	"player/fem_pain1.wav",
	"player/fem_pain2.wav",
	"player/fem_pain3.wav",
	"player/fem_pain4.wav",
	"player/fem_pain5.wav",
	"player/fem_pain6.wav",
	"player/fem_pain7.wav",
};

static constexpr const char* const FemaleDie[] = {
	"player/fem_die1.wav",
};

static constexpr const char* const MalePain[] = {
	"player/male_pain1.wav",  "player/male_pain2.wav",  "player/male_pain3.wav",  "player/male_pain4.wav",
	"player/male_pain5.wav",  "player/male_pain6.wav",  "player/male_pain7.wav",  "player/male_pain8.wav",
	"player/male_pain9.wav",  "player/male_pain10.wav", "player/male_pain11.wav", "player/male_pain12.wav",
	"player/male_pain13.wav", "player/male_pain14.wav", "player/male_pain15.wav", "player/male_pain16.wav",
	"player/male_pain17.wav", "player/male_pain18.wav", "player/male_pain19.wav", "player/male_pain20.wav",
};

static constexpr const char* const MaleDie[] = {
	"player/male_die1.wav",
	"player/male_die2.wav",
	"player/male_die3.wav",
};

static constexpr const char* const BondPain[] = {
	"player/bond_pain1.wav",
	"player/bond_pain2.wav",
	"player/bond_pain3.wav",
	"player/bond_pain4.wav",
	"player/bond_pain5.wav",
	"player/bond_pain6.wav",
	"player/bond_pain7.wav",
	"player/bond_pain8.wav",
};

static constexpr const char* const BondDie[] = {
	"player/bond_die1.wav",
	"player/bond_die2.wav",
	"player/bond_die3.wav",
	"player/bond_die4.wav",
};

static constexpr const char* const Drown[] = {
	"player/drown.wav",
};

CPlayerSoundResources::CPlayerSoundResources() :
	CBaseResourceCollection<PlayerSoundId, static_cast<size_t>(PlayerSoundId::Count)>()
{
	InitialiseResources(PlayerSoundId::DieFemale, FemaleDie);
	InitialiseResources(PlayerSoundId::PainFemale, FemalePain);
	InitialiseResources(PlayerSoundId::DieMale, MaleDie);
	InitialiseResources(PlayerSoundId::PainMale, MalePain);
	InitialiseResources(PlayerSoundId::DieBond, BondDie);
	InitialiseResources(PlayerSoundId::PainBond, BondPain);
	InitialiseResources(PlayerSoundId::Drown, Drown);
}
