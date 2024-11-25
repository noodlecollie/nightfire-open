#include "weaponatts_collection.h"
#include "weaponregistry.h"

namespace WeaponAtts
{
	WACollection::WACollection(const std::function<void(WACollection&)>& initialiser)
	{
		initialiser(*this);

		Validate();
		GenerateAttackModeSignatures();
		Register();
	}

	void WACollection::Register() const
	{
		CWeaponRegistry::StaticInstance().Add(this);
	}

	void WACollection::Validate() const
	{
		ASSERTSZ_Q(
			static_cast<size_t>(AttackModes.Count()) <= WeaponAtts::WACollection::MAX_ATTACK_MODES,
			"Attack mode limit exceeded.");

		Core.Validate();
		ViewModel.Validate();
		Prediction.Validate();
	}

	void WACollection::RegisterCvars() const
	{
		ASSERTSZ(!m_CvarsRegistered, "Cvars were already registered");

		FOR_EACH_VEC(SkillRecords, index)
		{
			SkillRecords[index].RegisterCvars();
		}

		FOR_EACH_VEC(CustomCvars, index)
		{
			CVAR_REGISTER(CustomCvars[index]);
		}

		m_CvarsRegistered = true;
	}

	void WACollection::GenerateAttackModeSignatures() const
	{
		FOR_EACH_VEC(AttackModes, index)
		{
			AttackModes[index]->SetSignature(Core.Id, index);
		}
	}
}  // namespace WeaponAtts
