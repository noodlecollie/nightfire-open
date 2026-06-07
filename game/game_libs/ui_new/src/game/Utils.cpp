#include "game/Utils.h"
#include "udll_int.h"

Rml::String EscapeStringForConsoleCommand(Rml::String input)
{
	bool scripting = gEngfuncs.pfnGetCvarFloat("cmd_scripting") != 0.0f;

	Rml::String out;
	out.reserve(input.length());

	for ( auto it = input.begin(); it != input.end(); ++it )
	{
		const char c = *it;

		if ( c == '"' )
		{
			out += "\\";
		}
		else if ( scripting && c == '$' )
		{
			out += "$";
		}

		out += c;
	}

	return out;
}
