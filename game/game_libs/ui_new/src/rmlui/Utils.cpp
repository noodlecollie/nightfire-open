#include "rmlui/Utils.h"
#include <RmlUi/Core/Element.h>
#include "EnginePublicAPI/keydefs.h"
#include "UIDebug.h"

// Note: Does not cater for modifier keys, since these are not handled by
// Rml::Input::KeyIdentifier.
static const std::unordered_map<int, Rml::Input::KeyIdentifier>& EngineToRmlKeyMap()
{
	using namespace Rml::Input;

	static std::unordered_map<int, KeyIdentifier> map;

	if ( map.empty() )
	{
		for ( int key = '0'; key <= '9'; ++key )
		{
			KeyIdentifier rmlKey = static_cast<Rml::Input::KeyIdentifier>(KeyIdentifier::KI_0 + (key - '0'));
			map.insert({key, rmlKey});
		}

		for ( int key = 'a'; key <= 'z'; ++key )
		{
			KeyIdentifier rmlKey = static_cast<KeyIdentifier>(KeyIdentifier::KI_A + (key - 'a'));
			map.insert({key, rmlKey});
		}

		for ( int key = K_F1; key <= K_F12; ++key )
		{
			KeyIdentifier rmlKey = static_cast<KeyIdentifier>(KeyIdentifier::KI_F1 + (key - K_F1));
			map.insert({key, rmlKey});
		}

		map.insert({';', KeyIdentifier::KI_OEM_1});
		map.insert({'=', KeyIdentifier::KI_OEM_PLUS});
		map.insert({',', KeyIdentifier::KI_OEM_COMMA});
		map.insert({'-', KeyIdentifier::KI_OEM_MINUS});
		map.insert({'.', KeyIdentifier::KI_OEM_PERIOD});
		map.insert({'/', KeyIdentifier::KI_OEM_2});
		map.insert({'`', KeyIdentifier::KI_OEM_3});
		map.insert({'[', KeyIdentifier::KI_OEM_4});
		map.insert({'\\', KeyIdentifier::KI_OEM_5});
		map.insert({']', KeyIdentifier::KI_OEM_6});
		map.insert({'\'', KeyIdentifier::KI_OEM_7});
		map.insert({'#', KeyIdentifier::KI_OEM_102});
		map.insert({K_TAB, KeyIdentifier::KI_TAB});
		map.insert({K_ENTER, KeyIdentifier::KI_RETURN});
		map.insert({K_ESCAPE, KeyIdentifier::KI_ESCAPE});
		map.insert({K_SPACE, KeyIdentifier::KI_SPACE});
		map.insert({K_BACKSPACE, KeyIdentifier::KI_BACK});
		map.insert({K_UPARROW, KeyIdentifier::KI_UP});
		map.insert({K_RIGHTARROW, KeyIdentifier::KI_RIGHT});
		map.insert({K_LEFTARROW, KeyIdentifier::KI_LEFT});
		map.insert({K_DOWNARROW, KeyIdentifier::KI_DOWN});
		map.insert({K_INS, KeyIdentifier::KI_INSERT});
		map.insert({K_DEL, KeyIdentifier::KI_DELETE});
		map.insert({K_PGDN, KeyIdentifier::KI_NEXT});
		map.insert({K_HOME, KeyIdentifier::KI_HOME});
		map.insert({K_END, KeyIdentifier::KI_END});
		map.insert({K_KP_HOME, KeyIdentifier::KI_NUMPAD7});
		map.insert({K_KP_UPARROW, KeyIdentifier::KI_NUMPAD8});
		map.insert({K_KP_PGUP, KeyIdentifier::KI_NUMPAD9});
		map.insert({K_KP_LEFTARROW, KeyIdentifier::KI_NUMPAD4});
		map.insert({K_KP_5, KeyIdentifier::KI_NUMPAD5});
		map.insert({K_KP_RIGHTARROW, KeyIdentifier::KI_NUMPAD6});
		map.insert({K_KP_END, KeyIdentifier::KI_NUMPAD1});
		map.insert({K_KP_DOWNARROW, KeyIdentifier::KI_NUMPAD2});
		map.insert({K_KP_PGDN, KeyIdentifier::KI_NUMPAD3});
		map.insert({K_KP_ENTER, KeyIdentifier::KI_NUMPADENTER});
		map.insert({K_KP_INS, KeyIdentifier::KI_NUMPAD0});
		map.insert({K_KP_DEL, KeyIdentifier::KI_DECIMAL});
		map.insert({K_KP_SLASH, KeyIdentifier::KI_DIVIDE});
		map.insert({K_KP_MINUS, KeyIdentifier::KI_SUBTRACT});
		map.insert({K_KP_PLUS, KeyIdentifier::KI_ADD});
		map.insert({K_KP_MUL, KeyIdentifier::KI_MULTIPLY});

		// From testing, the numpad * seems to come through as ASCII??
		// Just to make life more difficult for us...
		map.insert({'*', KI_MULTIPLY});

		// Explicitly ignore these ones. We know we don't want to support them.
		map.insert({K_CAPSLOCK, KeyIdentifier::KI_UNKNOWN});
		map.insert({K_SHIFT, KeyIdentifier::KI_UNKNOWN});
		map.insert({K_ALT, KeyIdentifier::KI_UNKNOWN});
		map.insert({K_CTRL, KeyIdentifier::KI_UNKNOWN});
		map.insert({K_KP_NUMLOCK, KeyIdentifier::KI_UNKNOWN});
		map.insert({K_WIN, KeyIdentifier::KI_UNKNOWN});
		map.insert({K_SCROLLOCK, KeyIdentifier::KI_UNKNOWN});
	}

	return map;
}

static const std::unordered_map<Rml::Input::KeyIdentifier, int>& RmlToEngineKeyMap()
{
	using namespace Rml::Input;

	static std::unordered_map<KeyIdentifier, int> map;

	if ( map.empty() )
	{
		for ( int rmlKey = KeyIdentifier::KI_0; rmlKey <= KeyIdentifier::KI_9; ++rmlKey )
		{
			int key = '0' + (rmlKey - KeyIdentifier::KI_0);
			map.insert({static_cast<KeyIdentifier>(rmlKey), key});
		}

		for ( int rmlKey = KeyIdentifier::KI_A; rmlKey <= KeyIdentifier::KI_Z; ++rmlKey )
		{
			int key = 'a' + (rmlKey - KeyIdentifier::KI_A);
			map.insert({static_cast<KeyIdentifier>(rmlKey), key});
		}

		for ( int rmlKey = KeyIdentifier::KI_F1; rmlKey <= KeyIdentifier::KI_F12; ++rmlKey )
		{
			int key = K_F1 + (rmlKey - KeyIdentifier::KI_F1);
			map.insert({static_cast<KeyIdentifier>(rmlKey), key});
		}

		map.insert({KeyIdentifier::KI_OEM_1, ';'});
		map.insert({KeyIdentifier::KI_OEM_PLUS, '='});
		map.insert({KeyIdentifier::KI_OEM_COMMA, ','});
		map.insert({KeyIdentifier::KI_OEM_MINUS, '-'});
		map.insert({KeyIdentifier::KI_OEM_PERIOD, '.'});
		map.insert({KeyIdentifier::KI_OEM_2, '/'});
		map.insert({KeyIdentifier::KI_OEM_3, '`'});
		map.insert({KeyIdentifier::KI_OEM_4, '['});
		map.insert({KeyIdentifier::KI_OEM_5, '\\'});
		map.insert({KeyIdentifier::KI_OEM_6, ']'});
		map.insert({KeyIdentifier::KI_OEM_7, '\''});
		map.insert({KeyIdentifier::KI_OEM_102, '#'});
		map.insert({KeyIdentifier::KI_TAB, K_TAB});
		map.insert({KeyIdentifier::KI_RETURN, K_ENTER});
		map.insert({KeyIdentifier::KI_ESCAPE, K_ESCAPE});
		map.insert({KeyIdentifier::KI_SPACE, K_SPACE});
		map.insert({KeyIdentifier::KI_BACK, K_BACKSPACE});
		map.insert({KeyIdentifier::KI_UP, K_UPARROW});
		map.insert({KeyIdentifier::KI_RIGHT, K_RIGHTARROW});
		map.insert({KeyIdentifier::KI_LEFT, K_LEFTARROW});
		map.insert({KeyIdentifier::KI_DOWN, K_DOWNARROW});
		map.insert({KeyIdentifier::KI_INSERT, K_INS});
		map.insert({KeyIdentifier::KI_DELETE, K_DEL});
		map.insert({KeyIdentifier::KI_NEXT, K_PGDN});
		map.insert({KeyIdentifier::KI_HOME, K_HOME});
		map.insert({KeyIdentifier::KI_END, K_END});
		map.insert({KeyIdentifier::KI_NUMPAD7, K_KP_HOME});
		map.insert({KeyIdentifier::KI_NUMPAD8, K_KP_UPARROW});
		map.insert({KeyIdentifier::KI_NUMPAD9, K_KP_PGUP});
		map.insert({KeyIdentifier::KI_NUMPAD4, K_KP_LEFTARROW});
		map.insert({KeyIdentifier::KI_NUMPAD5, K_KP_5});
		map.insert({KeyIdentifier::KI_NUMPAD6, K_KP_RIGHTARROW});
		map.insert({KeyIdentifier::KI_NUMPAD1, K_KP_END});
		map.insert({KeyIdentifier::KI_NUMPAD2, K_KP_DOWNARROW});
		map.insert({KeyIdentifier::KI_NUMPAD3, K_KP_PGDN});
		map.insert({KeyIdentifier::KI_NUMPADENTER, K_KP_ENTER});
		map.insert({KeyIdentifier::KI_NUMPAD0, K_KP_INS});
		map.insert({KeyIdentifier::KI_DECIMAL, K_KP_DEL});
		map.insert({KeyIdentifier::KI_DIVIDE, K_KP_SLASH});
		map.insert({KeyIdentifier::KI_SUBTRACT, K_KP_MINUS});
		map.insert({KeyIdentifier::KI_ADD, K_KP_PLUS});
		map.insert({KeyIdentifier::KI_MULTIPLY, K_KP_MUL});
	}

	return map;
}

Rml::String DescribeElement(const Rml::Element* element)
{
	if ( !element )
	{
		return "<null>";
	}

	Rml::String out = element->GetTagName();
	Rml::String id = element->GetId();

	return (!id.empty()) ? out + "#" + id : out;
}

int GetEventKeyId(const Rml::Event& event)
{
	return event.GetParameter<int>("key_identifier", 0);
}

Rml::Input::KeyIdentifier EngineKeyToRmlKey(int key)
{
	const std::unordered_map<int, Rml::Input::KeyIdentifier>& map = EngineToRmlKeyMap();
	const auto it = map.find(key);

	if ( it != map.end() )
	{
		return it->second;
	}

	// Oops, an unrecognised key that we might need to have mapped!
	ASSERTSZ(false, "Unrecognised key, may need mapping in RmlUi backend");

	return Rml::Input::KeyIdentifier::KI_UNKNOWN;
}

int RmlKeyToEngineKey(Rml::Input::KeyIdentifier key)
{
	const std::unordered_map<Rml::Input::KeyIdentifier, int>& map = RmlToEngineKeyMap();
	const auto it = map.find(key);

	if ( it != map.end() )
	{
		return it->second;
	}

	// We don't assert here, because RmlUi has many more keys than the engine supports,
	// and we're not trying to catch mappings that we miss in this direction.

	return -1;
}

unsigned char EngineKeyToRmlKeyModifier(int key)
{
	// Only track these three keys. Things like meta/Windows will likely
	// trigger system functions and won't be relevant for us.
	switch ( key )
	{
		case K_ALT:
		{
			return Rml::Input::KeyModifier::KM_ALT;
		}

		case K_CTRL:
		{
			return Rml::Input::KeyModifier::KM_CTRL;
		}

		case K_SHIFT:
		{
			return Rml::Input::KeyModifier::KM_SHIFT;
		}

		default:
		{
			return 0;
		}
	}
}
