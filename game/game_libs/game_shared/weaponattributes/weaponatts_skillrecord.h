#pragma once

#include "standard_includes.h"
#include "EnginePublicAPI/cvardef.h"
#include "skill.h"
#include "utlstring.h"

namespace WeaponAtts
{
	struct WASkillRecord
	{
		typedef float skilldata_t::* SkillDataEntryPtr;

		WASkillRecord(const char* cvarBaseName, SkillDataEntryPtr entry);
		WASkillRecord(const WASkillRecord& other);

		// After registring cvars, the instance of the class must persist until shutdown.
		// This class is designed to be used statically, so it should be OK.
		void RegisterCvars() const;
		void UpdateSkillValue(skilldata_t& instance) const;

		float GetValue(int skillLevel) const;

	private:
		SkillDataEntryPtr m_Entry;
		CUtlString m_BaseName;
		CUtlString m_NameBuffers[TOTAL_SKILL_LEVELS];
		mutable cvar_t m_Cvars[TOTAL_SKILL_LEVELS];
	};
}  // namespace WeaponAtts
