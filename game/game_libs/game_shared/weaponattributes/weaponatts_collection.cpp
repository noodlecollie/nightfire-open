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
		ASSERTSZ(!m_CvarsRegistered, "Cvars were already registered");

		FOR_EACH_VEC(SkillRecords, index)
		{
			SkillRecords[index].RegisterCvars();
		}

		m_CustomCvarObjects.Purge();
		m_CustomCvarObjects.SetCount(CustomCvars.Count());

		FOR_EACH_VEC(CustomCvars, index)
		{
			const WACustomCvar& cvarDef = CustomCvars[index];
			cvar_t& cvar = m_CustomCvarObjects[index];

			cvar.name = const_cast<char*>(cvarDef.Name);
			cvar.string = const_cast<char*>(cvarDef.DefaultValue);
			cvar.value = cvarDef.IsNumerical ? static_cast<float>(atof(cvar.string)) : 0.0f;
			cvar.flags = 0;
			cvar.next = nullptr;

			CVAR_REGISTER(&cvar);
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
