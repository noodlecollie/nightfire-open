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
#include "Action.h"
#include "Table.h"
#include "StringArrayModel.h"
#include "PicButton.h"
#include "StringVectorModel.h"

#define ART_BANNER "gfx/shell/head_touchoptions"
#define ART_GAMMA "gfx/shell/gamma"

class CMenuFileDialog : public CMenuFramework
{
public:
	CMenuFileDialog() :
		CMenuFramework("CMenuFileDialog")
	{
	}

private:
	class CFileListModel : public CStringVectorModel
	{
	public:
		void Update() override;
	};

	class CPreview : public CMenuAction
	{
	public:
		void Draw() override;
		HIMAGE image;
	};

	void _Init(void) override;
	void _VidInit(void) override;
	void SaveAndPopMenu() override;
	void RejectChanges();
	void ApplyChanges(const char* fileName);
	void UpdateExtra();

	CFileListModel model;
	CMenuTable fileList;
	CPreview preview;
};

void CMenuFileDialog::CPreview::Draw()
{
	UI_FillRect(m_scPos.x - 2, m_scPos.y - 2, m_scSize.w + 4, m_scSize.h + 4, 0xFFC0C0C0);
	UI_FillRect(m_scPos.x, m_scPos.y, m_scSize.w, m_scSize.h, 0xFF808080);
	EngFuncs::PIC_Set(image, 255, 255, 255, 255);
	EngFuncs::PIC_DrawTrans(m_scPos, m_scSize);
}

void CMenuFileDialog::CFileListModel::Update(void)
{
	FileDialogGlobals& globalData = FileDialogGlobals::GlobalData();

	Purge();

	for ( size_t patternIndex = 0; patternIndex < globalData.PatternCount(); ++patternIndex )
	{
		const char* pattern = globalData.GetPattern(patternIndex);

		int numFiles = 0;
		const char* const* fileNames = EngFuncs::GetFilesList(pattern, &numFiles, TRUE);

		if ( numFiles > 0 )
		{
			for ( int fileIndex = 0; fileIndex < numFiles; ++fileIndex )
			{
				AddToTail(CUtlString(fileNames[fileIndex]));
			}
		}
	}
}

void CMenuFileDialog::ApplyChanges(const char* fileName)
{
	FileDialogGlobals& globalData = FileDialogGlobals::GlobalData();
	globalData.SetResultAndCallCallback(fileName);
}

void CMenuFileDialog::RejectChanges()
{
	ApplyChanges("");
	Hide();
}

void CMenuFileDialog::SaveAndPopMenu()
{
	const char* fileName = model.GetText(fileList.GetCurrentIndex());
	ApplyChanges(fileName);
	Hide();
}

void CMenuFileDialog::UpdateExtra()
{
	const char* fileName = model.GetText(fileList.GetCurrentIndex());

	if ( FileDialogGlobals::GlobalData().ResultHasPreview() )
	{
		preview.image = EngFuncs::PIC_Load(fileName);
	}
}

/*
=================
UI_FileDialog_Init
=================
*/
void CMenuFileDialog::_Init(void)
{
	// banner.SetPicture( ART_BANNER );

	fileList.iFlags |= QMF_DROPSHADOW;
	fileList.SetModel(&model);
	fileList.onChanged = VoidCb(&CMenuFileDialog::UpdateExtra);
	fileList.SetRect(360, 230, -20, 465);
	UpdateExtra();

	preview.SetRect(72, 380, 196, 196);

	AddItem(background);
	// AddItem( banner );
	AddButton(L("Done"), L("Use selected file"), PC_DONE, VoidCb(&CMenuFileDialog::SaveAndPopMenu));
	AddButton(L("GameUI_Cancel"), L("Cancel file selection"), PC_CANCEL, VoidCb(&CMenuFileDialog::RejectChanges));
	AddItem(preview);
	AddItem(fileList);
}

void CMenuFileDialog::_VidInit()
{
	preview.SetVisibility(FileDialogGlobals::GlobalData().ResultHasPreview());
}

ADD_MENU(menu_filedialog, CMenuFileDialog, UI_FileDialog_Menu);
