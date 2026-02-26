#include "rmlui/EventListenerImpl.h"
#include "udll_int.h"
#include <algorithm>

EventListenerImpl::EventListenerImpl(Rml::String strategy, Rml::String args) :
	m_Strategy(std::move(strategy)),
	m_Args(std::move(args))
{
}

void EventListenerImpl::ProcessEvent(Rml::Event&)
{
	if ( m_Strategy == "command" )
	{
		gEngfuncs.pfnClientCmd(false, m_Args.c_str());
	}
	else
	{
		Rml::Log::Message(Rml::Log::Type::LT_WARNING, "Unknown event handling atrategy: \"%s\"", m_Strategy.c_str());
	}
}
