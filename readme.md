Nightfire Open
==============

![DM Power](/resources/img/dm_power.png "DM Power")

![Tests](https://github.com/noodlecollie/nightfire-open/actions/workflows/CI-Game.yaml/badge.svg)
![Releases](https://github.com/noodlecollie/nightfire-open/actions/workflows/Release.yaml/badge.svg)

Nightfire Open is an open source reboot of [James Bond 007: Nightfire](https://en.wikipedia.org/wiki/James_Bond_007:_Nightfire) as a Half Life mod, which runs on Windows and Linux.

## Downloads

The latest downloads for the game and dedicated server can be found on the [releases page](https://github.com/noodlecollie/nightfire-open/releases). Note that at present the game is in a relatively minimal playable state, so the experience will not be representative of what is planned for the future.

To run the game:

* Run `bond.exe` on Windows
* Execute `bond.sh` on Linux.

## Building

Ensure you have [Git LFS](https://git-lfs.com/) installed, as the repo uses this to manage the game content.

[CMake](https://cmake.org/download/) 3.21 or greater is required to build the game. Compilation has been tested with Visual Studio 2022 on Windows, and GCC 11.4 on Linux.
On Windows, dependencies should be managed by the embedded `vcpkg` install. On Linux, you will need at least the following:

``` bash
sudo apt install build-essential mesa-utils libsdl2-dev
```

For a basic debug build of the game client, follow these steps:

``` bash
# Clone the repo to your hard drive and enter the directory.
# Note that without passing --recursive, vcpkg will not function
# properly on Windows. To fix this for an existing clone,
# run git submodule update --init --recursive
git clone --recursive https://github.com/noodlecollie/nightfire-open.git && cd nightfire-open

# Configure and run a build with CMake.
cmake -DCMAKE_INSTALL_PREFIX:STRING=install -S . -B build
cmake --build build --config Debug --target install -j 10
```

The game will be placed in `build/install`. To run on Linux:

``` bash
cd build/install/nightfire-open
./bond
```

To run on Windows:

``` cmd
cd build\install\nightfire-open
bond.exe
```

## About

The primary purpose of this game is to recreate and improve the experience of the PC version of Nightfire, and to provide in the process an open source codebase that will allow for extension and refinement.

Nightfire Open is built with the [Xash3D FWGS](https://github.com/FWGS/xash3d-fwgs) engine, an open source Goldsrc engine re-implementation. This is for a few reasons:

* The codebase is fully open, so can be adapted as required.
* It's smaller and simpler than the Source engine, its closest cousin.
* It's close in principle to the genuine PC Nightfire engine (itself a heavily modified version of GoldSrc).
* Developing on a more limited engine doesn't come with the expectation that the game will be photorealistic. I don't have the time or the energy as a single hobbyist to be working on some sort of HD photorealistic remake.

The original repo used for development was [afterburner-game](https://github.com/noodlecollie/afterburner-game), but it became clear that being tied to the upstream engine repo was detrimental. This project's coding standards were different to the upstream; it needed more flexibility than the engine was able to offer on its own; the Git subrepository architecture was consistently a pain to work with; and the Waf build system was complex, esoteric and poorly documented. Consequently, it was decided to take a hard fork of the Xash3D engine and migrate everything to this new CMake-based repository instead, for full customisability.

## Credits

Many thanks to:

* Gearbox for the original game.
* The FWGS team for the Xash3D engine and Xash3D version of Half Life SDK.
* j-yaniv and others for Rho-Bot Half Life bots.
* Developers of STB libraries.
* Developers of rapidjson library.
* Contributors to XeNTaX game reverse-engineering forums.
* ZapSplat.com for various free sound effects.
* Various Nightfire modders over the years whose tools and reverse-engineering have proven invaluable when developing this mod (specifically Tres, 009, Ford, UltimateSniper).
