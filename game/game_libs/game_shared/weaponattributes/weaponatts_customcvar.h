#pragma once

#include "weaponatts_base.h"

namespace WeaponAtts
{
	struct WACustomCvar : public WABase
	{
		const char* Name = nullptr;

		// This is a string because the cvar_t expects a static string
		// as its default value, so we require that the user provides one.
		// We can easily convert this to a float at runtime.
		const char* DefaultValue = "";

		bool IsNumerical = true;

		virtual void Validate() const override
		{
			ASSERTSZ_Q(Name, "Cvar must have a name.");
		}

		WACustomCvar(const char* name, const char* defaultValue, bool isNumerical) :
			Name(name),
			DefaultValue(defaultValue),
			IsNumerical(isNumerical)
		{
		}

		static WACustomCvar String(const char* name, const char* defaultValue)
		{
			return WACustomCvar(name, defaultValue, false);
		}

		static WACustomCvar Number(const char* name, const char* defaultValue)
		{
			return WACustomCvar(name, defaultValue, true);
		}
	};
}
