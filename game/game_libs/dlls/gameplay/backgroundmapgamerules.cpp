#include "gameplay/backgroundmapgamerules.h"
#include "gameplay/gameplaySystems.h"
#include "gameplay/gameplaySystemsBase.h"

// MENUMAP TODO: May need to be applied on think? Not sure why
void CBackgroundMapGameRules::PlayerSpawn(CBasePlayer* pPlayer)
{
	// Completely override this and do not call into the base implementation.

	pPlayer->pev->movetype = MOVETYPE_NOCLIP;
	pPlayer->pev->solid = SOLID_NOT;
	pPlayer->pev->effects |= EF_NODRAW;

	CGameplaySystemsBase* gps = GameplaySystems::GetBase();
	CObserverPointList& observerPoints = gps->ObserverPointList();

	edict_t* observerPoint = observerPoints.GetNextObserverPointWithIndex(0);

	if ( observerPoint )
	{
		g_engfuncs.pfnSetView(pPlayer->edict(), observerPoint);
		pPlayer->EnableControl(false);
	}
}
