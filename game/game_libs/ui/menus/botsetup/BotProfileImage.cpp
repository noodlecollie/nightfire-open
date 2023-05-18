#include "BotProfileImage.h"

CBotProfileImage::CBotProfileImage() :
	CMenuBaseItem()
{
}

void CBotProfileImage::Init()
{
	CMenuBaseItem::Init();

	SetSize(IMAGE_WIDTH, IMAGE_HEIGHT);
}

void CBotProfileImage::Draw()
{
	UI_FillRect(m_scPos, m_scSize, uiPromptBgColor);
	// UI_DrawString(font, m_scPos, m_scSize, L("Bot preview goes here"), colorBase, m_scChSize, QM_CENTER, ETF_SHADOW);

	if ( m_hImage )
	{
		EngFuncs::PIC_Set(m_hImage, 255, 255, 255, 255);
		EngFuncs::PIC_Draw(m_scPos, m_scSize);
	}
}

void CBotProfileImage::SetImage(const CUtlString& botSkin)
{
	if ( botSkin.IsEmpty() )
	{
		m_hImage = 0;
		return;
	}

	CUtlString pathToImage;
	pathToImage.AppendFormat("models/player/%s/%s.bmp", botSkin.String(), botSkin.String());

	m_hImage = EngFuncs::PIC_Load(pathToImage.String(), 0);
}
