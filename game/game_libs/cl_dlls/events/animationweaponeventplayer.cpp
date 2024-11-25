#include "animationweaponeventplayer.h"
#include "eventscripts.h"

void AnimationWeaponEventPlayer::EventStart()
{
	if ( EV_IsLocal(m_iEntIndex) )
	{
		AnimateViewModel();
	}

	PlayFireSound();
}
