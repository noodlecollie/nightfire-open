#include "rmlui/EventListenerInstancerImpl.h"
#include "rmlui/EventListenerImpl.h"
#include "UIDebug.h"

inline void TrimLeft(std::string& s)
{
	s.erase(
		s.begin(),
		std::find_if(
			s.begin(),
			s.end(),
			[](unsigned char ch)
			{
				return !std::isspace(ch);
			}
		)
	);
}

inline void TrimRight(std::string& s)
{
	s.erase(
		std::find_if(
			s.rbegin(),
			s.rend(),
			[](unsigned char ch)
			{
				return !std::isspace(ch);
			}
		).base(),
		s.end()
	);
}

inline Rml::String Trim(Rml::String s)
{
	TrimLeft(s);
	TrimRight(s);
	return s;
}

static inline Rml::String DescribeElement(Rml::Element* element)
{
	if ( !element )
	{
		return "<NULL>";
	}

	const Rml::String& id = element->GetId();
	const Rml::String& tag = element->GetTagName();

	if ( !id.empty() )
	{
		return "<" + tag + " id='" + id + "'>";
	}
	else
	{
		return "<" + tag + ">";
	}
}

Rml::EventListener* EventListenerInstancerImpl::InstanceEventListener(const Rml::String& value, Rml::Element* element)
{
	const std::size_t colonIndex = value.find(':');

	if ( colonIndex == Rml::String::npos )
	{
		Rml::Log::Message(
			Rml::Log::Type::LT_WARNING,
			"Event listener instancer found no colon separator in event string \"%s\" for element %s",
			value.c_str(),
			DescribeElement(element).c_str()
		);

		return nullptr;
	}

	return new EventListenerImpl(Trim(value.substr(0, colonIndex)), Trim(value.substr(colonIndex + 1)));
}
