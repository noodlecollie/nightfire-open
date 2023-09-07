#pragma once

#include "PlatformDefs/typedefs.h"

int Q_buildnum(void);
int Q_buildnum_compat(void);
const char* Q_PlatformStringByID(const int platform);
const char* Q_buildos(void);
const char* Q_ArchitectureStringByID(const int arch, const uint abi, const int endianness, const qboolean is64);
const char* Q_buildarch(void);
const char* Q_buildcommit(void);
