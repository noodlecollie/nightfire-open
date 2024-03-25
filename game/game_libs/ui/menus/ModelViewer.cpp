#include "Framework.h"
#include "StudioModelView.h"
#include "Table.h"
#include "StringVectorModel.h"
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
		static constexpr int TOP_MARGIN = 50;
		static constexpr int LEFT_MARGIN = 300;
		static constexpr int RIGHT_MARGIN = 100;
		static constexpr int BOTTOM_MARGIN = 50;
		static constexpr int PADDING = 30;

		banner.SetPicture("gfx/shell/head_blank");

		AddItem(background);
		AddItem(banner);

		AddButton(L("Select"), L("Select model"), PC_CUSTOMIZE, VoidCb(&CMenuModelViewer::SelectModel));
		AddButton(L("Back"), L("Return to main menu"), PC_DONE, VoidCb(&CMenuModelViewer::Hide));

		const int itemWidth =
			(static_cast<int>(ScreenWidth / uiStatic.scaleX) - LEFT_MARGIN - RIGHT_MARGIN - PADDING) / 2;
		const int itemHeight = static_cast<int>(ScreenHeight / uiStatic.scaleY) - TOP_MARGIN - BOTTOM_MARGIN;

		m_SequenceTable.SetRect(LEFT_MARGIN, TOP_MARGIN, itemWidth, itemHeight);
		m_SequenceTable.SetModel(&m_Model);
		m_SequenceTable.onChanged = VoidCb(&CMenuModelViewer::HandleSequenceChanged);
		AddItem(m_SequenceTable);

		m_View.SetRect(LEFT_MARGIN + itemWidth + PADDING, TOP_MARGIN, itemWidth, itemHeight);
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
				HandleNewModelPicked(result);
			});

		UI_FileDialog_Menu();
	}

	void HandleNewModelPicked(const char* path)
	{
		m_View.SetModel(path);
		m_View.ResetOrientation();

		int numSequences = EngFuncs::GetModelSequenceCount(m_View.ent);
		m_Model.Purge();

		for ( int index = 0; index < numSequences; ++index )
		{
			m_Model.AddToTail(CUtlString(EngFuncs::GetModelSequenceName(m_View.ent, index)));
		}

		m_View.FitModelToViewVertically();
	}

	void HandleSequenceChanged()
	{
		if ( m_View.ent )
		{
			m_View.ent->curstate.sequence = m_SequenceTable.GetCurrentIndex();
		}
	}

	void LookUpModelSubdirsRecursively()
	{
		FileDialogGlobals& globalData = FileDialogGlobals::GlobalData();

		globalData.ClearPatterns();
		int numDirsChecked = 0;

		CUtlVector<CUtlString> patternList;

		// Add the first directory to search
		patternList.AddToTail(CUtlString("models"));

		// Keep checking until we found no new directories.
		do
		{
			int numSubdirs = 0;
			char** subdirs = EngFuncs::GetDirectoriesList(patternList[numDirsChecked].Get(), &numSubdirs, true);

			if ( subdirs && numSubdirs > 0 )
			{
				for ( int index = 0; index < numSubdirs; ++index )
				{
					patternList.AddToTail(CUtlString(subdirs[index]));
				}
			}

			++numDirsChecked;
		}
		while ( patternList.Count() > numDirsChecked );

		// patternList.Sort() does stupid casting that throws warnings that aren't
		// easy to resolve, so we just call this manually.
		qsort(
			&patternList.Head(),
			patternList.Count(),
			sizeof(CUtlString),
			[](const void* a, const void* b)
			{
				const CUtlString* aStr = reinterpret_cast<const CUtlString*>(a);
				const CUtlString* bStr = reinterpret_cast<const CUtlString*>(b);
				return strcmp(aStr ? aStr->Get() : "", bStr ? bStr->Get() : "");
			});

		FOR_EACH_VEC(patternList, index)
		{
			CUtlString& pattern = patternList[index];
			pattern.Append("/*.mdl");
			globalData.AddPattern(pattern.Get());
		}
	}

	CMenuStudioModelView m_View;
	CMenuTable m_SequenceTable;
	CStringVectorModel m_Model;
};

ADD_MENU(menu_modelviewer, CMenuModelViewer, UI_ModelViewer_Menu);
