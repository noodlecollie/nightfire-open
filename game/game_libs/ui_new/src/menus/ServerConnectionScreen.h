#pragma once

#include "framework/BaseMenu.h"

class ServerConnectionScreen : public BaseMenu
{
public:
	static const char* const NAME;

	ServerConnectionScreen();

	void ReceiveConnectionProgress_Connect(const Rml::String& server);
};
