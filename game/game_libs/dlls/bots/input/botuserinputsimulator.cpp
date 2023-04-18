#include "botuserinputsimulator.h"
#include "standard_includes.h"

CBotUserInputSimulator::CBotUserInputSimulator()
{
	m_iCurrentButtons = 0;

	for ( uint32_t index = 0; index < MAX_FLAGS; ++index )
	{
		m_flButtonReleaseTimes[index] = 0;
	}
}

void CBotUserInputSimulator::Think()
{
	UpdateCurrentButtons();
}

int CBotUserInputSimulator::GetButtons() const
{
	return m_iCurrentButtons;
}

void CBotUserInputSimulator::SetButtons(int flags, float pressDuration)
{
	for ( uint32_t index = 0; index < MAX_FLAGS; ++index )
	{
		if ( flags & (1 << index) )
		{
			SetIndividualButton(index, pressDuration);
		}
	}
}

void CBotUserInputSimulator::SetIndividualButton(uint32_t index, float pressDuration)
{
	ASSERT(index < MAX_FLAGS);

	if ( pressDuration <= 0.0f )
	{
		m_flButtonReleaseTimes[index] = 0.0f;
	}
	else if ( pressDuration == INFINITE_DURATION )
	{
		m_flButtonReleaseTimes[index] = INFINITE_DURATION;
	}
	else
	{
		m_flButtonReleaseTimes[index] = gpGlobals->time + pressDuration;
	}
}

void CBotUserInputSimulator::UpdateCurrentButtons()
{
	m_iCurrentButtons = 0;

	// Exceedingly unlikely to happen:
	if ( gpGlobals->time <= 0.0f )
	{
		return;
	}

	for ( uint32_t index = 0; index < MAX_FLAGS; ++index )
	{
		float releaseTime = m_flButtonReleaseTimes[index];

		if ( releaseTime > gpGlobals->time )
		{
			m_iCurrentButtons |= 1 << index;
		}
	}
}
