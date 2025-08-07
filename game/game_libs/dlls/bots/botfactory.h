#pragma once

#include <cstdint>
#include "botprofiles/botprofiletable.h"

class CBaseBot;
class CUtlString;

// Note that this class does not handle removal of bots.
// Just kick the bot as you would any other player.
class CBotFactory
{
public:
	// Clears any existing bot profiles and reloads from profiles file.
	void LoadBotProfiles();

	// Create n bots with random profiles.
	void CreateBots(uint32_t num);

	// Try to create a bot with a specific profile name.
	bool TryCreateBot(const CUtlString& profileName, const CUtlString& playerName = CUtlString());

private:
	void RandomProfileNameList(CUtlVector<CUtlString>& list, size_t count);
	bool CreateBot(const CBotProfileTable::ProfileData* profile = NULL, const CUtlString& playerName = CUtlString());

	void SetBotAttributesViaProfile(CBaseBot* bot, const CBotProfileTable::ProfileData* profile);
	void SetBotSkin(CBaseBot* bot, const CUtlString& skin);

	uint32_t m_nBotCount;
	CBotProfileTable m_ProfileTable;
};
