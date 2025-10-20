#pragma once

#include <cstdint>
#include "botprofiles/botprofiletable.h"

class CBasePlayer;
class CUtlString;
struct edict_s;

namespace Botrix
{
	// Note that this class does not handle removal of bots.
	// Just kick the bot as you would any other player.
	class CBotFactory
	{
	public:
		static void RegisterCvars();

		static CUtlString GetSanitisedNetName(const CBotProfileTable::ProfileData& profile);

		// Does not set name - assumes this has already been set.
		static void SetBotAttributesViaProfile(struct edict_s* bot, const CBotProfileTable::ProfileData& profile);

		// Clears any existing bot profiles and reloads from profiles file.
		void LoadBotProfiles();

		// Create n bots with random profiles.
		void CreateBots(uint32_t num);

		// Try to create a bot with a specific profile name.
		bool TryCreateBot(const CUtlString& profileName, const CUtlString& playerName = CUtlString());

		CBotProfileTable& ProfileTable();

	private:
		void RandomProfileNameList(CUtlVector<CUtlString>& list, size_t count);
		bool
		CreateBot(const CBotProfileTable::ProfileData* profile = NULL, const CUtlString& playerName = CUtlString());

		static void SetBotSkin(CBasePlayer* bot, const CUtlString& skin);

		uint32_t m_nBotCount;
		CBotProfileTable m_ProfileTable;
	};
}  // namespace Botrix
