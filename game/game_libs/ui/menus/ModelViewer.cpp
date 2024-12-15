#include "Framework.h"
#include "StudioSceneModel.h"
#include "DeveloperStudioSceneView.h"
#include "Table.h"
#include "StringVectorModel.h"
#include "CheckBox.h"
#include "Field.h"
#include "utlstring.h"

static constexpr int TOP_MARGIN = 50;
static constexpr int LEFT_MARGIN = 300;
static constexpr int RIGHT_MARGIN = 50;
static constexpr int BOTTOM_MARGIN = 50;
static constexpr int VIEW_PADDING = 30;
static constexpr int INTER_AREA_PADDING = 10;

static constexpr int NEG_VIEW_RIGHT = -1 * RIGHT_MARGIN;

static constexpr int CTRLAREA_NUM_ROWS = 3;
static constexpr int CTRLAREA_ROW_HEIGHT = 40;
static constexpr int CTRLAREA_ROW_PADDING = 8;
static constexpr int CTRLAREA_HEIGHT =
	(CTRLAREA_NUM_ROWS * CTRLAREA_ROW_HEIGHT) + ((CTRLAREA_NUM_ROWS + 1) * CTRLAREA_ROW_PADDING);
static constexpr int NEG_CTRLAREA_TOP = -1 * (BOTTOM_MARGIN + CTRLAREA_HEIGHT);
static constexpr int NEG_CTRLAREA_ROW_START = NEG_CTRLAREA_TOP + CTRLAREA_ROW_PADDING;

static constexpr int NEG_VIEW_BOTTOM = NEG_CTRLAREA_TOP - INTER_AREA_PADDING;

static constexpr float VIEW_COL1_FACTOR = 0.4f;
static constexpr float VIEW_COL2_FACTOR = 1.0f - VIEW_COL1_FACTOR;

static constexpr int FIELD_PADDING = 15;

class CMenuModelViewer : public CMenuFramework
{
public:
	CMenuModelViewer() :
		CMenuFramework("CMenuModelViewer")
	{
	}

private:
	void _Init() override
	{
		banner.SetPicture("gfx/shell/head_blank");

		AddItem(background);
		AddItem(banner);

		AddButton(L("Select"), L("Select model"), PC_CUSTOMIZE, VoidCb(&CMenuModelViewer::SelectModel));
		AddButton(L("Back"), L("Return to main menu"), PC_DONE, VoidCb(&CMenuModelViewer::Hide));

		m_SceneModel.Clear();
		m_MainStudioModel = m_SceneModel.AddEntData();
		ResetCameraToDefaultPosition();

		AddMainViews();
		AddFineCameraControls();
		AddViewToggles();
	}

	void Draw() override
	{
		if ( m_PendingCameraParamSet )
		{
			SetCameraValuesFromUI();
			m_PendingCameraParamSet = false;
		}

		if ( !m_CameraParamsBeingEdited )
		{
			UpdateFineCameraControlValues();
		}

		UpdateAnimationTime(m_MainStudioModel);
		CMenuFramework::Draw();
	}

	void AddMainViews()
	{
		const int col1Width = GetFirstColumnWidth();
		const int col2Left = LEFT_MARGIN + col1Width + VIEW_PADDING;
		const int tableHeight = (GetViewAreaHeight() - VIEW_PADDING) / 2;

		m_SequenceTable.SetRect(LEFT_MARGIN, TOP_MARGIN, GetFirstColumnWidth(), tableHeight);
		m_SequenceTable.SetModel(&m_SequenceModel);
		m_SequenceTable.onChanged = VoidCb(&CMenuModelViewer::HandleSequenceChanged);
		m_SequenceTable.SetNameAndStatus(L("Sequences"), L("Animation sequences in this model"));
		AddItem(m_SequenceTable);

		const int halfColWidth = (col1Width / 2) - INTER_AREA_PADDING;

		m_SkinTable.SetRect(LEFT_MARGIN, TOP_MARGIN + tableHeight + VIEW_PADDING, halfColWidth, tableHeight);
		m_SkinTable.SetModel(&m_SkinModel);
		m_SkinTable.onChanged = VoidCb(&CMenuModelViewer::HandleSkinChanged);
		m_SkinTable.SetNameAndStatus(L("Skins"), L("Texture skins in this model"));
		AddItem(m_SkinTable);

		m_BodyTable.SetRect(
			LEFT_MARGIN + halfColWidth + INTER_AREA_PADDING,
			TOP_MARGIN + tableHeight + VIEW_PADDING,
			halfColWidth,
			tableHeight);
		m_BodyTable.SetModel(&m_BodyModel);
		m_BodyTable.onChanged = VoidCb(&CMenuModelViewer::HandleBodyChanged);
		m_BodyTable.SetNameAndStatus(L("Body Groups"), L("Body groups in this model"));
		AddItem(m_BodyTable);

		m_SceneView.SetRect(col2Left, TOP_MARGIN, NEG_VIEW_RIGHT, NEG_VIEW_BOTTOM);
		m_SceneView.SetAllowPitchRotation(true);
		m_SceneView.SetAllowRightButtonZoom(true);
		m_SceneView.SetModel(&m_SceneModel);
		m_SceneView.SetCameraDistFromCentre(96.0f);
		AddItem(m_SceneView);
	}

	void AddFineCameraControls()
	{
		const int col1Width = GetFirstColumnWidth();
		const int fieldWidth = (col1Width - (2 * FIELD_PADDING)) / 3;

		int rowOffset = 0;

		m_FieldCameraPosX.SetCoord(LEFT_MARGIN, NEG_CTRLAREA_ROW_START + rowOffset);
		m_FieldCameraPosX.size.w = fieldWidth;
		SetCommonFineUIParams(m_FieldCameraPosX);
		AddItem(m_FieldCameraPosX);

		m_FieldCameraPosY.SetCoord(
			m_FieldCameraPosX.pos.x + m_FieldCameraPosX.size.w + FIELD_PADDING,
			NEG_CTRLAREA_ROW_START + rowOffset);
		m_FieldCameraPosY.size.w = fieldWidth;
		SetCommonFineUIParams(m_FieldCameraPosY);
		AddItem(m_FieldCameraPosY);

		m_FieldCameraPosZ.SetCoord(
			m_FieldCameraPosY.pos.x + m_FieldCameraPosY.size.w + FIELD_PADDING,
			NEG_CTRLAREA_ROW_START + rowOffset);
		m_FieldCameraPosZ.size.w = fieldWidth;
		SetCommonFineUIParams(m_FieldCameraPosZ);
		AddItem(m_FieldCameraPosZ);

		rowOffset += CTRLAREA_ROW_HEIGHT + CTRLAREA_ROW_PADDING;

		m_FieldCameraPitch.SetCoord(LEFT_MARGIN, NEG_CTRLAREA_ROW_START + rowOffset);
		m_FieldCameraPitch.size.w = fieldWidth;
		SetCommonFineUIParams(m_FieldCameraPitch);
		AddItem(m_FieldCameraPitch);

		m_FieldCameraYaw.SetCoord(
			m_FieldCameraPitch.pos.x + m_FieldCameraPitch.size.w + FIELD_PADDING,
			NEG_CTRLAREA_ROW_START + rowOffset);
		m_FieldCameraYaw.size.w = fieldWidth;
		SetCommonFineUIParams(m_FieldCameraYaw);
		AddItem(m_FieldCameraYaw);

		m_FieldCameraDist.SetCoord(
			m_FieldCameraYaw.pos.x + m_FieldCameraYaw.size.w + FIELD_PADDING,
			NEG_CTRLAREA_ROW_START + rowOffset);
		m_FieldCameraDist.size.w = fieldWidth;
		SetCommonFineUIParams(m_FieldCameraDist);
		AddItem(m_FieldCameraDist);
	}

	void AddViewToggles()
	{
		const int col2Left = LEFT_MARGIN + GetFirstColumnWidth() + VIEW_PADDING;
		const int controlCol2Left = col2Left + (GetSecondColumnWidth() / 2) + CTRLAREA_ROW_PADDING;

		int rowOffset = 0;

		m_CheckEnableModelBBox.SetCoord(col2Left, NEG_CTRLAREA_ROW_START + rowOffset);
		m_CheckEnableModelBBox.bChecked = m_SceneView.GetDrawModelBoundingBoxes();
		m_CheckEnableModelBBox.onChanged = VoidCb(&CMenuModelViewer::HandleModelBBoxCheckBoxChanged);
		m_CheckEnableModelBBox.SetNameAndStatus(
			L("Model Bounds"),
			L("Enable or disable drawing bounds of overall model"));

		AddItem(m_CheckEnableModelBBox);

		m_CheckEnableOriginMarker.SetCoord(controlCol2Left, NEG_CTRLAREA_ROW_START + rowOffset);
		m_CheckEnableOriginMarker.bChecked = m_SceneView.GetDrawOriginMarker();
		m_CheckEnableOriginMarker.onChanged = VoidCb(&CMenuModelViewer::HandleOriginMarkerCheckBoxChanged);
		m_CheckEnableOriginMarker.SetNameAndStatus(
			L("Origin Marker"),
			L("Enable or disable drawing marker at model origin"));

		AddItem(m_CheckEnableOriginMarker);

		rowOffset += CTRLAREA_ROW_HEIGHT + CTRLAREA_ROW_PADDING;

		m_CheckEnableSequenceBBox.SetCoord(col2Left, NEG_CTRLAREA_ROW_START + rowOffset);
		m_CheckEnableSequenceBBox.bChecked = m_SceneView.GetDrawSequenceBoundingBoxes();
		m_CheckEnableSequenceBBox.onChanged = VoidCb(&CMenuModelViewer::HandleSequenceBBoxCheckBoxChanged);
		m_CheckEnableSequenceBBox.SetNameAndStatus(
			L("Sequence Bounds"),
			L("Enable or disable drawing bounds of current animation sequence"));

		AddItem(m_CheckEnableSequenceBBox);

		m_CheckEnableEyePositionMarker.SetCoord(controlCol2Left, NEG_CTRLAREA_ROW_START + rowOffset);
		m_CheckEnableEyePositionMarker.bChecked = m_SceneView.GetDrawEyePositionMarker();
		m_CheckEnableEyePositionMarker.onChanged = VoidCb(&CMenuModelViewer::HandleEyePositionMarkerCheckBoxChanged);
		m_CheckEnableEyePositionMarker.SetNameAndStatus(
			L("Eye Position Marker"),
			L("Enable or disable drawing marker at model eye position"));

		AddItem(m_CheckEnableEyePositionMarker);

		rowOffset += CTRLAREA_ROW_HEIGHT + CTRLAREA_ROW_PADDING;

		m_CheckCentreAtEyes.SetCoord(col2Left, NEG_CTRLAREA_ROW_START + rowOffset);
		m_CheckCentreAtEyes.bChecked = m_SceneView.GetDrawEyePositionMarker();
		m_CheckCentreAtEyes.onChanged = VoidCb(&CMenuModelViewer::HandleCentreAtEyesChanged);
		m_CheckCentreAtEyes.SetNameAndStatus(
			L("Centre At Eyes"),
			L("Centres the camera around the model eye position"));

		AddItem(m_CheckCentreAtEyes);
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

		ResetCameraToDefaultPosition();

		int numSequences = EngFuncs::GetModelSequenceCount(m_MainStudioModel);
		m_SequenceModel.Purge();

		for ( int index = 0; index < numSequences; ++index )
		{
			m_SequenceModel.AddToTail(CUtlString(EngFuncs::GetModelSequenceName(m_MainStudioModel, index)));
		}

		int numSkins = EngFuncs::GetModelSkinCount(m_MainStudioModel);
		m_SkinModel.Purge();

		for ( int index = 0; index < numSkins; ++index )
		{
			CUtlString indexStr;
			indexStr.Format("Skin %d", index);
			m_SkinModel.AddToTail(indexStr);
		}

		int numBodyVariations = EngFuncs::GetModelBodyVariationCount(m_MainStudioModel);
		m_BodyModel.Purge();

		for ( int index = 0; index < numBodyVariations; ++index )
		{
			CUtlString indexStr;
			indexStr.Format("Body %d", index);
			m_BodyModel.AddToTail(indexStr);
		}

		m_MainStudioModel->curstate.animtime = gpGlobals->time;
	}

	void HandleSequenceChanged()
	{
		if ( !m_MainStudioModel )
		{
			return;
		}

		m_MainStudioModel->curstate.sequence = m_SequenceTable.GetCurrentIndex();
		m_MainStudioModel->curstate.animtime = gpGlobals->time;
	}

	void HandleSkinChanged()
	{
		if ( !m_MainStudioModel )
		{
			return;
		}

		m_MainStudioModel->curstate.skin = static_cast<short>(m_SkinTable.GetCurrentIndex());
	}

	void HandleBodyChanged()
	{
		if ( !m_MainStudioModel )
		{
			return;
		}

		m_MainStudioModel->curstate.body = static_cast<short>(m_BodyTable.GetCurrentIndex());
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

	// Update animtime (the time at which the animation began playing)
	// so that animations will loop properly.
	void UpdateAnimationTime(cl_entity_t* ent)
	{
		if ( !ent || !ent->model )
		{
			return;
		}

		float duration = EngFuncs::GetModelSequenceDuration(ent, ent->curstate.sequence);

		if ( duration <= 0.0f || ent->curstate.framerate == 0.0f )
		{
			return;
		}

		duration /= ent->curstate.framerate;

		float timeDiff = gpGlobals->time - ent->curstate.animtime;

		if ( timeDiff >= duration )
		{
			timeDiff = fmodf(timeDiff, duration);
			ent->curstate.animtime = gpGlobals->time + timeDiff;
		}
	}

	void UpdateFineCameraControlValues()
	{
		static constexpr const char* const FLOAT_FORMAT = "%.02f";

		const float* cameraCentre = m_SceneView.GetCameraCentre();

		CUtlString value;

		value.Format(FLOAT_FORMAT, cameraCentre[0]);
		m_FieldCameraPosX.SetBuffer(value.Get());

		value.Format(FLOAT_FORMAT, cameraCentre[1]);
		m_FieldCameraPosY.SetBuffer(value.Get());

		value.Format(FLOAT_FORMAT, cameraCentre[2]);
		m_FieldCameraPosZ.SetBuffer(value.Get());

		value.Format(FLOAT_FORMAT, m_SceneView.GetCameraPitch());
		m_FieldCameraPitch.SetBuffer(value.Get());

		value.Format(FLOAT_FORMAT, m_SceneView.GetCameraYaw());
		m_FieldCameraYaw.SetBuffer(value.Get());

		value.Format(FLOAT_FORMAT, m_SceneView.GetCameraDistFromCentre());
		m_FieldCameraDist.SetBuffer(value.Get());
	}

	void SetCameraValuesFromUI()
	{
		vec3_t position = {0.0f, 0.0f, 0.0f};

		position[0] = strtof(m_FieldCameraPosX.GetBuffer(), nullptr);
		position[1] = strtof(m_FieldCameraPosY.GetBuffer(), nullptr);
		position[2] = strtof(m_FieldCameraPosZ.GetBuffer(), nullptr);

		float pitch = strtof(m_FieldCameraPitch.GetBuffer(), nullptr);
		float yaw = strtof(m_FieldCameraYaw.GetBuffer(), nullptr);
		float dist = strtof(m_FieldCameraDist.GetBuffer(), nullptr);

		m_SceneView.SetCameraCentre(position);
		m_SceneView.SetCameraPitch(pitch);
		m_SceneView.SetCameraYaw(yaw);
		m_SceneView.SetCameraDistFromCentre(dist);
	}

	void HandleOriginMarkerCheckBoxChanged()
	{
		m_SceneView.SetDrawOriginMarker(m_CheckEnableOriginMarker.bChecked);
	}

	void HandleEyePositionMarkerCheckBoxChanged()
	{
		m_SceneView.SetDrawEyePositionMarker(m_CheckEnableEyePositionMarker.bChecked);
	}

	void HandleModelBBoxCheckBoxChanged()
	{
		m_SceneView.SetDrawModelBoundingBoxes(m_CheckEnableModelBBox.bChecked);
	}

	void HandleSequenceBBoxCheckBoxChanged()
	{
		m_SceneView.SetDrawSequenceBoundingBoxes(m_CheckEnableSequenceBBox.bChecked);
	}

	void HandleCentreAtEyesChanged()
	{
		ComputeAndSetCameraPosition();

		if ( m_CheckCentreAtEyes.bChecked )
		{
			m_SceneView.SetCameraPitch(0.0f);
			m_SceneView.SetCameraYaw(180.0f);
			m_SceneView.SetCameraDistFromCentre(16.0f);
		}
		else
		{
			m_SceneView.SetCameraDistFromCentre(64.0f);
		}
	}

	void HandleCameraFineUIGotFocus()
	{
		m_CameraParamsBeingEdited = true;
	}

	void HandleCameraFineUILostFocus()
	{
		m_CameraParamsBeingEdited = false;
	}

	void HandleCameraFineValueChanged()
	{
		m_PendingCameraParamSet = true;
	}

	void ComputeAndSetCameraPosition()
	{
		vec3_t cameraCentre = {0.0f, 0.0f, 0.0f};

		if ( m_MainStudioModel && m_CheckCentreAtEyes.bChecked )
		{
			EngFuncs::GetModelEyePosition(m_MainStudioModel, cameraCentre);
		}

		m_SceneView.SetCameraCentre(cameraCentre);
	}

	void ResetCameraToDefaultPosition()
	{
		m_SceneView.ResetCamera();
		ComputeAndSetCameraPosition();
		m_SceneView.SetCameraYaw(180.0f);
		m_SceneView.SetCameraDistFromCentre(64.0f);
	}

	void ResetCameraRotation()
	{
		m_SceneView.SetCameraYaw(180.0f);
	}

	static void SetCommonFineUIParams(CMenuField& field)
	{
		field.eTextAlignment = QM_CENTER;
		field.onChanged = VoidCb(&CMenuModelViewer::HandleCameraFineValueChanged);
		field.onGotFocus = VoidCb(&CMenuModelViewer::HandleCameraFineUIGotFocus);
		field.onLostFocus = VoidCb(&CMenuModelViewer::HandleCameraFineUILostFocus);
	}

	static int GetTotalColumnWidth()
	{
		return static_cast<int>(ScreenWidth / uiStatic.scaleX) - LEFT_MARGIN - RIGHT_MARGIN - VIEW_PADDING;
	}

	static int GetFirstColumnWidth()
	{
		return static_cast<int>(static_cast<float>(GetTotalColumnWidth()) * VIEW_COL1_FACTOR);
	}

	static int GetSecondColumnWidth()
	{
		return static_cast<int>(static_cast<float>(GetTotalColumnWidth()) * VIEW_COL2_FACTOR);
	}

	static int GetViewAreaHeight()
	{
		return static_cast<int>(ScreenHeight / uiStatic.scaleY) - TOP_MARGIN + NEG_VIEW_BOTTOM;
	}

	CStudioSceneModel m_SceneModel;
	CStringVectorModel m_SequenceModel;
	CStringVectorModel m_SkinModel;
	CStringVectorModel m_BodyModel;

	CMenuDeveloperStudioSceneView m_SceneView;
	CMenuTable m_SequenceTable;
	CMenuTable m_SkinTable;
	CMenuTable m_BodyTable;

	CMenuField m_FieldCameraPosX;
	CMenuField m_FieldCameraPosY;
	CMenuField m_FieldCameraPosZ;
	CMenuField m_FieldCameraPitch;
	CMenuField m_FieldCameraYaw;
	CMenuField m_FieldCameraDist;
	bool m_PendingCameraParamSet = false;
	bool m_CameraParamsBeingEdited = false;

	CMenuCheckBox m_CheckEnableOriginMarker;
	CMenuCheckBox m_CheckEnableEyePositionMarker;
	CMenuCheckBox m_CheckEnableModelBBox;
	CMenuCheckBox m_CheckEnableSequenceBBox;
	CMenuCheckBox m_CheckCentreAtEyes;

	cl_entity_t* m_MainStudioModel = nullptr;
};

ADD_MENU(menu_modelviewer, CMenuModelViewer, UI_ModelViewer_Menu);
