#include <algorithm>
#include "Framework.h"

class CMenuAcknowledgements : public CMenuFramework
{
public:
	CMenuAcknowledgements() :
		CMenuFramework("Acknowledgements")
	{
	}

	void Draw() override;

protected:
	void _Init() override;
};

void CMenuAcknowledgements::_Init()
{
	AddItem(background);

	AddButton(L("Cancel"), nullptr, PC_CANCEL, VoidCb(&CMenuAcknowledgements::Hide));
}

void CMenuAcknowledgements::Draw()
{
	CMenuFramework::Draw();

	const int leftMargin = 200;

	int yOffset = 200;
	auto lineY = [&yOffset]() -> int
	{
		int offset = yOffset;
		yOffset += 20;
		return offset;
	};

	EngFuncs::DrawSetTextColor(255, 255, 255);

	EngFuncs::DrawConsoleString(leftMargin, lineY(), "Many thanks to:");
	lineY();
	EngFuncs::DrawConsoleString(leftMargin, lineY(), "Gearbox for the original 007 Nightfire PC game.");
	EngFuncs::DrawConsoleString(
		leftMargin,
		lineY(),
		"The FWGS team for the Xash3D engine and Xash3D version of Half Life SDK."
	);
	EngFuncs::DrawConsoleString(
		leftMargin,
		lineY(),
		"Boris Borisovich Godin for the Botrix multiplayer bot plugin for HL2DM."
	);
	EngFuncs::DrawConsoleString(leftMargin, lineY(), "Developers of STB and rapidJSON libraries.");
	EngFuncs::DrawConsoleString(leftMargin, lineY(), "Contributors to XeNTaX game reverse-engineering forums.");
	EngFuncs::DrawConsoleString(leftMargin, lineY(), "ZapSplat.com for various free sound effects.");
	EngFuncs::DrawConsoleString(
		leftMargin,
		lineY(),
		"Various Nightfire modders over the years whose tools and reverse-engineering"
	);
	EngFuncs::DrawConsoleString(leftMargin + 10, lineY(), "have proven invaluable when developing this mod");
	EngFuncs::DrawConsoleString(leftMargin + 10, lineY(), "(specifically Tres, 009, Ford, UltimateSniper).");
}

ADD_MENU(menu_acknowledgements, CMenuAcknowledgements, UI_Acknowledgements_Menu);
