#pragma once

#include "standard_includes.h"
#include "weapons.h"
#include "utlvector.h"
#include "weaponatts_collection.h"
#include "weaponatts_ammobasedattack.h"
#include "gameplay/weaponInaccuracyCalculator.h"

class CBaseBot;
class CBaseBotFightStyle;

// Build on top of CBasePlayerWeapon, because this is so tied into the engine
// already it'd be a pain to replace it (at least at this stage).
class CGenericWeapon : public CBasePlayerWeapon
{
public:
	enum SpawnFlag
	{
		SF_DontDrop = (1 << 0)
	};

	CGenericWeapon();
	virtual ~CGenericWeapon();
	virtual void Spawn() override;
	virtual void Precache() override;
	virtual int GetItemInfo(ItemInfo *p) override;
	virtual int AddToPlayer(CBasePlayer *pPlayer) override;
	virtual BOOL Deploy() override;
	virtual void PrimaryAttack() override;
	virtual void SecondaryAttack() override;
	virtual void Reload() override;
	virtual void WeaponIdle() override;
	virtual int iItemSlot() override;
	virtual void ItemPostFrame() override;
	virtual bool ReadPredictionData(const weapon_data_t* from) override;
	virtual bool WritePredictionData(weapon_data_t* to) override;
	virtual int UpdateClientData(CBasePlayer* pPlayer) override;

	virtual const WeaponAtts::WACollection& WeaponAttributes() const = 0;

	float GetInaccuracy() const;
	byte GetPrimaryAttackModeIndex() const;

#ifndef CLIENT_DLL
	// Don't know if this is the best place to put these?
	// Currently refactoring weapon attributes and didn't like putting
	// non-static behaviour like this into static attributes.
	// Evaluate whether this should be moved elsewhere - possibly
	// make bot weapon profiles local to bot code?
	virtual float Bot_CalcDesireToUse(CBaseBot& bot, CBaseEntity& enemy, float distanceToEnemy) const = 0;
	virtual void Bot_SetFightStyle(CBaseBotFightStyle& fightStyle) const = 0;
	virtual int Save(CSave &save) override;
	virtual int Restore(CRestore &restore) override;
	static TYPEDESCRIPTION m_SaveData[];
#endif

	static constexpr float DEFAULT_BULLET_TRACE_DISTANCE = 8192;
	static void GetSharedCircularGaussianSpread(uint32_t shotIndex, int shared_rand, float& x, float& y);

protected:
	enum class WeaponAttackType
	{
		None = -1,
		Primary = 0,
		Secondary = 1
	};

	template<typename T>
	const T* GetAttackModeFromAttributes(uint32_t index) const
	{
		const WeaponAtts::WACollection& atts = WeaponAttributes();
		ASSERT(index < atts.AttackModes.Count());

		if ( index >= atts.AttackModes.Count() )
		{
			return nullptr;
		}

		const T* attackMode = dynamic_cast<const T*>(atts.AttackModes[index].get());
		ASSERT(attackMode);

		return attackMode;
	}

	virtual const char* PickupSound() const override;

	// Overridable functions for attack modes:
	virtual void PrecacheAttackMode(const WeaponAtts::WABaseAttack& attackMode);
	virtual bool InvokeWithAttackMode(WeaponAttackType type, const WeaponAtts::WABaseAttack* attackMode);

	void PrecacheSoundSet(const WeaponAtts::WASoundSet& sounds);
	void SetViewModelBody(int body, bool immediate = false);
	float ViewModelAnimationDuration(int anim) const;
	void PlaySound(const WeaponAtts::WASoundSet& sound, int channel = CHAN_WEAPON, float volModifier = 1.0f);

	void DelayPendingActions(float secs, bool allowIfEarlier = false);
	void DelayFiring(float secs, bool allowIfEarlier = false, WeaponAttackType attackType = WeaponAttackType::None);
	bool HasAmmo(const WeaponAtts::WABaseAttack* attackMode, int minCount = 1, bool useClip = true) const;
	bool DecrementAmmo(const WeaponAtts::WABaseAttack* attackMode, int decrement);
	bool CanReload() const;

	int GetEventIDForAttackMode(const WeaponAtts::WABaseAttack* attack) const;

	template<typename T = WeaponAtts::WABaseAttack>
	inline const T* GetPrimaryAttackMode() const
	{
		return dynamic_cast<const T*>(m_pPrimaryAttackMode);
	}

	template<typename T = WeaponAtts::WABaseAttack>
	inline const T* GetSecondaryAttackMode() const
	{
		return dynamic_cast<const T*>(m_pSecondaryAttackMode);
	}

	void SetPrimaryAttackMode(const WeaponAtts::WABaseAttack* mode);
	void SetSecondaryAttackMode(const WeaponAtts::WABaseAttack* mode);

	// T can be used to validate the type of attack expected to be set,
	// but can be omitted if this is not required.
	template<typename T = WeaponAtts::WABaseAttack>
	inline void SetPrimaryAttackModeFromAttributes(uint32_t modeIndex)
	{
		SetPrimaryAttackMode(GetAttackModeFromAttributes<T>(modeIndex));
	}

	// T can be used to validate the type of attack expected to be set,
	// but can be omitted if this is not required.
	template<typename T = WeaponAtts::WABaseAttack>
	inline void SetSecondaryAttackModeFromAttributes(uint32_t modeIndex)
	{
		SetSecondaryAttackMode(GetAttackModeFromAttributes<T>(modeIndex));
	}

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

private:
	// TODO: Should these be delegated somewhere else, a la aggregate programming model?
	void PrecacheCore(const WeaponAtts::WACore& core);
	void PrecacheViewModel(const WeaponAtts::WAViewModel& viewModel);
	void PrecachePlayerModel(const WeaponAtts::WAPlayerModel& playerModel);

	bool InvokeAttack(WeaponAttackType type);
	void SetFireOnEmptyState(const WeaponAtts::WABaseAttack* attackMode);

	// Return true if reload action occurred, or false otherwise.
	bool IdleProcess_CheckReload();
	bool IdleProcess_CheckSpecialReload();
	void IdleProcess_PlayIdleAnimation();

	void FindWeaponSlotInfo();
	const WeaponAtts::AccuracyParameters* GetWeaponAccuracyParams() const;

	void PerformReload();
	bool ReloadUsableWeaponIfEmpty();
	void UpdateValuesPostFrame();
	bool ShouldSecondaryAttackThisFrame() const;
	bool ShouldPrimaryAttackThisFrame() const;
	void HandleNoButtonsDown_Server();
	void HandleNoButtonsDown_Client();

	CUtlVector<int> m_AttackModeEvents;
	CUtlVector<float> m_ViewAnimDurations;
	CWeaponInaccuracyCalculator m_InaccuracyCalculator;

	const WeaponAtts::WABaseAttack* m_pPrimaryAttackMode = nullptr;
	const WeaponAtts::WABaseAttack* m_pSecondaryAttackMode = nullptr;

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
inline const WeaponAtts::WABaseAttack* CGenericWeapon::GetAttackModeFromAttributes<WeaponAtts::WABaseAttack>(uint32_t index) const
{
	const WeaponAtts::WACollection& atts = WeaponAttributes();
	ASSERT(index < atts.AttackModes.Count());

	if ( index >= atts.AttackModes.Count() )
	{
		return nullptr;
	}

	return atts.AttackModes[index].get();
}

template<>
inline const WeaponAtts::WABaseAttack* CGenericWeapon::GetPrimaryAttackMode() const
{
	return m_pPrimaryAttackMode;
}

template<>
inline const WeaponAtts::WABaseAttack* CGenericWeapon::GetSecondaryAttackMode() const
{
	return m_pSecondaryAttackMode;
}

class CGenericAmmo : public CBasePlayerAmmo
{
public:
	enum SpawnFlag
	{
		SF_DontDrop = (1 << 0)
	};

	CGenericAmmo(const char* modelName, const CAmmoDef& ammoDef, const char* pickupSoundName = NULL)
		: CBasePlayerAmmo(),
		  m_szModelName(modelName),
		  m_AmmoDef(ammoDef),
		  m_szPickupSoundName(pickupSoundName)
	{
		ASSERT(m_szModelName);
	}

	void Spawn()
	{
		Precache();
		SET_MODEL(ENT(pev), m_szModelName);
		CBasePlayerAmmo::Spawn();

		if ( pev->spawnflags & SF_DontDrop )
		{
			pev->movetype = MOVETYPE_NONE;
		}
	}

	void Precache()
	{
		PRECACHE_MODEL(m_szModelName);
		PRECACHE_SOUND(m_szPickupSoundName ? m_szPickupSoundName : DEFAULT_PICKUP_SOUND);
	}

	BOOL AddAmmo(CBaseEntity *pOther)
	{
		if( pOther->GiveAmmo(m_AmmoDef.AmmoBoxGive, m_AmmoDef.AmmoName, m_AmmoDef.MaxCarry) != -1 )
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
