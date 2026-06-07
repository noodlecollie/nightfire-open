#pragma once

#include <RmlUi/Core/Types.h>

// All relevant config cvars (hostname, password, etc.) are expected
// to have been written before this function is called.
void CreateMultiplayerGame(const Rml::String& mapName);
