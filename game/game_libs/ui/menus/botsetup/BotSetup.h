#pragma once

#include "InGameBotListModel.h"
#include "utlvector.h"

void UI_BotSetup_Menu();
void BotSetup_GetBotsToAddToGame(CUtlVector<CInGameBotListModel::ListEntry>& list);
