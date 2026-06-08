#pragma once

#include <RmlUi/Core/Types.h>
#include <vector>

struct MapListing
{
	Rml::String fileName;
	Rml::String description;
};

Rml::String EscapeStringForConsoleCommand(Rml::String input);
std::vector<MapListing> GetMapListings();
