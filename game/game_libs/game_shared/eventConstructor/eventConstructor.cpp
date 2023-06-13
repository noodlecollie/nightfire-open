#include "eventConstructor.h"
#include "enginecallback.h"

namespace EventConstructor
{
	CEventConstructor::CEventConstructor()
	{
		Clear();
	}

	void CEventConstructor::Clear()
	{
		memset(&m_Args, 0, sizeof(m_Args));

		m_Args.vec3Origin = g_vecZero;
		m_Args.vec3Angles = g_vecZero;
		m_Args.vec3param1 = g_vecZero;
	}

	void CEventConstructor::Send()
	{
		PLAYBACK_EVENT_FULL(&m_Args);
	}

	event_fire_args_t& CEventConstructor::Args()
	{
		return m_Args;
	}

	const event_fire_args_t& CEventConstructor::Args() const
	{
		return m_Args;
	}

	void CEventConstructor::InitSimple(int flags, const edict_t* edict, int eventIndex, float delay)
	{
		Clear();

		m_Args.flags = flags;
		m_Args.invoker = edict;
		m_Args.eventIndex = static_cast<unsigned short>(eventIndex);
		m_Args.delay = delay;
	}
}  // namespace EventConstructor
