#pragma once

#include "weaponatts_base.h"
#include "utlvector.h"

namespace WeaponAtts
{
	// For precaching extra arbitrary files that are not covered
	// by other weapon attributes.
	struct WAPrecache : public WABase
	{
		CUtlVector<const char*> Sounds;
		CUtlVector<const char*> Models;
		CUtlVector<const char*> Classes;

		void Validate() const override
		{
			FOR_EACH_VEC(Sounds, index)
			{
				ASSERTSZ_Q(Sounds[index], "Sound path must be valid.");
			}

			FOR_EACH_VEC(Models, index)
			{
				ASSERTSZ_Q(Models[index], "Model path must be valid.");
			}

			FOR_EACH_VEC(Classes, index)
			{
				ASSERTSZ_Q(Classes[index], "Classname must be valid.");
			}
		}
	};
}  // namespace WeaponAtts
