#include "Framework.h"
#include "StudioSceneModel.h"
#include "DeveloperStudioSceneView.h"
#include "Table.h"
#include "StringVectorModel.h"
#include "CheckBox.h"
#include "utlstring.h"

static constexpr int TOP_MARGIN = 50;
static constexpr int LEFT_MARGIN = 300;
static constexpr int RIGHT_MARGIN = 50;
static constexpr int BOTTOM_MARGIN = 50;
static constexpr int PADDING = 30;
static constexpr int BOTTOM_CONTROL_AREA_HEIGHT = 100;
static constexpr int BOTTOM_CONTROL_AREA_PADDING = 15;
static constexpr int BOTTOM_CONTROL_ROW_HEIGHT = 40;

static constexpr float COL1_FACTOR = 0.4f;
static constexpr float COL2_FACTOR = 1.0f - COL1_FACTOR;

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

		AddButton(L("Select"), L("Select model"), PC_CUSTOMIZE, VoidCb(&CMenuModelViewer::SelectModel));
		AddButton(L("Back"), L("Return to main menu"), PC_DONE, VoidCb(&CMenuModelViewer::Hide));

		m_SceneModel.Clear();
		m_MainStudioModel = m_SceneModel.AddEntData();

		AddMainViews();
		AddBottomControls();
	}

	void AddMainViews()
	{
		const int viewHeight = GetViewAreaHeight();
		const int col1Width = GetFirstColumnWidth();
		const int col2Width = GetSecondColumnWidth();
		const int col2Left = LEFT_MARGIN + col1Width + PADDING;

		m_SequenceTable.SetRect(LEFT_MARGIN, TOP_MARGIN, col1Width, viewHeight);
		m_SequenceTable.SetModel(&m_SequenceModel);
		m_SequenceTable.onChanged = VoidCb(&CMenuModelViewer::HandleSequenceChanged);
		AddItem(m_SequenceTable);

		m_SceneView.SetRect(col2Left, TOP_MARGIN, col2Width, viewHeight);
		m_SceneView.SetAllowPitchRotation(true);
		m_SceneView.SetAllowRightButtonZoom(true);
		m_SceneView.SetModel(&m_SceneModel);
		m_SceneView.SetCameraDistFromOrigin(96.0f);
		AddItem(m_SceneView);
	}

	void AddBottomControls()
	{
		const int areaTop = GetControlAreaTop();
		const int col2Left = LEFT_MARGIN + GetFirstColumnWidth() + PADDING;
		const int controlCol2Left = col2Left + (GetSecondColumnWidth() / 2) + BOTTOM_CONTROL_AREA_PADDING;

		int rowOffset = 0;

		m_CheckEnableModelBBox.SetCoord(col2Left, areaTop + rowOffset);
		m_CheckEnableModelBBox.bChecked = m_SceneView.GetDrawModelBoundingBoxes();
		m_CheckEnableModelBBox.onChanged = VoidCb(&CMenuModelViewer::HandleModelBBoxCheckBoxChanged);
		m_CheckEnableModelBBox.SetNameAndStatus(
			L("Model Bounds"),
			L("Enable or disable drawing bounds of overall model"));

		AddItem(m_CheckEnableModelBBox);

		m_CheckEnableOriginMarker.SetCoord(controlCol2Left, areaTop + rowOffset);
		m_CheckEnableOriginMarker.bChecked = m_SceneView.GetDrawOriginMarker();
		m_CheckEnableOriginMarker.onChanged = VoidCb(&CMenuModelViewer::HandleOriginMarkerCheckBoxChanged);
		m_CheckEnableOriginMarker.SetNameAndStatus(
			L("Origin Marker"),
			L("Enable or disable drawing marker at model origin"));

		AddItem(m_CheckEnableOriginMarker);

		rowOffset += BOTTOM_CONTROL_ROW_HEIGHT;

		m_CheckEnableSequenceBBox.SetCoord(col2Left, areaTop + rowOffset);
		m_CheckEnableSequenceBBox.bChecked = m_SceneView.GetDrawSequenceBoundingBoxes();
		m_CheckEnableSequenceBBox.onChanged = VoidCb(&CMenuModelViewer::HandleSequenceBBoxCheckBoxChanged);
		m_CheckEnableSequenceBBox.SetNameAndStatus(
			L("Sequence Bounds"),
			L("Enable or disable drawing bounds of current animation sequence"));

		AddItem(m_CheckEnableSequenceBBox);
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
		if ( !m_MainStudioModel )
		{
			return;
		}

		EngFuncs::SetModel(m_MainStudioModel, path);
		m_SceneView.ResetCamera();
		m_SceneView.SetCameraDistFromOrigin(64.0f);

		int numSequences = EngFuncs::GetModelSequenceCount(m_MainStudioModel);
		m_SequenceModel.Purge();

		for ( int index = 0; index < numSequences; ++index )
		{
			m_SequenceModel.AddToTail(CUtlString(EngFuncs::GetModelSequenceName(m_MainStudioModel, index)));
		}
	}

	void HandleSequenceChanged()
	{
		if ( !m_MainStudioModel )
		{
			return;
		}

		m_MainStudioModel->curstate.sequence = m_SequenceTable.GetCurrentIndex();
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

	void HandleOriginMarkerCheckBoxChanged()
	{
		m_SceneView.SetDrawOriginMarker(m_CheckEnableOriginMarker.bChecked);
	}

	void HandleModelBBoxCheckBoxChanged()
	{
		m_SceneView.SetDrawModelBoundingBoxes(m_CheckEnableModelBBox.bChecked);
	}

	void HandleSequenceBBoxCheckBoxChanged()
	{
		m_SceneView.SetDrawSequenceBoundingBoxes(m_CheckEnableSequenceBBox.bChecked);
	}

	static int GetTotalColumnWidth()
	{
		return static_cast<int>(ScreenWidth / uiStatic.scaleX) - LEFT_MARGIN - RIGHT_MARGIN - PADDING;
	}

	static int GetFirstColumnWidth()
	{
		return static_cast<int>(static_cast<float>(GetTotalColumnWidth()) * COL1_FACTOR);
	}

	static int GetSecondColumnWidth()
	{
		return static_cast<int>(static_cast<float>(GetTotalColumnWidth()) * COL2_FACTOR);
	}

	static int GetViewAreaHeight()
	{
		return static_cast<int>(ScreenHeight / uiStatic.scaleY) - TOP_MARGIN - BOTTOM_MARGIN -
			BOTTOM_CONTROL_AREA_HEIGHT;
	}

	static int GetControlAreaTop()
	{
		return static_cast<int>(ScreenHeight / uiStatic.scaleY) - BOTTOM_MARGIN - BOTTOM_CONTROL_AREA_HEIGHT +
			BOTTOM_CONTROL_AREA_PADDING;
	}

	CStudioSceneModel m_SceneModel;
	CStringVectorModel m_SequenceModel;

	CMenuDeveloperStudioSceneView m_SceneView;
	CMenuTable m_SequenceTable;
	CMenuCheckBox m_CheckEnableOriginMarker;
	CMenuCheckBox m_CheckEnableModelBBox;
	CMenuCheckBox m_CheckEnableSequenceBBox;

	cl_entity_t* m_MainStudioModel = nullptr;
};

ADD_MENU(menu_modelviewer, CMenuModelViewer, UI_ModelViewer_Menu);
