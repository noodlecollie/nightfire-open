#pragma once

#include "BuildPlatform/Typedefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

const char* BuildPlatform_PlatformStringByID(const int platform);
const char* BuildPlatform_PlatformString(void);
const char* BuildPlatform_ArchitectureStringByID(const int arch, const uint abi, const int endianness, const qboolean is64);
const char* BuildPlatform_ArchitectureString(void);
const char* BuildPlatform_CommitString(void);

#ifdef __cplusplus
}  // extern "C"
#endif
