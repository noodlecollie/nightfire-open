#include "UIDebug.h"
#include "PlatformLib/String.h"
#include "udll_int.h"

#ifdef _DEBUG
void UIDBG_AssertFunction(bool fExpr, const char* szExpr, const char* szFile, int szLine, const char* szMessage)
{
	if ( fExpr )
	{
		return;
	}

	char szOut[512];

	if ( szMessage != NULL )
	{
		PlatformLib_SNPrintF(
			szOut,
			sizeof(szOut),
			"ASSERT FAILED:\n %s \n(%s@%d)\n%s",
			szExpr,
			szFile,
			szLine,
			szMessage
		);
	}
	else
	{
		PlatformLib_SNPrintF(szOut, sizeof(szOut), "ASSERT FAILED:\n %s \n(%s@%d)", szExpr, szFile, szLine);
	}

	gEngfuncs.pfnHostError("%s", szOut);
}
#endif  // DEBUG
