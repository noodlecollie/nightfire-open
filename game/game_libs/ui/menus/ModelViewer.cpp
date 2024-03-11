#include "Framework.h"
#include "PlayerModelView.h"

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

		AddButton(L("Select"), L("Select test model"), PC_OK, VoidCb(&CMenuModelViewer::SelectTestModel));
		AddButton(L("Back"), L("Return to main menu"), PC_DONE, VoidCb(&CMenuModelViewer::Hide));

		view.SetRect(660, 50, 600, 600);
		view.SetAllowPitchRotation(true);
		view.SetAllowRightButtonZoom(true);
		AddItem(view);
	}

	void SelectTestModel()
	{
		view.SetModel("models/acunit.mdl");
	}

	CMenuPlayerModelView view;
};

ADD_MENU(menu_modelviewer, CMenuModelViewer, UI_ModelViewer_Menu);
