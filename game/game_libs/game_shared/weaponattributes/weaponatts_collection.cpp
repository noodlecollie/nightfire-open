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
		ASSERTSZ_Q(AttackModes.Count() <= WeaponAtts::WACollection::MAX_ATTACK_MODES, "Attack mode limit exceeded.");

		Core.Validate();
		ViewModel.Validate();
		Prediction.Validate();
	}

	void WACollection::RegisterCvars() const
	{
		FOR_EACH_VEC(SkillRecords, index)
		{
			SkillRecords[index].RegisterCvars();
		}

		FOR_EACH_VEC(CustomCvars, index)
		{
			CVAR_REGISTER(CustomCvars[index]);
		}
	}

	void WACollection::GenerateAttackModeSignatures() const
	{
		FOR_EACH_VEC(AttackModes, index)
		{
			AttackModes[index]->SetSignature(Core.Id, index);
		}
	}
}
