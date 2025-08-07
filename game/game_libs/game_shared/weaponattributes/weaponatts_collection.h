#pragma once

#include <functional>
#include <memory>

#include "weaponatts_core.h"
#include "weaponatts_skillrecord.h"
#include "weaponatts_viewmodel.h"
#include "weaponatts_playermodel.h"
#include "weaponatts_soundset.h"
#include "weaponatts_ammodef.h"
#include "weaponatts_baseattack.h"
#include "weaponatts_prediction.h"
#include "weaponatts_precache.h"
#include "utlvector.h"

namespace WeaponAtts
{
	// Weapon attributes are designed to comprise a sort of static database of useful
	// properties for a weapon. The CGenericWeapon class and its subclasses use these
	// properties to establish the behaviour of each weapon.
	struct WACollection
	{
		static constexpr size_t MAX_ATTACK_MODES = 8;

		WACore Core;
		WAAmmoDef Ammo;
		CUtlVector<WASkillRecord> SkillRecords;
		CUtlVector<cvar_t*> CustomCvars;
		CUtlVector<std::shared_ptr<WABaseAttack>> AttackModes;

		WAViewModel ViewModel;
		WAPlayerModel PlayerModel;
		WAPrediction Prediction;
		WAPrecache Precache;

		WACollection(const std::function<void(WACollection&)>& initialiser);

		void RegisterCvars() const;

	private:
		void Register() const;
		void Validate() const;
		void GenerateAttackModeSignatures() const;

		mutable bool m_CvarsRegistered = false;
	};

	template<typename T>
	const WACollection& StaticWeaponAttributes();
}  // namespace WeaponAtts
