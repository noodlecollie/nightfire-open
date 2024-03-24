#include "Framework.h"
#include "PlayerModelView.h"
#include "utlstring.h"

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

		m_View.SetRect(660, 50, 600, 600);
		m_View.SetAllowPitchRotation(true);
		m_View.SetAllowRightButtonZoom(true);
		AddItem(m_View);
	}

	void SelectModel()
	{
		FileDialogGlobals& globalData = FileDialogGlobals::GlobalData();

		globalData.SetResultHasPreview(false);
		LookUpModelSubdirsRecursively();

		globalData.SetResultCallback(
			[this](const char* result)
			{
				m_View.SetModel(result);
				m_View.ResetOrientation();
			});

		UI_FileDialog_Menu();
	}

	void LookUpModelSubdirsRecursively()
	{
		FileDialogGlobals& globalData = FileDialogGlobals::GlobalData();

		globalData.ClearPatterns();
		size_t numDirsChecked = 0;

		// Add the first directory to search
		globalData.AddPattern("models/*.mdl");

		// Keep checking until we found no new directories.
		do
		{
			CUtlString dirPath(globalData.GetPattern(numDirsChecked));
			dirPath.Replace("/*.mdl", "");

			int numSubdirs = 0;
			char** subdirs = EngFuncs::GetDirectoriesList(dirPath.Get(), &numSubdirs, true);

			if ( subdirs && numSubdirs > 0 )
			{
				for ( int index = 0; index < numSubdirs; ++index )
				{
					CUtlString newPattern;
					newPattern.Format("%s/*.mdl", subdirs[index]);
					globalData.AddPattern(newPattern.Get());
				}
			}

			++numDirsChecked;
		}
		while ( globalData.PatternCount() > numDirsChecked );
	}

	CMenuPlayerModelView m_View;
};

ADD_MENU(menu_modelviewer, CMenuModelViewer, UI_ModelViewer_Menu);
