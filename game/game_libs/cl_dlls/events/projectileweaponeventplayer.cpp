#include "projectileweaponeventplayer.h"
#include "eventscripts.h"
#include "cl_dll.h"
#include "EnginePublicAPI/event_api.h"

void ProjectileWeaponEventPlayer::EventStart()
{
	if ( EV_IsLocal(m_iEntIndex) )
	{
		EV_MuzzleFlash();
		AnimateViewModel();
	}

	PlayFireSound();
}

bool ProjectileWeaponEventPlayer::Initialise()
{
	if ( !BaseWeaponEventPlayer::Initialise() )
	{
		return false;
	}

	m_pProjectileAttack = dynamic_cast<const WeaponAtts::WAProjectileAttack*>(m_pAttackMode);

	if ( !m_pProjectileAttack )
	{
		return false;
	}

	m_iShellModelIndex = gEngfuncs.pEventAPI->EV_FindModelIndex(m_pProjectileAttack->ShellModelName);
	return true;
}
