#include "BotSetup.h"
#include "Framework.h"
#include "Table.h"
#include "BotProfileListModel.h"
#include "InGameBotListModel.h"
#include "Field.h"
#include "StudioSceneModel.h"
#include "PlayerModelView.h"

static CUtlVector<CInGameBotListModel::ListEntry> CachedInGameBotList;

class CMenuBotSetup : public CMenuFramework
{
public:
	CMenuBotSetup();

	virtual void Show() override;
	virtual void Hide() override;

protected:
	virtual void _Init() override;
	virtual void _VidInit() override;

private:
	static constexpr int RIGHT_EDGE_MARGIN = 30;
	static constexpr int BOTTOM_EDGE_MARGIN = 80;
	static constexpr int LIST_TOP_OFFSET = 200;
	static constexpr int LIST_SPACING = 300;
	static constexpr int LEFT_MARGIN = 200;
	static constexpr int LIST_WIDTH = 250;
	static constexpr int PREVIEW_WIDTH = 208;

	static constexpr int CENTRAL_CONTROL_SPACING = 15;
	static constexpr int PREVIEW_TOP_OFFSET = LIST_TOP_OFFSET + 32;
	static constexpr int PREVIEW_LIST_LEFT_SPACING = (LIST_SPACING - PREVIEW_WIDTH) / 2;
	static constexpr int NAME_BOX_WIDTH = 208;
	static constexpr int NAME_BOX_LEFT_SPACING = (LIST_SPACING - NAME_BOX_WIDTH) / 2;

	static constexpr int MAX_BOT_NAME_LENGTH = 32;

	void AddButtonPressed();
	void RemoveButtonPressed();
	void RemoveAllButtonPressed();

	void CacheCurrentInGameBotList();
	void PopulateFromCachedInGameBotList();

	void RecalculateDimensions();
	void UpdateSelectedProfileDataFromUI();
	void UpdateUIFromSelectedProfileData();
	void UpdateButtonStates();
	void ClearSelectedProfile();
	void ResetSelectedTableIndices();

	CMenuTable m_BotProfileList;
	CBotProfileListModel m_BotProfileListModel;

	CMenuTable m_InGameBotList;
	CInGameBotListModel m_InGameBotListModel;
	CMenuPlayerModelView m_BotStudioView;
	CStudioSceneModel m_BotStudioModel;
	CMenuField m_SelectedBotName;
	CMenuPicButton m_AddButton;
	CMenuPicButton m_RemoveButton;
	CMenuPicButton m_RemoveAllButton;

	int m_iSidePadding;
	CBotProfileTable::ProfileData m_SelectedProfile;
	cl_entity_t* m_BotModelEnt = nullptr;
};

CMenuBotSetup::CMenuBotSetup() :
	CMenuFramework("CMenuBotSetup"),
	m_iSidePadding(0)
{
}

void CMenuBotSetup::_Init()
{
	AddItem(background);

	m_BotProfileListModel.SetItemActivatedCallback(
		[this](int, const CBotProfileTable::ProfileData& data)
		{
			m_SelectedProfile = data;

			UpdateUIFromSelectedProfileData();
			UpdateButtonStates();
		});

	m_BotProfileList.SetCharSize(QM_SMALLFONT);
	m_BotProfileList.SetupColumn(0, L("Available"), 1.0f);
	m_BotProfileList.SetModel(&m_BotProfileListModel);
	AddItem(m_BotProfileList);

	m_InGameBotListModel.SetItemDeleteCallback(
		[this]()
		{
			m_InGameBotList.Reload();
			UpdateButtonStates();
		});

	m_InGameBotList.SetCharSize(QM_SMALLFONT);
	m_InGameBotList.SetupColumn(0, L("In Game"), 1.0f);
	m_InGameBotList.SetModel(&m_InGameBotListModel);
	AddItem(m_InGameBotList);

	m_BotModelEnt = m_BotStudioModel.AddEntData();
	m_BotStudioView.SetModel(&m_BotStudioModel);
	m_BotStudioView.SetAllowCyclingSequences(false);

	// If we don't do this, all models will be forced to be
	// drawn as players in the engine. This means that if the
	// menu is open while an existing match is running, the
	// local player's model would always be drawn, instead
	// of the bot models, so we set this property to false.
	m_BotStudioView.SetForceDrawPlayerModel(false);

	AddItem(m_BotStudioView);

	m_SelectedBotName.iMaxLength = MAX_BOT_NAME_LENGTH;
	AddItem(m_SelectedBotName);

	m_AddButton.SetNameAndStatus(L("Add"), L("Add selected bot to the game."));
	m_AddButton.onReleased = VoidCb(&CMenuBotSetup::AddButtonPressed);
	AddItem(m_AddButton);

	m_RemoveButton.SetNameAndStatus(L("Remove"), L("Remove selected bot from the game."));
	m_RemoveButton.onReleased = VoidCb(&CMenuBotSetup::RemoveButtonPressed);
	AddItem(m_RemoveButton);

	m_RemoveAllButton.SetNameAndStatus(L("Remove All"), L("Remove all currently added bots."));
	m_RemoveAllButton.onReleased = VoidCb(&CMenuBotSetup::RemoveAllButtonPressed);
	AddItem(m_RemoveAllButton);

	AddButton(L("Done"), nullptr, PC_DONE, VoidCb(&CMenuBotSetup::Hide));
}

void CMenuBotSetup::_VidInit()
{
	RecalculateDimensions();

	m_BotProfileList.SetRect(LEFT_MARGIN + m_iSidePadding, LIST_TOP_OFFSET, LIST_WIDTH, -BOTTOM_EDGE_MARGIN);
	m_InGameBotList.SetRect(
		LEFT_MARGIN + m_iSidePadding + LIST_WIDTH + LIST_SPACING,
		LIST_TOP_OFFSET,
		LIST_WIDTH,
		-BOTTOM_EDGE_MARGIN);

	const int profileListRightEdge = m_BotProfileList.pos.x + m_BotProfileList.size.w;
	m_BotStudioView.SetCoord(profileListRightEdge + PREVIEW_LIST_LEFT_SPACING, PREVIEW_TOP_OFFSET);
	m_BotStudioView.SetSize(208, 256);

	const int botImageBottomEdge = m_BotStudioView.pos.y + m_BotStudioView.size.h;
	m_SelectedBotName.SetRect(
		profileListRightEdge + NAME_BOX_LEFT_SPACING,
		botImageBottomEdge + CENTRAL_CONTROL_SPACING,
		NAME_BOX_WIDTH,
		m_SelectedBotName.size.h);

	const int botNameBottomEdge = m_SelectedBotName.pos.y + m_SelectedBotName.size.h;
	m_AddButton.SetCoord(profileListRightEdge + (LIST_SPACING / 2) - 25, botNameBottomEdge + CENTRAL_CONTROL_SPACING);

	m_RemoveButton.SetCoord(
		profileListRightEdge + (LIST_SPACING / 2) - 45,
		m_AddButton.pos.y + m_AddButton.size.h);

	m_RemoveAllButton.SetCoord(profileListRightEdge + (LIST_SPACING / 2) - 65, -BOTTOM_EDGE_MARGIN - 25);

	UpdateButtonStates();
}

void CMenuBotSetup::RecalculateDimensions()
{
	Size screenLogicalSize(
		static_cast<int>(ScreenWidth / uiStatic.scaleX),
		static_cast<int>(ScreenHeight / uiStatic.scaleY));

	// Side padding is the distance between the hard left margin and the lists.
	// This is calculated by taking the margin/lists/central area/right margin away from the total width
	// and dividing the remaining area by 2.
	m_iSidePadding = (screenLogicalSize.w - LEFT_MARGIN - (2 * LIST_WIDTH) - LIST_SPACING - RIGHT_EDGE_MARGIN) / 2;

	if ( m_iSidePadding < 0 )
	{
		m_iSidePadding = 0;
	}
}

void CMenuBotSetup::AddButtonPressed()
{
	UpdateSelectedProfileDataFromUI();

	if ( m_SelectedProfile.profileName.Length() > 0 && !m_InGameBotListModel.IsFull() )
	{
		m_InGameBotListModel.AddEntry(m_SelectedProfile.profileName, m_SelectedProfile.playerName);
		m_InGameBotList.Reload();
	}

	UpdateButtonStates();
}

void CMenuBotSetup::RemoveButtonPressed()
{
	int currentIndex = m_InGameBotList.GetCurrentIndex();

	if ( currentIndex >= 0 && currentIndex < m_InGameBotListModel.GetRows() )
	{
		m_InGameBotListModel.RemoveEntryAt(currentIndex);
	}

	UpdateButtonStates();
}

void CMenuBotSetup::RemoveAllButtonPressed()
{
	m_InGameBotListModel.Clear();
	m_InGameBotList.Reload();
	UpdateButtonStates();
}

void CMenuBotSetup::UpdateSelectedProfileDataFromUI()
{
	m_SelectedProfile.playerName = m_SelectedBotName.GetBuffer();
}

void CMenuBotSetup::UpdateUIFromSelectedProfileData()
{
	m_SelectedBotName.SetBuffer(m_SelectedProfile.playerName.String());

	if ( m_BotModelEnt )
	{
		if ( !m_SelectedProfile.skin.IsEmpty() )
		{
			CUtlString modelPath;
			modelPath.Format("models/player/%s.mdl", m_SelectedProfile.skin.String());
			EngFuncs::SetModel(m_BotModelEnt, modelPath.String());
		}
		else
		{
			EngFuncs::SetModel(m_BotModelEnt, nullptr);
		}
	}
}

void CMenuBotSetup::UpdateButtonStates()
{
	m_AddButton.SetGrayed(m_SelectedProfile.profileName.Length() < 1 || m_InGameBotListModel.IsFull());

	int inGameBotIndex = m_InGameBotList.GetCurrentIndex();
	m_RemoveButton.SetGrayed(inGameBotIndex < 0 || inGameBotIndex >= m_InGameBotListModel.GetRows());

	m_RemoveAllButton.SetGrayed(m_InGameBotListModel.GetRows() < 1);
}

void CMenuBotSetup::ClearSelectedProfile()
{
	m_SelectedProfile.profileName.Clear();
	m_SelectedProfile.playerName.Clear();
	m_SelectedProfile.skin.Clear();
	m_SelectedBotName.Clear();
}

void CMenuBotSetup::ResetSelectedTableIndices()
{
	if ( m_BotProfileListModel.GetRows() > 0 )
	{
		m_BotProfileList.SetCurrentIndex(0);
	}

	if ( m_InGameBotListModel.GetRows() > 0 )
	{
		m_InGameBotList.SetCurrentIndex(0);
	}
}

void CMenuBotSetup::CacheCurrentInGameBotList()
{
	CachedInGameBotList.Purge();

	for ( uint32_t index = 0; index < static_cast<uint32_t>(m_InGameBotListModel.GetRows()); ++index )
	{
		CachedInGameBotList.AddToTail(*m_InGameBotListModel.Entry(index));
	}
}

void CMenuBotSetup::PopulateFromCachedInGameBotList()
{
	m_InGameBotListModel.Clear();

	FOR_EACH_VEC(CachedInGameBotList, index)
	{
		const CInGameBotListModel::ListEntry& entry = CachedInGameBotList[index];

		m_InGameBotListModel.AddEntry(entry.profileName, entry.playerName);
	}

	m_InGameBotList.Reload();
	CachedInGameBotList.Purge();
}

void CMenuBotSetup::Show()
{
	CMenuFramework::Show();

	PopulateFromCachedInGameBotList();
	ClearSelectedProfile();
	UpdateButtonStates();
	ResetSelectedTableIndices();
}

void CMenuBotSetup::Hide()
{
	CacheCurrentInGameBotList();
	ClearSelectedProfile();
	UpdateButtonStates();

	CMenuFramework::Hide();
}

void BotSetup_GetBotsToAddToGame(CUtlVector<CInGameBotListModel::ListEntry>& list)
{
	list.Purge();

	FOR_EACH_VEC(CachedInGameBotList, index)
	{
		list.AddToTail(CachedInGameBotList[index]);
	}
}

ADD_MENU(menu_botsetup, CMenuBotSetup, UI_BotSetup_Menu);
