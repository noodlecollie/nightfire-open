#pragma once

#include "InGameBotListModel.h"
#include "utlvector.h"

void UI_BotSetup_Menu();
void BotSetup_SetMaxClients(int max);
void BotSetup_GetBotsToAddToGame(CUtlVector<CInGameBotListModel::ListEntry>& list);
