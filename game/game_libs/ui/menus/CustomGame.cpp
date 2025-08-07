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
#include "Bitmap.h"
#include "PicButton.h"
#include "Action.h"
#include "Table.h"
#include "YesNoMessageBox.h"
#include "EnginePublicAPI/keydefs.h"
#include "PlatformLib/String.h"

#define ART_BANNER "gfx/shell/head_custom"

#define MAX_MODS 512  // engine limit

class CMenuModListModel : public CMenuBaseModel
{
public:
	void Update() override;
	int GetColumns() const override
	{
		return 4;
	}
	int GetRows() const override
	{
		return m_iNumItems;
	}
	const char* GetCellText(int line, int column) override
	{
		return modsDescription[line][column];
	}

	char modsDir[MAX_MODS][64];
	char modsWebSites[MAX_MODS][256];
	char modsDescription[MAX_MODS][5][32];

	int m_iNumItems;
};

class CMenuCustomGame : public CMenuFramework
{
public:
	CMenuCustomGame() :
		CMenuFramework("CMenuCustomGame")
	{
	}

private:
	void ShowDialog(void)
	{
		msgBox.ToggleVisibility();
	}
	void ChangeGame(void* pExtra);
	void Go2Site(void* pExtra);
	void UpdateExtras();
	virtual void _Init() override;

	CMenuPicButton* load;
	CMenuPicButton* go2url;

	// prompt dialog
	CMenuYesNoMessageBox msgBox;

	CMenuTable modList;
	CMenuModListModel modListModel;
};

void CMenuCustomGame::ChangeGame(void* pExtra)
{
	char cmd[128];
	PlatformLib_SNPrintF(cmd, sizeof(cmd), "game %s\n", (const char*)pExtra);
	EngFuncs::ClientCmd(FALSE, cmd);
}

void CMenuCustomGame::Go2Site(void* pExtra)
{
	const char* url = (const char*)pExtra;
	if ( url[0] )
		EngFuncs::ShellExecute(url, NULL, false);
}

void CMenuCustomGame::UpdateExtras()
{
	int i = modList.GetCurrentIndex();

	load->onReleased.pExtra = modListModel.modsDir[i];
	load->SetGrayed(!PlatformLib_StrCaseCmp(modListModel.modsDir[i], gMenu.m_gameinfo.gamefolder));

	go2url->onReleased.pExtra = modListModel.modsWebSites[i];
	go2url->SetGrayed(modListModel.modsWebSites[i][0] == 0);

	msgBox.onPositive.pExtra = modListModel.modsDir[i];
}

/*
=================
CMenuModListModel::Update
=================
*/
void CMenuModListModel::Update(void)
{
	int numGames, i;
	GAMEINFO** games;

	games = EngFuncs::GetGamesList(&numGames);

	for ( i = 0; i < numGames; i++ )
	{
		Q_strncpy(modsDir[i], games[i]->gamefolder, sizeof(modsDir[i]));
		Q_strncpy(modsWebSites[i], games[i]->game_url, sizeof(modsWebSites[i]));

		Q_strncpy(modsDescription[i][0], games[i]->type, 32);

		if ( ColorStrlen(games[i]->title) > 31 )  // NAME_LENGTH
		{
			Q_strncpy(modsDescription[i][1], games[i]->title, 32 - 4);
			// I am lazy to put strncat here :(
			modsDescription[i][1][28] = modsDescription[i][1][29] = modsDescription[i][1][30] = '.';
			modsDescription[i][1][31] = 0;
		}
		else
			Q_strncpy(modsDescription[i][1], games[i]->title, 32);

		Q_strncpy(modsDescription[i][2], games[i]->version, 32);

		if ( games[i]->size[0] && atoi(games[i]->size) != 0 )
			Q_strncpy(modsDescription[i][3], games[i]->size, 32);
		else
			Q_strncpy(modsDescription[i][3], "0.0 Mb", 32);
	}

	m_iNumItems = numGames;
}

/*
=================
UI_CustomGame_Init
=================
*/
void CMenuCustomGame::_Init(void)
{
	banner.SetPicture(ART_BANNER);

	msgBox.SetMessage(L("GameUI_ForceGameRestart"));
	msgBox.onPositive = MenuCb(&CMenuCustomGame::ChangeGame);
	msgBox.Link(this);

	AddItem(background);
	AddItem(banner);
	load =
		AddButton(L("Activate"), L("Activate selected custom game"), PC_ACTIVATE, VoidCb(&CMenuCustomGame::ShowDialog));

	go2url = AddButton(
		L("Visit web site"),
		L("Visit the web site of game developers"),
		PC_VISIT_WEB_SITE,
		MenuCb(&CMenuCustomGame::Go2Site));
	AddButton(
		L("Done"),
		L("Return to main menu"),
		PC_DONE,  // Done - уже где-то было, поэтому в отдельный файл повторно не выношу
		VoidCb(&CMenuCustomGame::Hide));

	modList.onChanged = VoidCb(&CMenuCustomGame::UpdateExtras);
	modList.SetupColumn(0, L("GameUI_Type"), 0.20f);
	modList.SetupColumn(1, L("Name"), 0.50f);
	modList.SetupColumn(2, L("Ver"), 0.15f);
	modList.SetupColumn(3, L("Size"), 0.15f);
	modList.SetModel(&modListModel);
	modList.SetRect(360, 230, -20, 465);

	AddItem(modList);

	for ( int i = 0; i < modListModel.GetRows(); i++ )
	{
		if ( !PlatformLib_StrCaseCmp(modListModel.modsDir[i], gMenu.m_gameinfo.gamefolder) )
		{
			modList.SetCurrentIndex(i);
			if ( modList.onChanged )
				modList.onChanged(&modList);
			break;
		}
	}
}

ADD_MENU(menu_customgame, CMenuCustomGame, UI_CustomGame_Menu);
