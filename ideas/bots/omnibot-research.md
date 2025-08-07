[OmniBot](https://github.com/jswigart/omni-bot) is a bot library designed to be plugged into first person shooter games. Nightfire Open has a fork of this that we can modify and adapt, at https://github.com/noodlecollie/omni-bot.

The OmniBot project seems to be designed to tie into old games (Quake, Team Fortress, and others) which have SDKs based on relatively ancient versions of MSVC. Because of this, the project structure and code quality leave something to be desired. However, there may be things we could do to adapt it for use in Nightfire Open.

# What is required to use OmniBot?

The repo itself does not contain much in the way of documentation, so this document details investigations into the structure of the projects themselves. The Quake 4 integration was used as an example, because it is probably the closest in style to Nightfire Open.

For an integration with a particular game, the OmniBot Common library (found under `OmniBot/Common`) is built statically, and linked into a shared library designed for use with the game in question. Here, `OmniBot/Q4` is compiled to produce the `omnibot_q4` shared library.

Additionally, each game has an interface which loads and comunicates with the shared library. For Quake 4, the `GameInterfaces/Q4` project is built - this appears to be a copy of the Quake SDK. For reference, the `GameInterfaces/Q4/src/mpgame/omnibot/omnibot_interface.h` file defines the class used to facilitate communications between the Quake server and the OmniBot implementation.

In the case of Nightfire Open, we would probably use the game code to directly communicate with the OmniBot library, and would not need a separate project under `GameInterfaces`.

# How is the OmniBot library initialised?

When a game interface wants to load its OmniBot shared library, it calls `Omnibot_LoadLibrary()`, and to unload the library it calls `Omnibot_FreeLibrary()`. These functions are implemented within `Omnibot/Common/BotLoadLibrary.cpp` - this file appears to be compiled into the game interface project as well as the OmniBot common library. Shoving random `.cpp` files into projects that don't own them seems to be a theme of this repository, and it's not one that I like at all, but let's put that to one side for now. I suspect that this file is simply used as a helper for loading the shared library - we should certainly rewrite this in Nightfire Open to suit our own purposes.

`Omnibot_LoadLibrary()` loads the OmniBot library for the game - in Quake 4's case this would be `omnibot_q4.dll` for Windows, or `libomnibot_q4.so` for Linux. A factory function is then looked up and called to properly set up the interface to the library internals; this appears to be `CreateBotInterface()` on Windows but `ExportBotFunctionsFromDLL()` on Linux, even though this latter function does not exist in code.

Following the chain through to `CreateBotInterface()`, this funcion is declared within `Omnibot/Common/BotExports.h`, and is the only formally exported function in the shared library. It creates and returns an `OmnibotFunctions` class instance, and this instance is used as an interface to communicate with the library internals.

To complete the life cycle, `Omnibot_FreeLibrary()` is called. The shared library is unloaded, and the previously produced interface is automatically destroyed by the unload.

# How do the game and the library communicate with one another?

`Omnibot/Common/IEngineInterface.h` defines two interface classes which encapsulate all game-to-library and library-to-game fuction calls. `IEngineInterface` defines the set of functions that a bot can call on the game, and `OmnibotFunctionInterface` defines the set of functions that a game can call to control the OmniBot library.
