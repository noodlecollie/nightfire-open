#include "nfopenheader.h"
#include "log.h"

namespace NFOpen
{
	dnfopenextraheader_t* GetHeader(dheader_t* header)
	{
		if ( !header )
		{
			Error("NFOpen::GetHeader: BSP header was null\n");
			return nullptr;
		}

		return (dnfopenextraheader_t*)((byte*)header + sizeof(dheader_t));
	}

	dnfopenextralump_t* LookUpLump(dnfopenextraheader_t* header, uint32_t lumpID)
	{
		dnfopenextralump_t* base = (dnfopenextralump_t*)((byte*)header + sizeof(*header));

		for ( uint32_t index = 0; index < header->numLumps; ++index )
		{
			if ( base[index].lumpIndex == lumpID )
			{
				return &base[index];
			}
		}

		Error("NFOpen::LookUpLump: Could not find lump with ID %u\n", lumpID);
		return nullptr;
	}
}  // namespace NFOpen
