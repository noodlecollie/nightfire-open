#pragma once

#include "InGameBotListModel.h"
#include "utlvector.h"

void UI_BotSetup_Menu();
void BotSetup_SetHideCallback(std::function<void()> callback);
void BotSetup_SetMaxClients(int max);
void BotSetup_GetBotsToAddToGame(CUtlVector<CInGameBotListModel::ListEntry>& list);
int BotSetup_GetCachedBotCount();
bool BotSetup_ShouldFillTo60Percent();
