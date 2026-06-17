#include "game/Utils.h"
#include <RmlUi/Core/Log.h>
#include "Utils/InFilePtr.h"
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

std::vector<MapListing> GetMapListings()
{
	static constexpr const char* const MAPS_FILE_NAME = "maps.lst";

	if ( !gEngfuncs.pfnCreateMapsList(false) )
	{
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Engine could not build %s", MAPS_FILE_NAME);
		return {};
	}

	InFileCharsPtr inFile(MAPS_FILE_NAME);

	if ( !inFile.IsValid() )
	{
		Rml::Log::Message(Rml::Log::Type::LT_ERROR, "Could not open %s", MAPS_FILE_NAME);
		return {};
	}

	std::vector<MapListing> maps;

	while ( true )
	{
		MapListing listing;
		listing.fileName.resize(64, '\0');
		listing.description.resize(64, '\0');

		if ( !inFile.ParseToken(&listing.fileName[0], listing.fileName.length() + 1) ||
			 !inFile.ParseToken(&listing.description[0], listing.description.length() + 1) )
		{
			// End of file
			break;
		}

		maps.push_back(std::move(listing));
	}

	return maps;
}
