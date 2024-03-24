#include "Framework.h"
#include "PlayerModelView.h"

// REMOVE ME
#include "enginecallback_menu.h"

class CMenuModelViewer : public CMenuFramework
{
public:
	CMenuModelViewer() :
		CMenuFramework("CMenuModelViewer")
	{
	}

private:
	virtual void _Init() override
	{
		banner.SetPicture("gfx/shell/head_blank");

		AddItem(background);
		AddItem(banner);

		AddButton(L("Select"), L("Select test model"), PC_CUSTOMIZE, VoidCb(&CMenuModelViewer::SelectModel));
		AddButton(L("Back"), L("Return to main menu"), PC_DONE, VoidCb(&CMenuModelViewer::Hide));

		view.SetRect(660, 50, 600, 600);
		view.SetAllowPitchRotation(true);
		view.SetAllowRightButtonZoom(true);
		AddItem(view);
	}

	void SelectModel()
	{
		// FileDialogGlobals& globalData = FileDialogGlobals::GlobalData();

		// globalData.ClearPatterns();
		// globalData.SetResultHasPreview(false);

		// // TODO: Look up all subfolders
		// globalData.AddPattern("models/*.mdl");

		// globalData.SetResultCallback([this](const char* result)
		// {
		// 	view.SetModel(result);
		// 	view.ResetOrientation();
		// });

		// UI_FileDialog_Menu();

		int num = 0;
		char** dirs = EngFuncs::GetDirectoriesList("models", &num, true);
		(void)dirs;
	}

	CMenuPlayerModelView view;
};

ADD_MENU(menu_modelviewer, CMenuModelViewer, UI_ModelViewer_Menu);
