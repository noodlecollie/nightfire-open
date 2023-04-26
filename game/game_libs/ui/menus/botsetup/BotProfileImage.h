#pragma once

#include "BaseItem.h"
#include "utlstring.h"

class CBotProfileImage : public CMenuBaseItem
{
public:
	static constexpr int IMAGE_WIDTH = 208;
	static constexpr int IMAGE_HEIGHT = 256;

	CBotProfileImage();

	virtual void Init() override;
	virtual void Draw() override;

	void SetImage(const CUtlString& botSkin);

private:
	HIMAGE m_hImage = 0;
};
