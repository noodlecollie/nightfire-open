#include "meleeweaponeventplayer.h"
#include "eventscripts.h"

void MeleeWeaponEventPlayer::EventStart()
{
	if ( EV_IsLocal(m_iEntIndex) )
	{
		AnimateViewModel();
	}

	PlayFireSound();
}
