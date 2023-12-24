/*
build.c - returns a engine build number
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include <assert.h>
#include "BuildPlatform/PlatformID.h"
#include "BuildPlatform/Typedefs.h"
#include "BuildEnums.h"
#include "BuildPlatform/VCS.h"

const char* BuildPlatform_PlatformStringByID(const int platform)
{
	switch ( platform )
	{
		case PLATFORM_WIN32:
		{
			return "win32";
		}

		case PLATFORM_LINUX_UNKNOWN:
		{
			return "linuxunkabi";
		}

		case PLATFORM_LINUX:
		{
			return "linux";
		}
	}

	assert(0);
	return "unknown";
}

const char* BuildPlatform_PlatformString(void)
{
	return BuildPlatform_PlatformStringByID(XASH_PLATFORM());
}

const char* BuildPlatform_ArchitectureStringByID(const int arch, const uint abi, const int endianness, const qboolean is64)
{
	switch ( arch )
	{
		case ARCHITECTURE_AMD64:
		{
			return "amd64";
		}

		case ARCHITECTURE_X86:
		{
			return "i386";
		}

		case ARCHITECTURE_E2K:
		{
			return "e2k";
		}

		case ARCHITECTURE_MIPS:
		{
			return endianness == ENDIANNESS_LITTLE ? (is64 ? "mips64el" : "mipsel") : (is64 ? "mips64" : "mips");
		}

		case ARCHITECTURE_ARM:
		{
			// no support for big endian ARM here
			if ( endianness == ENDIANNESS_LITTLE )
			{
				const int ver = (abi >> ARCHITECTURE_ARM_VER_SHIFT) & ARCHITECTURE_ARM_VER_MASK;
				const qboolean hardfp = (abi & ARCHITECTURE_ARM_HARDFP);

				if ( is64 )
				{
					return "arm64";  // keep as arm64, it's not aarch64!
				}

				switch ( ver )
				{
					case 8:
					{
						return hardfp ? "armv8_32hf" : "armv8_32l";
					}

					case 7:
					{
						return hardfp ? "armv7hf" : "armv7l";
					}

					case 6:
					{
						return "armv6l";
					}

					case 5:
					{
						return "armv5l";
					}

					case 4:
					{
						return "armv4l";
					}
				}
			}
			break;
		}

		case ARCHITECTURE_RISCV:
		{
			switch ( abi )
			{
				case ARCHITECTURE_RISCV_FP_SOFT:
				{
					return is64 ? "riscv64" : "riscv32";
				}

				case ARCHITECTURE_RISCV_FP_SINGLE:
				{
					return is64 ? "riscv64f" : "riscv32f";
				}

				case ARCHITECTURE_RISCV_FP_DOUBLE:
				{
					return is64 ? "riscv64d" : "riscv64f";
				}
			}

			break;
		}
	}

	assert(0);

	return is64 ? (endianness == ENDIANNESS_LITTLE ? "unknown64el" : "unknownel")
				: (endianness == ENDIANNESS_LITTLE ? "unknown64be" : "unknownbe");
}

const char* BuildPlatform_ArchitectureString(void)
{
	return BuildPlatform_ArchitectureStringByID(
		XASH_ARCHITECTURE(),
		XASH_ARCHITECTURE_ABI(),
		XASH_ENDIANNESS(),
#if XASH_64BIT()
		true
#else
		false
#endif
	);
}

const char* BuildPlatform_CommitString(void)
{
	return VCS_COMMIT_ID;
}
