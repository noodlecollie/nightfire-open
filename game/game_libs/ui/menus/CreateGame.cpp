/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "Framework.h"
#include "EnginePublicAPI/keydefs.h"
#include "Bitmap.h"
#include "Field.h"
#include "CheckBox.h"
#include "PicButton.h"
#include "Table.h"
#include "Action.h"
#include "YesNoMessageBox.h"
#include "Table.h"
#include "botsetup/BotSetup.h"

#define ART_BANNER "gfx/shell/head_creategame"

class CMenuCreateGame;
class CMenuMapListModel : public CMenuBaseModel
{
public:
	CMenuMapListModel(CMenuCreateGame* parent) :
		parent(parent)
	{
	}

	void Update() override;

	int GetColumns() const override
	{
		return 2;
	}

	int GetRows() const override
	{
		return m_iNumItems;
	}

	const char* GetCellText(int line, int column) override
	{
		switch ( column )
		{
			case 0:
				return mapName[line];
			case 1:
				return mapsDescription[line];
		}

		return NULL;
	}

	char mapName[UI_MAXGAMES][64];
	char mapsDescription[UI_MAXGAMES][64];
	int m_iNumItems;
	CMenuCreateGame* parent;
};

class CMenuCreateGame : public CMenuFramework
{
public:
	CMenuCreateGame() :
		CMenuFramework("CMenuCreateGame"),
		mapsListModel(this)
	{
	}
	static void Begin(CMenuBaseItem* pSelf, void* pExtra);

	void Reload(void) override;

	char* mapsDescriptionPtr[UI_MAXGAMES];

	CMenuField maxClients;
	CMenuField hostName;
	CMenuField password;
	CMenuCheckBox nat;
	CMenuField botCount;

	// newgame prompt dialog
	CMenuYesNoMessageBox msgBox;

	CMenuTable mapsList;
	CMenuMapListModel mapsListModel;

	CMenuPicButton* done;

private:
	void _Init() override;
	void _VidInit() override;
	void ShowBotSetupMenu();
	void UpdateBotsField();
};

/*
=================
CMenuCreateGame::Begin
=================
*/
void CMenuCreateGame::Begin(CMenuBaseItem* pSelf, void*)
{
	CMenuCreateGame* menu = (CMenuCreateGame*)pSelf->Parent();
	int item = menu->mapsList.GetCurrentIndex();
	if ( item < 0 || item > UI_MAXGAMES )
		return;

	const char* mapName;
	if ( menu->mapsList.GetCurrentIndex() == 0 )
	{
		int idx = EngFuncs::RandomLong(1, menu->mapsListModel.GetRows());
		mapName = menu->mapsListModel.mapName[idx];
	}
	else
	{
		mapName = menu->mapsListModel.mapName[menu->mapsList.GetCurrentIndex()];
	}

	if ( !EngFuncs::IsMapValid(mapName) )
	{
		return;  // bad map
	}

	if ( EngFuncs::GetCvarFloat("host_serverstate") )
	{
		if ( EngFuncs::GetCvarFloat("maxplayers") == 1.0f )
		{
			EngFuncs::HostEndGame("end of the game");
		}
		else
		{
			EngFuncs::HostEndGame("starting new server");
		}
	}

	EngFuncs::CvarSetValue("deathmatch", 1.0f);  // start deathmatch as default
	EngFuncs::CvarSetString("defaultmap", mapName);
	EngFuncs::CvarSetValue("sv_nat", EngFuncs::GetCvarFloat("public") ? (menu->nat.bChecked ? 1.0f : 0.0f) : 0.0f);
	menu->password.WriteCvar();
	menu->hostName.WriteCvar();
	menu->maxClients.WriteCvar();

	EngFuncs::PlayBackgroundTrack(NULL, NULL);

	// all done, start server
	EngFuncs::WriteServerConfig(EngFuncs::GetCvarString("lservercfgfile"));

	char cmd[1024];
	char cmd2[256];
	PlatformLib_SNPrintF(cmd, sizeof(cmd), "exec %s\n", EngFuncs::GetCvarString("lservercfgfile"));

	EngFuncs::ClientCmd(TRUE, cmd);

	// dirty listenserver config form old xash may rewrite maxplayers
	int maxClients = atoi(menu->maxClients.GetBuffer());
	EngFuncs::CvarSetValue("maxplayers", static_cast<float>(maxClients));

	Com_EscapeCommand(cmd2, mapName, 256);

	// hack: wait three frames allowing server to completely shutdown, reapply maxplayers and start new map
	PlatformLib_SNPrintF(
		cmd,
		sizeof(cmd),
		"endgame;menu_connectionprogress localserver;wait;wait;wait;maxplayers %i;latch;map %s\n",
		atoi(menu->maxClients.GetBuffer()),
		cmd2
	);
	EngFuncs::ClientCmd(FALSE, cmd);

	// Very important!! We need to wait a frame after invoking the map command,
	// or things will not be initialised.
	EngFuncs::ClientCmd(FALSE, "wait");

	if ( BotSetup_ShouldFillTo60Percent() )
	{
		EngFuncs::ClientCmd(FALSE, "bot_fill_to_percent 60");
	}
	else
	{
		CUtlVector<CInGameBotListModel::ListEntry> botList;
		BotSetup_GetBotsToAddToGame(botList);

		if ( botList.Count() > 0 )
		{
			CUtlString botCmd;

			FOR_EACH_VEC(botList, index)
			{
				const CInGameBotListModel::ListEntry& entry = botList[index];

				CUtlString command;
				command.AppendFormat("; bot_register_add \"%s\"", entry.profileName.String());

				if ( !entry.playerName.IsEmpty() )
				{
					command.AppendFormat(" \"%s\"", entry.playerName.String());
				}

				botCmd.Append(command.String());
			}

			EngFuncs::ClientCmd(FALSE, botCmd.String());
		}
	}
}

/*
=================
CMenuMapListModel::Update
=================
*/
void CMenuMapListModel::Update(void)
{
	char* afile;

	if ( !uiStatic.needMapListUpdate )
		return;

	if ( !EngFuncs::CreateMapsList(TRUE) || (afile = (char*)EngFuncs::COM_LoadFile("maps.lst", NULL)) == NULL )
	{
		parent->done->SetGrayed(true);
		m_iNumItems = 0;
		Con_Printf("Cmd_GetMapsList: can't open maps.lst\n");
		return;
	}

	char* pfile = afile;
	char token[1024];
	int numMaps = 1;

	PlatformLib_StrCpy(mapName[0], sizeof(mapName[0]), L("GameUI_RandomMap"));
	mapsDescription[0][0] = 0;

	while ( (pfile = EngFuncs::COM_ParseFile(pfile, token, sizeof(token))) != NULL )
	{
		if ( numMaps >= UI_MAXGAMES )
			break;

		Q_strncpy(mapName[numMaps], token, 64);
		if ( (pfile = EngFuncs::COM_ParseFile(pfile, token, sizeof(token))) == NULL )
		{
			Q_strncpy(mapsDescription[numMaps], mapName[numMaps], 64);
			break;  // unexpected end of file
		}
		Q_strncpy(mapsDescription[numMaps], token, 64);
		numMaps++;
	}

	if ( !(numMaps - 1) )
		parent->done->SetGrayed(true);
	m_iNumItems = numMaps;
	EngFuncs::COM_FreeFile(afile);
	uiStatic.needMapListUpdate = false;
}

/*
=================
CMenuCreateGame::Init
=================
*/
void CMenuCreateGame::_Init(void)
{
	uiStatic.needMapListUpdate = true;
	banner.SetPicture(ART_BANNER);

	nat.SetNameAndStatus("NAT", L("Use NAT Bypass instead of direct mode"));
	nat.bChecked = true;

	// add them here, so "done" button can be used by mapsListModel::Update
	AddItem(background);
	AddItem(banner);
	CMenuPicButton* advOpt =
		AddButton(L("Adv. Options"), L("Open the game advanced options menu"), PC_ADV_OPT, UI_AdvServerOptions_Menu);
	advOpt->SetGrayed(!UI_AdvServerOptions_IsAvailable());

	AddButton(L("Bots"), L("Configure bots for this match."), PC_CONFIG, VoidCb(&CMenuCreateGame::ShowBotSetupMenu));

	done = AddButton(L("GameUI_OK"), L("Start the multiplayer game"), PC_DONE, Begin);
	done->onReleasedClActive = msgBox.MakeOpenEvent();

	mapsList.SetCharSize(QM_SMALLFONT);
	mapsList.SetupColumn(0, L("GameUI_Map"), 0.5f);
	mapsList.SetupColumn(1, L("Title"), 0.5f);
	mapsList.SetModel(&mapsListModel);

	hostName.szName = L("GameUI_ServerName");
	hostName.iMaxLength = 28;
	hostName.LinkCvar("hostname");

	maxClients.iMaxLength = 3;
	maxClients.bNumbersOnly = true;
	maxClients.szName = L("GameUI_MaxPlayers");
	SET_EVENT_MULTI(maxClients.onChanged, {
		(void)pExtra;
		CMenuField* self = (CMenuField*)pSelf;
		const char* buf = self->GetBuffer();
		if ( buf[0] == 0 )
			return;

		int players = atoi(buf);
		if ( players <= 1 )
			self->SetBuffer("2");
		else if ( players > 32 )
			self->SetBuffer("32");
	});
	SET_EVENT_MULTI(maxClients.onCvarGet, {
		(void)pExtra;
		CMenuField* self = (CMenuField*)pSelf;
		const char* buf = self->GetBuffer();

		int players = atoi(buf);
		if ( players <= 1 )
		{
			self->SetBuffer("16");
		}
		else if ( players > 32 )
		{
			self->SetBuffer("32");
		}
	});
	maxClients.LinkCvar("maxplayers");

	password.szName = L("GameUI_Password");
	password.iMaxLength = 16;
	password.eTextAlignment = QM_CENTER;
	password.bHideInput = true;
	password.LinkCvar("sv_password");

	msgBox.onPositive = Begin;
	msgBox.SetMessage(L("Starting a new game will exit any current game, OK to exit?"));
	msgBox.Link(this);

	botCount.szName = L("Bots");
	botCount.eTextAlignment = QM_CENTER;
	botCount.iFlags |= QMF_INACTIVE;

	AddButton(L("GameUI_Cancel"), L("Return to the previous menu"), PC_CANCEL, VoidCb(&CMenuCreateGame::Hide));
	AddItem(hostName);
	AddItem(maxClients);
	AddItem(password);
	AddItem(botCount);
	AddItem(nat);
	AddItem(mapsList);
}

void CMenuCreateGame::_VidInit()
{
	nat.SetCoord(72, 685);

	if ( !EngFuncs::GetCvarFloat("public") )
	{
		nat.Hide();
	}
	else
	{
		nat.Show();
	}

	mapsList.SetRect(590, 230, -20, 465);

	hostName.SetRect(350, 260, 205, 32);
	maxClients.SetRect(350, 360, 205, 32);
	password.SetRect(350, 460, 205, 32);
	botCount.SetRect(350, 560, 205, 32);
}

void CMenuCreateGame::Reload(void)
{
	mapsListModel.Update();
	UpdateBotsField();
}

void CMenuCreateGame::ShowBotSetupMenu()
{
	BotSetup_SetHideCallback(
		[this]()
		{
			UpdateBotsField();
		}
	);

	BotSetup_SetMaxClients(atoi(maxClients.GetBuffer()));
	UI_BotSetup_Menu();
}

void CMenuCreateGame::UpdateBotsField()
{
	char botBuffer[32];
	botBuffer[0] = '\0';

	if ( BotSetup_ShouldFillTo60Percent() )
	{
		PlatformLib_SNPrintF(botBuffer, sizeof(botBuffer), "60%%");
	}
	else
	{
		PlatformLib_SNPrintF(botBuffer, sizeof(botBuffer), "%d", BotSetup_GetCachedBotCount());
	}

	botCount.SetBuffer(botBuffer);
}

ADD_MENU(menu_creategame, CMenuCreateGame, UI_CreateGame_Menu);
