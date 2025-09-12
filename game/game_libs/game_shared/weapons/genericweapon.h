#pragma once

#include "standard_includes.h"
#include "weapons.h"
#include "utlvector.h"
#include "weaponatts_collection.h"
#include "weaponatts_ammobasedattack.h"
#include "gameplay/weaponInaccuracyCalculator.h"

class CBaseBot;
class CBaseBotFightStyle;

namespace WeaponMechanics
{
	class CBaseMechanic;
	struct InvocationResult;
}  // namespace WeaponMechanics

// Build on top of CBasePlayerWeapon, because this is so tied into the engine
// already it'd be a pain to replace it (at least at this stage).
class CGenericWeapon : public CBasePlayerWeapon
{
	friend class WeaponMechanics::CBaseMechanic;

public:
	enum SpawnFlag
	{
		SF_DontDrop = (1 << 0)
	};

	CGenericWeapon();
	~CGenericWeapon();
	void Spawn() override;
	void Precache() override;
	int GetItemInfo(ItemInfo* p) override;
	int AddToPlayer(CBasePlayer* pPlayer) override;
	BOOL Deploy() override;
	void Holster(int skiplocal = 0) override;
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	void Reload() override;
	void WeaponIdle() override;
	int iItemSlot() override;
	void ItemPostFrame() override;
	bool ReadPredictionData(const weapon_data_t* from) override;
	bool WritePredictionData(weapon_data_t* to) override;
	int UpdateClientData(CBasePlayer* pPlayer) override;

	virtual const WeaponAtts::WACollection& WeaponAttributes() const = 0;

	float GetInaccuracy() const;
	byte GetPrimaryAttackModeIndex() const;

	template<typename T>
	const T* GetAttackModeFromAttributes(uint32_t index) const
	{
		const WeaponAtts::WACollection& atts = WeaponAttributes();
		ASSERT(index < static_cast<uint32_t>(atts.AttackModes.Count()));

		if ( index >= static_cast<uint32_t>(atts.AttackModes.Count()) )
		{
			return nullptr;
		}

		const T* attackMode = dynamic_cast<const T*>(atts.AttackModes[index].get());
		ASSERT(attackMode);

		return attackMode;
	}

#ifndef CLIENT_DLL
	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];
#endif

	const char* PickupSound() const override;

protected:
	void PrecacheSoundSet(const WeaponAtts::WASoundSet& sounds);
	void SetViewModelBody(int body, bool immediate = false);
	float ViewModelAnimationDuration(int anim) const;
	void PlaySound(const WeaponAtts::WASoundSet& sound, int channel = CHAN_WEAPON, float volModifier = 1.0f);

	void DelayPendingActions(float secs, bool allowIfEarlier = false);
	void DelayFiring(
		float secs,
		bool allowIfEarlier = false,
		WeaponAtts::AttackMode attackMode = WeaponAtts::AttackMode::None
	);
	bool HasAmmo(const WeaponAtts::WABaseAttack* attackMode, int minCount = 1, bool useClip = true) const;
	bool HasAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool pool, int minCount = 1, bool useClip = true) const;
	bool DecrementAmmo(const WeaponAtts::WABaseAttack* attackMode);
	bool DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool pool, int decrement);
	int AmmoLeft(const WeaponAtts::WABaseAttack* attackMode) const;
	int AmmoLeft(WeaponAtts::WAAmmoBasedAttack::AmmoPool pool) const;

	bool CanPrimaryAttack() const;
	bool CanSecondaryAttack() const;
	bool CanPrimaryOrSecondaryAttack() const;
	bool CanReload() const;

	int GetEventIDForAttackMode(const WeaponAtts::WABaseAttack* attack) const;

	// Override the view model animations based on those used
	// for an attack mode.
	WeaponAtts::AttackMode GetViewModelAnimationSource();
	void SetViewModelAnimationSource(WeaponAtts::AttackMode source);

	// Return the value to set m_fInSpecialReload to next.
	virtual int HandleSpecialReload(int currentState);

	// For convenience:
	inline void SetNextPrimaryAttack(float secsInFuture, bool allowIfEarlier = false)
	{
		const float delay = UTIL_WeaponTimeBase() + secsInFuture;
		if ( allowIfEarlier || delay > m_flNextPrimaryAttack )
		{
			m_flNextPrimaryAttack = GetNextAttackDelay(secsInFuture);
		}
	}

	inline void SetNextSecondaryAttack(float secsInFuture, bool allowIfEarlier = false)
	{
		const float delay = UTIL_WeaponTimeBase() + secsInFuture;
		if ( allowIfEarlier || delay > m_flNextSecondaryAttack )
		{
			m_flNextSecondaryAttack = delay;
		}
	}

	inline void SetNextIdleTime(float secsInFuture, bool allowIfEarlier = false)
	{
		const float delay = UTIL_WeaponTimeBase() + secsInFuture;
		if ( allowIfEarlier || delay > m_flTimeWeaponIdle )
		{
			m_flTimeWeaponIdle = delay;
		}
	}

	inline int DefaultEventFlags()
	{
#ifdef CLIENT_WEAPONS
		return FEV_NOTHOST;
#else
		return 0;
#endif
	}

	template<typename Attack, typename Mechanic>
	Mechanic* AddMechanic(const Attack* attack)
	{
		Mechanic* mechanic = new Mechanic(this, attack);
		m_Mechanics.AddToTail(mechanic);
		return mechanic;
	}

	// Helper so that the types can be inferred
	template<typename Attack, typename Mechanic>
	void AddMechanic(const Attack* inAttack, Mechanic*& outMode)
	{
		outMode = AddMechanic<Attack, Mechanic>(inAttack);
	}

	// Helper that allows choosing an attack via index
	template<typename Attack, typename Mechanic>
	void AddMechanicByAttributeIndex(uint32_t index, Mechanic*& outMode)
	{
		AddMechanic<Attack, Mechanic>(GetAttackModeFromAttributes<Attack>(index), outMode);
	}

	WeaponMechanics::CBaseMechanic* GetPrimaryAttackMechanic() const;
	void SetPrimaryAttackMechanic(WeaponMechanics::CBaseMechanic* mechanic);
	WeaponMechanics::CBaseMechanic* GetSecondaryAttackMechanic() const;
	void SetSecondaryAttackMechanic(WeaponMechanics::CBaseMechanic* mechanic);
	WeaponMechanics::CBaseMechanic* GetAttackMechanic(WeaponAtts::AttackMode mode) const;

	virtual bool PrepareToInvokeAttack(WeaponAtts::AttackMode mode);
	virtual void AttackInvoked(WeaponAtts::AttackMode mode, const WeaponMechanics::InvocationResult& result);

private:
	// TODO: Should these be delegated somewhere else, a la aggregate programming model?
	void PrecacheCore(const WeaponAtts::WACore& core);
	void PrecacheViewModel(const WeaponAtts::WAViewModel& viewModel);
	void PrecachePlayerModel(const WeaponAtts::WAPlayerModel& playerModel);

	WeaponMechanics::CBaseMechanic* GetMechanicByIndex(int index) const;
	void SetMechanicIndex(WeaponMechanics::CBaseMechanic* mechanic, int& outIndex);
	bool IsValidMechanicIndex(int index) const;
	WeaponAtts::AttackMode GetAttackModeForMechanic(const WeaponMechanics::CBaseMechanic* mechanic) const;
	const WeaponAtts::WAAmmoBasedAttack* GetPrimaryAmmoBasedAttackMode() const;

	WeaponMechanics::CBaseMechanic* GetEnqueuedMechanic() const;
	void SetEnqueuedMechanic(WeaponAtts::AttackMode mode, WeaponMechanics::CBaseMechanic* mechanic, int nextStep);
	void ResetEnqueuedMechanic();

	bool InvokeAttack(WeaponAtts::AttackMode mode);
	bool InvokeMechanic(WeaponAtts::AttackMode mode, WeaponMechanics::CBaseMechanic* mechanic, int step);
	void SetFireOnEmptyState(const WeaponAtts::WABaseAttack* attackMode);
	void SetFireOnEmptyState(WeaponAtts::WAAmmoBasedAttack::AmmoPool pool);

	// Return true if reload action occurred, or false otherwise.
	bool IdleProcess_CheckReload();
	bool IdleProcess_CheckSpecialReload();
	void IdleProcess_PlayIdleAnimation();

	void FindWeaponSlotInfo();
	const WeaponAtts::AccuracyParameters* GetWeaponAccuracyParams() const;
	const WeaponAtts::ViewModelAnimationSet& GetViewModelAnimationSet() const;

	void RunAttackLogic();
	void PerformReload();
	bool ReloadUsableWeaponIfEmpty();
	void UpdateValuesPostFrame();
	void PlayEmptySoundIfAllowed(const WeaponAtts::WABaseAttack& attackMode);
	bool ShouldSecondaryAttackThisFrame() const;
	bool ShouldPrimaryAttackThisFrame() const;
	void HandleNoButtonsDown_Server();
	void HandleNoButtonsDown_Client();

	CUtlVector<int> m_AttackModeEvents;
	CUtlVector<float> m_ViewAnimDurations;
	CWeaponInaccuracyCalculator m_InaccuracyCalculator;

	// If set to something other than none, the view model animations
	// are taken from the specified attack mode, rather than the default
	// view model attributes.
	WeaponAtts::AttackMode m_ViewModelAnimationSource = WeaponAtts::AttackMode::None;

	CUtlVector<WeaponMechanics::CBaseMechanic*> m_Mechanics;
	int m_PrimaryAttackMechanicIndex = -1;
	int m_SecondaryAttackMechanicIndex = -1;
	int m_EnqueuedMechanicIndex = -1;
	int m_EnqueuedAttackMode = static_cast<int>(WeaponAtts::AttackMode::None);
	int m_NextEnqueuedAttackStep = 0;
	int m_iViewModelIndex = 0;
	int m_iViewModelBody = 0;
	int m_iWeaponSlot = -1;
	int m_iWeaponSlotPosition = -1;
	bool m_bPrimaryAttackHeldDown = false;
	bool m_bPrimaryAttackThisFrame = false;
	bool m_bSecondaryAttackHeldDown = false;
	bool m_bSecondaryAttackThisFrame = false;
	float m_flInaccuracy = 0.0f;
	byte m_iLastPriAttackMode = 0;
};

// Function specialisations which must be outside the class
// to be fully standards-compliant:

// Specialisation for base class, which does not need a dynamic_cast.
template<>
inline const WeaponAtts::WABaseAttack* CGenericWeapon::GetAttackModeFromAttributes<WeaponAtts::WABaseAttack>(
	uint32_t index
) const
{
	const WeaponAtts::WACollection& atts = WeaponAttributes();
	ASSERT(index < static_cast<uint32_t>(atts.AttackModes.Count()));

	if ( index >= static_cast<uint32_t>(atts.AttackModes.Count()) )
	{
		return nullptr;
	}

	return atts.AttackModes[index].get();
}

class CGenericAmmo : public CBasePlayerAmmo
{
public:
	enum SpawnFlag
	{
		SF_DontDrop = (1 << 0)
	};

	CGenericAmmo(const char* modelName, const CAmmoDef& ammoDef, const char* pickupSoundName = NULL) :
		CBasePlayerAmmo(),
		m_szModelName(modelName),
		m_AmmoDef(ammoDef),
		m_szPickupSoundName(pickupSoundName)
	{
		ASSERT(m_szModelName);
	}

	void Spawn() override
	{
		Precache();
		SET_MODEL(ENT(pev), m_szModelName);
		CBasePlayerAmmo::Spawn();

		if ( pev->spawnflags & SF_DontDrop )
		{
			pev->movetype = MOVETYPE_NONE;
		}
	}

	void Precache() override
	{
		PRECACHE_MODEL(m_szModelName);
		PRECACHE_SOUND(m_szPickupSoundName ? m_szPickupSoundName : DEFAULT_PICKUP_SOUND);
	}

	BOOL AddAmmo(CBaseEntity* pOther) override
	{
		if ( pOther->GiveAmmo(m_AmmoDef.AmmoBoxGive, m_AmmoDef.AmmoName, m_AmmoDef.MaxCarry) != -1 )
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, DEFAULT_PICKUP_SOUND, 1, ATTN_NORM);
			return TRUE;
		}

		return FALSE;
	}

	inline const char* AmmoName() const
	{
		return m_AmmoDef.AmmoName;
	}

private:
	static constexpr const char* DEFAULT_PICKUP_SOUND = "items/ammopickup1.wav";

	const char* m_szModelName;
	const CAmmoDef& m_AmmoDef;
	const char* m_szPickupSoundName;
};
