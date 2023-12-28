#pragma once

#include <cstddef>
#include "bspfile.h"

namespace NFOpen
{
	dnfopenextraheader_t* GetHeader(dheader_t* header);
	dnfopenextralump_t* LookUpLump(dnfopenextraheader_t* header, uint32_t lumpID);
}  // namespace NFOpen
