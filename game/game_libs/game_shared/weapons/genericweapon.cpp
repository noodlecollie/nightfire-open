#include "genericweapon.h"
#include "studio_utils_shared.h"
#include "util.h"
#include "weaponslots.h"
#include "gamerules.h"
#include "gameplay/inaccuracymodifiers.h"
#include "EnginePublicAPI/weaponinfo.h"
#include "util/extramath.h"
#include "util/cvarFuncs.h"
#include "weaponmechanics/basemechanic.h"

#ifdef CLIENT_DLL
#include "cl_dll.h"
#include "EnginePublicAPI/cl_entity.h"
#endif

#ifndef CLIENT_DLL
// Just randomly externing things like this is disgusting,
// but it's consistent with how the rest of the HL code
// is written. Fix this later.
extern int gmsgCurWeaponPriAttackMode;
#endif

CGenericWeapon::CGenericWeapon() :
	CBasePlayerWeapon()
{
}

CGenericWeapon::~CGenericWeapon()
{
	m_PrimaryAttackMechanicIndex = -1;
	m_SecondaryAttackMechanicIndex = -1;
	m_Mechanics.PurgeAndDeleteElements();
}

void CGenericWeapon::Spawn()
{
	const WeaponAtts::WACore& core = WeaponAttributes().Core;
	core.Validate();

	pev->classname = MAKE_STRING(core.Classname);  // hack to allow for old names
	Precache();
	m_iId = static_cast<int>(core.Id);
	FindWeaponSlotInfo();

	const WeaponAtts::WAPlayerModel& pm = WeaponAttributes().PlayerModel;

	if ( pm.WorldModelName )
	{
		SET_MODEL(ENT(pev), pm.WorldModelName);
		pev->body = pm.WorldModelBody;
		pev->skin = pm.WorldModelSkin;
	}

	m_iDefaultAmmo = WeaponAttributes().Ammo.PrimaryAmmoOnFirstPickup;

	m_InaccuracyCalculator.Clear();
	m_flInaccuracy = 0.0f;

	if ( !(pev->spawnflags & SF_DontDrop) )
	{
		FallInit();
	}
	else
	{
		Materialize();
	}
}

void CGenericWeapon::Precache()
{
	const WeaponAtts::WACollection& atts = WeaponAttributes();

	PrecacheCore(atts.Core);
	PrecacheViewModel(atts.ViewModel);
	PrecachePlayerModel(atts.PlayerModel);

	FOR_EACH_VEC(m_Mechanics, index)
	{
		m_Mechanics[index]->Precache();
	}
}

void CGenericWeapon::PrecacheSoundSet(const WeaponAtts::WASoundSet& sounds)
{
	const WeightedValueList<const char*>& soundList = sounds.SoundNames;

	for ( uint32_t index = 0; index < soundList.Count(); ++index )
	{
		PRECACHE_SOUND(soundList.Value(index));
	}
}

void CGenericWeapon::PrecacheCore(const WeaponAtts::WACore& core)
{
	if ( core.PickupSoundOverride )
	{
		// Default pickup sound is precached as part of the sound resources system.
		PRECACHE_SOUND(core.PickupSoundOverride);
	}
}

void CGenericWeapon::PrecacheViewModel(const WeaponAtts::WAViewModel& viewModel)
{
	if ( viewModel.ModelName )
	{
		m_iViewModelIndex = PRECACHE_MODEL(viewModel.ModelName);
		StudioGetAnimationDurations(m_iViewModelIndex, m_ViewAnimDurations);
	}

	PrecacheSoundSet(viewModel.Animations.ReloadSounds);
}

void CGenericWeapon::PrecachePlayerModel(const WeaponAtts::WAPlayerModel& playerModel)
{
	if ( playerModel.PlayerModelName )
	{
		PRECACHE_MODEL(playerModel.PlayerModelName);
	}

	if ( playerModel.WorldModelName )
	{
		PRECACHE_MODEL(playerModel.WorldModelName);
	}
}

int CGenericWeapon::GetItemInfo(ItemInfo* p)
{
	FindWeaponSlotInfo();

	const WeaponAtts::WACollection& atts = WeaponAttributes();

	p->pszName = STRING(pev->classname);
	p->iMaxClip = atts.Ammo.MaxClip;
	p->iSlot = m_iWeaponSlot;
	p->iPosition = m_iWeaponSlotPosition;
	p->iFlags = atts.Core.Flags;
	p->iId = m_iId = static_cast<int>(atts.Core.Id);
	p->iWeight = atts.Core.SwitchWeight;

	const CAmmoDef* primaryAmmo = atts.Ammo.PrimaryAmmo;
	if ( primaryAmmo )
	{
		p->pszAmmo1 = primaryAmmo->AmmoName;
		p->iMaxAmmo1 = primaryAmmo->MaxCarry;
	}
	else
	{
		p->pszAmmo1 = NULL;
		p->iMaxAmmo1 = -1;
	}

	const CAmmoDef* secondaryAmmo = atts.Ammo.SecondaryAmmo;
	if ( secondaryAmmo )
	{
		p->pszAmmo2 = secondaryAmmo->AmmoName;
		p->iMaxAmmo2 = secondaryAmmo->MaxCarry;
	}
	else
	{
		p->pszAmmo2 = NULL;
		p->iMaxAmmo2 = -1;
	}

	return 1;
}

int CGenericWeapon::AddToPlayer(CBasePlayer* pPlayer)
{
	if ( !CBasePlayerWeapon::AddToPlayer(pPlayer) )
	{
		return FALSE;
	}

	MESSAGE_BEGIN(MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev);
	WRITE_BYTE(m_iId);
	MESSAGE_END();
	return TRUE;
}

BOOL CGenericWeapon::Deploy()
{
	ResetEnqueuedMechanic();

	const WeaponAtts::WACollection& atts = WeaponAttributes();
	const WeaponAtts::ViewModelAnimationSet& anims = GetViewModelAnimationSet();

	return DefaultDeploy(
		atts.ViewModel.ModelName,
		atts.PlayerModel.PlayerModelName,
		anims.Anim_Draw,
		atts.PlayerModel.PlayerAnimExtension,
		m_iViewModelBody);
}

void CGenericWeapon::Holster(int skiplocal)
{
	ResetEnqueuedMechanic();
	CBasePlayerWeapon::Holster(skiplocal);
}

void CGenericWeapon::PrimaryAttack()
{
	if ( InvokeAttack(WeaponAtts::AttackMode::Primary) )
	{
		m_bPrimaryAttackThisFrame = true;
	}
}

void CGenericWeapon::SecondaryAttack()
{
	if ( InvokeAttack(WeaponAtts::AttackMode::Secondary) )
	{
		m_bSecondaryAttackThisFrame = true;
	}
}

bool CGenericWeapon::InvokeAttack(WeaponAtts::AttackMode mode)
{
	if ( mode == WeaponAtts::AttackMode::None || m_Mechanics.IsEmpty() )
	{
		return false;
	}

	if ( !PrepareToInvokeAttack(mode) )
	{
		return false;
	}

	WeaponMechanics::CBaseMechanic* mechanic = GetAttackMechanic(mode);

	if ( !mechanic )
	{
		return false;
	}

	return InvokeMechanic(mode, mechanic, 0);
}

// TODO: Allow this to handle secondary too? Do we need this?
void CGenericWeapon::Reload()
{
	if ( !CanReload() )
	{
		return;
	}

	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = GetPrimaryAmmoBasedAttackMode();

	if ( !ammoAttack )
	{
		// Checked in CanReload(), so this should never happen.
		ASSERT(false);
		return;
	}

	const WeaponAtts::WACollection& atts = WeaponAttributes();
	const int maxClip = atts.Ammo.MaxClip;

	if ( ammoAttack->SpecialReload )
	{
		// Reload is more complicated, so let the derived weapon class do it.
		m_fInSpecialReload = HandleSpecialReload(m_fInSpecialReload);
		return;
	}

	const WeaponAtts::ViewModelAnimationSet& anims = GetViewModelAnimationSet();

	const WeightedValueList<int>* reloadAnimList = m_iClip < 1 ? &anims.AnimList_ReloadEmpty : &anims.AnimList_Reload;

	if ( m_iClip < 1 && reloadAnimList->Count() < 1 )
	{
		reloadAnimList = &anims.AnimList_Reload;
	}

	int anim = reloadAnimList->Count() > 0
		? reloadAnimList->ItemByProbabilisticValue(UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1))
		: -1;

	float animDuration = ammoAttack->ReloadDuration;

	if ( animDuration < 0.0f )
	{
		animDuration = anim >= 0 ? ViewModelAnimationDuration(anim) : 0;
	};

	if ( DefaultReload(maxClip, anim, animDuration, m_iViewModelBody) )
	{
		PlaySound(anims.ReloadSounds);
		SetNextIdleTime(animDuration + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1), true);
	}
}

void CGenericWeapon::ItemPostFrame()
{
	m_bPrimaryAttackThisFrame = false;
	m_bSecondaryAttackThisFrame = false;

	m_InaccuracyCalculator.SetLastSmoothedInaccuracy(m_flInaccuracy);
	m_InaccuracyCalculator.SetAccuracyParams(GetWeaponAccuracyParams());
	m_InaccuracyCalculator.SetPlayer(m_pPlayer);
	m_InaccuracyCalculator.CalculateInaccuracy();

	const bool canAttack = m_pPlayer->m_flNextAttack <= UTIL_WeaponTimeBase();

	if ( canAttack )
	{
		RunAttackLogic();
	}

	UpdateValuesPostFrame();
}

void CGenericWeapon::RunAttackLogic()
{
	WeaponMechanics::CBaseMechanic* enqueuedMechanic = GetEnqueuedMechanic();

	if ( enqueuedMechanic && m_flEnqueuedMechanicInvocationTime <= UTIL_WeaponTimeBase() )
	{
		InvokeMechanic(
			static_cast<WeaponAtts::AttackMode>(m_EnqueuedAttackMode),
			enqueuedMechanic,
			m_NextEnqueuedAttackStep);
	}

	WeaponTick();

	if ( (m_fInReload) && (m_pPlayer->m_flNextAttack <= UTIL_WeaponTimeBase()) )
	{
		PerformReload();
	}

	if ( !(m_pPlayer->pev->button & IN_ATTACK) )
	{
		m_flLastFireTime = 0.0f;
		m_bPrimaryAttackHeldDown = false;
	}

	if ( !(m_pPlayer->pev->button & IN_ATTACK2) )
	{
		m_bSecondaryAttackHeldDown = false;
	}

	if ( ShouldSecondaryAttackThisFrame() )
	{
		WeaponMechanics::CBaseMechanic* secondaryMechanic = GetSecondaryAttackMechanic();

		if ( secondaryMechanic )
		{
			SetFireOnEmptyState(secondaryMechanic->GetAttackMode());
		}

		SecondaryAttack();
		m_bSecondaryAttackHeldDown = true;
	}
	else if ( ShouldPrimaryAttackThisFrame() )
	{
		WeaponMechanics::CBaseMechanic* primaryMechanic = GetSecondaryAttackMechanic();

		if ( primaryMechanic )
		{
			SetFireOnEmptyState(primaryMechanic->GetAttackMode());
		}

		PrimaryAttack();
		m_bPrimaryAttackHeldDown = true;
	}
	else if ( m_pPlayer->pev->button & IN_RELOAD && iMaxClip() != WEAPON_NOCLIP && !m_fInReload )
	{
		// reload when reload is pressed, or if no buttons are down and weapon is empty.
		Reload();
	}
	else if ( !(m_pPlayer->pev->button & (IN_ATTACK | IN_ATTACK2)) )
	{
		// no fire buttons down
		m_fFireOnEmpty = FALSE;

		HandleNoButtonsDown_Server();
		HandleNoButtonsDown_Client();
	}
	else
	{
		// catch all
		if ( ShouldWeaponIdle() )
		{
			WeaponIdle();
		}
	}
}

void CGenericWeapon::PerformReload()
{
	// TODO: Ammo needs to be sync'd to client for this to work.
#ifndef CLIENT_DLL
	// complete the reload.
	int j = Q_min(iMaxClip() - m_iClip, m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]);

	// Add them to the clip
	m_iClip += j;
	m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= j;
#else
	// No idea why this is so arbitrary, but Half Life had it in...
	m_iClip += 10;
#endif

	m_fInReload = FALSE;
}

bool CGenericWeapon::ReloadUsableWeaponIfEmpty()
{
	// weapon is useable. Reload if empty and weapon has waited as long as it has to after firing
	if ( m_iClip == 0 && !(iFlags() & ITEM_FLAG_NOAUTORELOAD) &&
		 m_flNextPrimaryAttack < (UseDecrement() ? 0.0 : gpGlobals->time) )
	{
		Reload();
		return true;
	}

	return false;
}

void CGenericWeapon::UpdateValuesPostFrame()
{
	m_InaccuracyCalculator.SetFiredThisFrame(m_bPrimaryAttackThisFrame);
	m_InaccuracyCalculator.SetLastFireTime(m_flLastPrimaryAttack);
	m_InaccuracyCalculator.SetLastFireTimeIsDecremented(UseDecrement());
	m_InaccuracyCalculator.AddInaccuracyPenaltyFromFiring();
	m_flInaccuracy = m_InaccuracyCalculator.SmoothedInaccuracy();

	if ( IsActiveItem() )
	{
		m_pPlayer->m_flWeaponInaccuracy = m_flInaccuracy;
	}
}

void CGenericWeapon::PlayEmptySoundIfAllowed(const WeaponAtts::WABaseAttack& attackMode)
{
	if ( attackMode.PlayDryFireSoundOnEmpty )
	{
		PlayEmptySound();
	}
}

bool CGenericWeapon::ShouldSecondaryAttackThisFrame() const
{
	const bool buttonDown = m_pPlayer->pev->button & IN_ATTACK2;
	const bool canAttack = CanAttack(m_flNextSecondaryAttack, gpGlobals->time, UseDecrement());
	WeaponMechanics::CBaseMechanic* mechanic = GetSecondaryAttackMechanic();
	const WeaponAtts::WABaseAttack* mode = mechanic ? mechanic->GetAttackMode() : nullptr;
	const bool secAttackIsContinuous = mode && mode->IsContinuous;

	return buttonDown && canAttack && (secAttackIsContinuous || !m_bSecondaryAttackHeldDown);
}

bool CGenericWeapon::ShouldPrimaryAttackThisFrame() const
{
	const bool buttonDown = m_pPlayer->pev->button & IN_ATTACK;
	const bool canAttack = CanAttack(m_flNextPrimaryAttack, gpGlobals->time, UseDecrement());
	WeaponMechanics::CBaseMechanic* mechanic = GetPrimaryAttackMechanic();
	const WeaponAtts::WABaseAttack* mode = mechanic ? mechanic->GetAttackMode() : nullptr;
	const bool priAttackIsContinuous = mode && mode->IsContinuous;

	return buttonDown && canAttack && (priAttackIsContinuous || !m_bPrimaryAttackHeldDown);
}

void CGenericWeapon::HandleNoButtonsDown_Server()
{
#ifndef CLIENT_DLL
	if ( !IsUseable() && m_flNextPrimaryAttack < (UseDecrement() ? 0.0 : gpGlobals->time) )
	{
		// weapon isn't useable, switch.
		if ( !(iFlags() & ITEM_FLAG_NOAUTOSWITCHEMPTY) && g_pGameRules->GetNextBestWeapon(m_pPlayer, this) )
		{
			m_flNextPrimaryAttack = static_cast<float>((UseDecrement() ? 0.0 : gpGlobals->time) + 0.3f);
			return;
		}
	}
	else if ( ReloadUsableWeaponIfEmpty() )
	{
		return;
	}

	WeaponIdle();
#endif
}

void CGenericWeapon::HandleNoButtonsDown_Client()
{
#ifdef CLIENT_DLL
	if ( ReloadUsableWeaponIfEmpty() )
	{
		return;
	}

	WeaponIdle();
#endif
}

bool CGenericWeapon::ReadPredictionData(const weapon_data_t* from)
{
	if ( !CBasePlayerWeapon::ReadPredictionData(from) )
	{
		return false;
	}

	m_flInaccuracy = from->m_flInaccuracy;
	m_PrimaryAttackMechanicIndex = from->m_PrimaryAttackMechanicIndex;
	m_SecondaryAttackMechanicIndex = from->m_SecondaryAttackMechanicIndex;
	m_EnqueuedMechanicIndex = from->m_EnqueuedMechanicIndex;
	m_NextEnqueuedAttackStep = from->m_NextEnqueuedAttackStep;
	m_flEnqueuedMechanicInvocationTime = from->m_flEnqueuedMechanicInvocationTime;
	return true;
}

bool CGenericWeapon::WritePredictionData(weapon_data_t* to)
{
	if ( !CBasePlayerWeapon::WritePredictionData(to) )
	{
		return false;
	}

	to->m_flInaccuracy = m_flInaccuracy;
	to->m_PrimaryAttackMechanicIndex = m_PrimaryAttackMechanicIndex;
	to->m_SecondaryAttackMechanicIndex = m_SecondaryAttackMechanicIndex;
	to->m_EnqueuedMechanicIndex = m_EnqueuedMechanicIndex;
	to->m_NextEnqueuedAttackStep = m_NextEnqueuedAttackStep;
	to->m_flEnqueuedMechanicInvocationTime = m_flEnqueuedMechanicInvocationTime;
	return true;
}

int CGenericWeapon::UpdateClientData(CBasePlayer* pPlayer)
{
	CBasePlayerWeapon::UpdateClientData(pPlayer);

#ifndef CLIENT_DLL
	if ( !IsActiveItem() )
	{
		return 1;
	}

	byte currentAttackMode = GetPrimaryAttackModeIndex();

	if ( pPlayer->m_pActiveItem == pPlayer->m_pClientActiveItem && currentAttackMode == m_iLastPriAttackMode )
	{
		// Same weapon as last frame and no mode change, no need to update.
		return 1;
	}

	// Update the HUD with the new mode.
	MESSAGE_BEGIN(MSG_ONE, gmsgCurWeaponPriAttackMode, nullptr, pPlayer->pev);
	WRITE_BYTE(m_iId);
	WRITE_BYTE(currentAttackMode);
	MESSAGE_END();

	m_iLastPriAttackMode = currentAttackMode;
#endif

	return 1;
}

void CGenericWeapon::SetFireOnEmptyState(const WeaponAtts::WABaseAttack* attackMode)
{
	if ( !attackMode )
	{
		return;
	}

	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(attackMode);

	if ( !ammoAttack )
	{
		return;
	}

	SetFireOnEmptyState(ammoAttack->UsesAmmoPool);
}

void CGenericWeapon::SetFireOnEmptyState(WeaponAtts::WAAmmoBasedAttack::AmmoPool pool)
{
	const char* ammoName = nullptr;
	int ammoIndex = -1;

	switch ( pool )
	{
		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary:
		{
			ammoName = pszAmmo1();
			ammoIndex = PrimaryAmmoIndex();
			break;
		}

		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Secondary:
		{
			ammoName = pszAmmo2();
			ammoIndex = SecondaryAmmoIndex();
			break;
		}

		default:
		{
			return;
		}
	}

	if ( ammoName && ammoIndex >= 0 && m_pPlayer->m_rgAmmo[ammoIndex] < 1 )
	{
		m_fFireOnEmpty = TRUE;
	}
}

int CGenericWeapon::HandleSpecialReload(int)
{
	return 0;
}

void CGenericWeapon::WeaponIdle()
{
	FOR_EACH_VEC(m_Mechanics, index)
	{
		m_Mechanics[index]->WeaponIdle();
	}

	ResetEmptySound();

	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
	{
		return;
	}

	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = GetPrimaryAmmoBasedAttackMode();

	if ( (ammoAttack && ammoAttack->SpecialReload) ? IdleProcess_CheckSpecialReload() : IdleProcess_CheckReload() )
	{
		// Reload did something, so don't play idle animations;
		return;
	}

	IdleProcess_PlayIdleAnimation();
}

bool CGenericWeapon::IdleProcess_CheckReload()
{
	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = GetPrimaryAmmoBasedAttackMode();

	// m_iClip must be compared exactly with 0 here - a value of -1 indicates clips are not relevant!
	if ( ammoAttack && ammoAttack->AutoReload && m_iClip == 0 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] > 0 )
	{
		Reload();
		return true;
	}

	return false;
}

bool CGenericWeapon::IdleProcess_CheckSpecialReload()
{
	if ( m_fInSpecialReload > 0 ||  // Haven't finished reloading yet
		 (m_iClip < 1 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]) )  // Need to fire off reload sequence automatically
	{
		m_fInSpecialReload = HandleSpecialReload(m_fInSpecialReload);
		return true;
	}

	return false;
}

void CGenericWeapon::IdleProcess_PlayIdleAnimation()
{
	const WeaponAtts::ViewModelAnimationSet& anims = GetViewModelAnimationSet();
	const WeightedValueList<int>* animList = m_iClip < 1 ? &anims.AnimList_IdleEmpty : &anims.AnimList_Idle;

	if ( m_iClip < 1 && animList->Count() < 1 )
	{
		animList = &anims.AnimList_Idle;
	}

	if ( animList->Count() < 1 )
	{
		return;
	}

	const float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0.0, 1.0);
	const int anim = animList->ItemByProbabilisticValue(flRand);

	SetNextIdleTime(ViewModelAnimationDuration(anim) + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1), true);
	SendWeaponAnim(anim, m_iViewModelBody);
}

void CGenericWeapon::PlaySound(const WeaponAtts::WASoundSet& sound, int channel, float volModifier)
{
	if ( sound.SoundNames.Count() < 1 )
	{
		return;
	}

	const float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0.0, 1.0);
	const char* soundName = sound.SoundNames.ItemByProbabilisticValue(flRand);
	const float volume = (sound.MinVolume < sound.MaxVolume)
		? UTIL_SharedRandomFloat(m_pPlayer->random_seed, sound.MinVolume, sound.MaxVolume)
		: sound.MaxVolume;
	const int pitch = (sound.MinPitch < sound.MaxPitch)
		? UTIL_SharedRandomLong(m_pPlayer->random_seed, sound.MinPitch, sound.MaxPitch)
		: sound.MaxPitch;

	EMIT_SOUND_DYN(
		ENT(m_pPlayer->pev),
		channel,
		soundName,
		volume * volModifier,
		sound.Attenuation,
		sound.Flags,
		pitch);
}

void CGenericWeapon::SetViewModelBody(int body, bool immediate)
{
	m_iViewModelBody = body;

#ifdef CLIENT_DLL
	if ( immediate )
	{
		struct cl_entity_s* viewModel = gEngfuncs.GetViewModel();
		if ( viewModel )
		{
			viewModel->curstate.body = body;
		}
	}
#else
	(void)immediate;
#endif
};

float CGenericWeapon::ViewModelAnimationDuration(int anim) const
{
	if ( anim < 0 || anim >= m_ViewAnimDurations.Count() )
	{
		return 0.0f;
	}

	return m_ViewAnimDurations[anim];
}

void CGenericWeapon::DelayPendingActions(float secs, bool allowIfEarlier)
{
	DelayFiring(secs, allowIfEarlier);
	SetNextIdleTime(secs, allowIfEarlier);
}

void CGenericWeapon::DelayFiring(float secs, bool allowIfEarlier, WeaponAtts::AttackMode mode)
{
	if ( mode == WeaponAtts::AttackMode::None || mode == WeaponAtts::AttackMode::Primary )
	{
		SetNextPrimaryAttack(secs, allowIfEarlier);
		m_flLastPrimaryAttack = UTIL_WeaponTimeBase();
	}

	if ( mode == WeaponAtts::AttackMode::None || mode == WeaponAtts::AttackMode::Secondary )
	{
		SetNextSecondaryAttack(secs, allowIfEarlier);
		m_flLastSecondaryAttack = UTIL_WeaponTimeBase();
	}
}

bool CGenericWeapon::HasAmmo(const WeaponAtts::WABaseAttack* attackMode, int minCount, bool useClip) const
{
	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(attackMode);

	if ( !ammoAttack )
	{
		// Treat as an infinite pool.
		return true;
	}

	return HasAmmo(ammoAttack->UsesAmmoPool, minCount, useClip);
}

bool CGenericWeapon::HasAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool pool, int minCount, bool useClip) const
{
	switch ( pool )
	{
		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary:
		{
			if ( useClip )
			{
				return m_iClip >= minCount;
			}
			else
			{
				return m_iPrimaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] >= minCount;
			}
		}

		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Secondary:
		{
			return m_iSecondaryAmmoType >= 0 && m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] >= minCount;
		}

		default:
		{
			// Treat as an infinite pool.
			return true;
		}
	}
}

bool CGenericWeapon::DecrementAmmo(const WeaponAtts::WABaseAttack* attackMode)
{
	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(attackMode);

	if ( !ammoAttack || ammoAttack->AmmoDecrement < 1 )
	{
		// Treat as an infinite pool.
		return true;
	}

	return DecrementAmmo(ammoAttack->UsesAmmoPool, ammoAttack->AmmoDecrement);
}

bool CGenericWeapon::DecrementAmmo(WeaponAtts::WAAmmoBasedAttack::AmmoPool pool, int decrement)
{
	// Really we should force the argument to be unsigned, but this will do for now.
	ASSERTSZ(decrement > 0, "Expected positive ammo decrement");

	switch ( pool )
	{
		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary:
		{
			if ( m_iClip < 0 )
			{
				// Weapon doesn't use clips, so decrement ammo pool directly.

				if ( m_iPrimaryAmmoType < 0 || m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] < decrement )
				{
					return false;
				}

				m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= decrement;
				return true;
			}

			if ( m_iClip < decrement )
			{
				return false;
			}

			m_iClip -= decrement;
			return true;
		}

		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Secondary:
		{
			if ( m_iSecondaryAmmoType < 0 || m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] < decrement )
			{
				return false;
			}

			m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] -= decrement;
			return true;
		}

		default:
		{
			// Treat as an infinite pool.
			return true;
		}
	}
}

int CGenericWeapon::AmmoLeft(const WeaponAtts::WABaseAttack* attackMode) const
{
	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = dynamic_cast<const WeaponAtts::WAAmmoBasedAttack*>(attackMode);

	if ( !ammoAttack )
	{
		// Treat as an infinite pool.
		return -1;
	}

	return AmmoLeft(ammoAttack->UsesAmmoPool);
}

int CGenericWeapon::AmmoLeft(WeaponAtts::WAAmmoBasedAttack::AmmoPool pool) const
{
	switch ( pool )
	{
		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Primary:
		{
			if ( m_iClip < 0 )
			{
				// Weapon doesn't use clips, so decrement ammo pool directly.

				if ( m_iPrimaryAmmoType < 0 )
				{
					return -1;
				}

				return m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType];
			}

			return m_iClip;
		}

		case WeaponAtts::WAAmmoBasedAttack::AmmoPool::Secondary:
		{
			if ( m_iSecondaryAmmoType < 0 )
			{
				return -1;
			}

			return m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType];
		}

		default:
		{
			// Treat as an infinite pool.
			return -1;
		}
	}
}

bool CGenericWeapon::CanPrimaryAttack() const
{
	return CanAttack(m_flNextPrimaryAttack, gpGlobals->time, UseDecrement());
}

bool CGenericWeapon::CanSecondaryAttack() const
{
	return CanAttack(m_flNextSecondaryAttack, gpGlobals->time, UseDecrement());
}

bool CGenericWeapon::CanPrimaryOrSecondaryAttack() const
{
	return CanPrimaryAttack() || CanSecondaryAttack();
}

bool CGenericWeapon::CanReload() const
{
	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = GetPrimaryAmmoBasedAttackMode();

	if ( !ammoAttack )
	{
		return false;
	}

	const WeaponAtts::WACollection& atts = WeaponAttributes();
	const int maxClip = atts.Ammo.MaxClip;

	if ( maxClip == WEAPON_NOCLIP || m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] < 1 || m_iClip == maxClip ||
		 m_flNextPrimaryAttack > UTIL_WeaponTimeBase() )
	{
		return false;
	}

	return true;
}

int CGenericWeapon::GetEventIDForAttackMode(const WeaponAtts::WABaseAttack* attack) const
{
	return (attack && attack->Signature()->Index < static_cast<uint32_t>(m_AttackModeEvents.Count()))
		? m_AttackModeEvents[attack->Signature()->Index]
		: -1;
}

WeaponMechanics::CBaseMechanic* CGenericWeapon::GetPrimaryAttackMechanic() const
{
	return GetMechanicByIndex(m_PrimaryAttackMechanicIndex);
}

void CGenericWeapon::SetPrimaryAttackMechanic(WeaponMechanics::CBaseMechanic* mechanic)
{
	SetMechanicIndex(mechanic, m_PrimaryAttackMechanicIndex);
}

WeaponMechanics::CBaseMechanic* CGenericWeapon::GetSecondaryAttackMechanic() const
{
	return GetMechanicByIndex(m_SecondaryAttackMechanicIndex);
}

void CGenericWeapon::SetSecondaryAttackMechanic(WeaponMechanics::CBaseMechanic* mechanic)
{
	SetMechanicIndex(mechanic, m_SecondaryAttackMechanicIndex);
}

WeaponMechanics::CBaseMechanic* CGenericWeapon::GetAttackMechanic(WeaponAtts::AttackMode mode) const
{
	switch ( mode )
	{
		case WeaponAtts::AttackMode::Primary:
		{
			return GetPrimaryAttackMechanic();
		}

		case WeaponAtts::AttackMode::Secondary:
		{
			return GetSecondaryAttackMechanic();
		}

		default:
		{
			return nullptr;
		}
	}
}

bool CGenericWeapon::PrepareToInvokeAttack(WeaponAtts::AttackMode)
{
	// By default, we allow all invocations.
	return true;
}

void CGenericWeapon::AttackInvoked(WeaponAtts::AttackMode mode, const WeaponMechanics::InvocationResult& result)
{
	if ( result.result == WeaponMechanics::InvocationResult::REJECTED_CANNOT_ATTACK_UNDERWATER && m_fFireOnEmpty )
	{
		PlayEmptySoundIfAllowed(*result.mechanic->GetAttackMode());
		DelayFiring(0.2f, false, mode);
	}
}

WeaponAtts::AttackMode CGenericWeapon::GetViewModelAnimationSource()
{
	return m_ViewModelAnimationSource;
}

void CGenericWeapon::SetViewModelAnimationSource(WeaponAtts::AttackMode source)
{
	m_ViewModelAnimationSource = source;
}

float CGenericWeapon::GetInaccuracy() const
{
	return m_flInaccuracy;
}

byte CGenericWeapon::GetPrimaryAttackModeIndex() const
{
	WeaponMechanics::CBaseMechanic* primaryMechanic = GetPrimaryAttackMechanic();

	if ( !primaryMechanic )
	{
		return 0;
	}

	const WeaponAtts::WABaseAttack* attackMode = primaryMechanic->GetAttackMode();
	const WeaponAtts::WACollection& atts = WeaponAttributes();
	const size_t attackModeCount = atts.AttackModes.Count();

	for ( size_t index = 0; index < attackModeCount; ++index )
	{
		if ( attackMode == atts.AttackModes[static_cast<int>(index)].get() )
		{
			// This cast is fine, since we should never exceed MAX_ATTACK_MODES.
			return static_cast<byte>(index);
		}
	}

	return 0;
}

const WeaponAtts::AccuracyParameters* CGenericWeapon::GetWeaponAccuracyParams() const
{
	// For now, base this off the primary attack mode only.
	WeaponMechanics::CBaseMechanic* primaryMechanic = GetPrimaryAttackMechanic();

	if ( !primaryMechanic )
	{
		return nullptr;
	}

	const WeaponAtts::WAAmmoBasedAttack* ammoAttack = primaryMechanic->GetAmmoBasedAttackMode();
	return ammoAttack ? &ammoAttack->Accuracy : nullptr;
}

const WeaponAtts::ViewModelAnimationSet& CGenericWeapon::GetViewModelAnimationSet() const
{
	if ( m_ViewModelAnimationSource != WeaponAtts::AttackMode::None )
	{
		const WeaponMechanics::CBaseMechanic* mechanic = m_ViewModelAnimationSource == WeaponAtts::AttackMode::Secondary
			? GetSecondaryAttackMechanic()
			: GetPrimaryAttackMechanic();

		const WeaponAtts::WABaseAttack* attackMode = mechanic ? mechanic->GetAttackMode() : nullptr;

		if ( attackMode && attackMode->OverrideAnimations )
		{
			return *attackMode->OverrideAnimations;
		}
	}

	return WeaponAttributes().ViewModel.Animations;
}

const char* CGenericWeapon::PickupSound() const
{
	const WeaponAtts::WACollection& atts = WeaponAttributes();

	if ( atts.Core.PickupSoundOverride )
	{
		return atts.Core.PickupSoundOverride;
	}

	return CBasePlayerWeapon::PickupSound();
}

int CGenericWeapon::iItemSlot()
{
	ASSERT(m_iWeaponSlot >= 0);
	return m_iWeaponSlot;
}

void CGenericWeapon::FindWeaponSlotInfo()
{
	if ( m_iWeaponSlot >= 0 && m_iWeaponSlotPosition >= 0 )
	{
		return;
	}

	const WeaponId_e id = WeaponAttributes().Core.Id;

	for ( int slot = 0; slot < MAX_WEAPON_SLOTS; ++slot )
	{
		for ( int position = 0; position < MAX_WEAPON_POSITIONS; ++position )
		{
			if ( WEAPON_HUD_SLOTS[slot][position] == id )
			{
				m_iWeaponSlot = slot;
				m_iWeaponSlotPosition = position;
				return;
			}
		}
	}

	ASSERTSZ(false, "No slot/position found for this weapon.");
}

bool CGenericWeapon::InvokeMechanic(WeaponAtts::AttackMode mode, WeaponMechanics::CBaseMechanic* mechanic, int step)
{
	const int invokeIndex = m_Mechanics.Find(mechanic);

	if ( IsValidMechanicIndex(m_EnqueuedMechanicIndex) && invokeIndex != m_EnqueuedMechanicIndex )
	{
		// We're invoking a new attack, so reset the old one that wasn't finished.
		ResetEnqueuedMechanic();
	}

	if ( !mechanic || !IsValidMechanicIndex(invokeIndex) )
	{
		SetEnqueuedMechanic(WeaponAtts::AttackMode::None, nullptr, 0);
		return false;
	}

	const WeaponMechanics::InvocationResult result = mechanic->Invoke(step);

	if ( result.result == WeaponMechanics::InvocationResult::INCOMPLETE )
	{
		SetEnqueuedMechanic(mode, mechanic, step + 1);
		m_flEnqueuedMechanicInvocationTime = result.nextInvocationTime;
	}
	else
	{
		SetEnqueuedMechanic(WeaponAtts::AttackMode::None, nullptr, 0);
	}

	AttackInvoked(mode, result);

	return !result.WasRejected();
}

WeaponMechanics::CBaseMechanic* CGenericWeapon::GetMechanicByIndex(int index) const
{
	return IsValidMechanicIndex(index) ? m_Mechanics[index] : nullptr;
}

void CGenericWeapon::SetMechanicIndex(WeaponMechanics::CBaseMechanic* mechanic, int& outIndex)
{
	const int index = mechanic ? m_Mechanics.Find(mechanic) : -1;

	if ( mechanic && index < 0 )
	{
		ASSERT(false);
		return;
	}

	outIndex = index;
}

WeaponMechanics::CBaseMechanic* CGenericWeapon::GetEnqueuedMechanic() const
{
	return GetMechanicByIndex(m_EnqueuedMechanicIndex);
}

void CGenericWeapon::SetEnqueuedMechanic(
	WeaponAtts::AttackMode mode,
	WeaponMechanics::CBaseMechanic* mechanic,
	int nextStep)
{
	SetMechanicIndex(mechanic, m_EnqueuedMechanicIndex);
	m_EnqueuedAttackMode = static_cast<int>(mechanic ? mode : WeaponAtts::AttackMode::None);
	m_NextEnqueuedAttackStep = std::max<int>(nextStep, 0);

	if ( !mechanic )
	{
		m_flEnqueuedMechanicInvocationTime = 0.0f;
	}
}

void CGenericWeapon::ResetEnqueuedMechanic()
{
	WeaponMechanics::CBaseMechanic* enqueuedMechanic = GetEnqueuedMechanic();

	if ( enqueuedMechanic )
	{
		enqueuedMechanic->Reset();
		SetEnqueuedMechanic(WeaponAtts::AttackMode::None, nullptr, 0);
	}
}

bool CGenericWeapon::IsValidMechanicIndex(int index) const
{
	return index >= 0 && index < m_Mechanics.Count();
}

WeaponAtts::AttackMode CGenericWeapon::GetAttackModeForMechanic(const WeaponMechanics::CBaseMechanic* mechanic) const
{
	if ( mechanic )
	{
		if ( GetPrimaryAttackMechanic() == mechanic )
		{
			return WeaponAtts::AttackMode::Primary;
		}

		if ( GetSecondaryAttackMechanic() == mechanic )
		{
			return WeaponAtts::AttackMode::Secondary;
		}
	}

	return WeaponAtts::AttackMode::None;
}

const WeaponAtts::WAAmmoBasedAttack* CGenericWeapon::GetPrimaryAmmoBasedAttackMode() const
{
	const WeaponMechanics::CBaseMechanic* primaryMechanic = GetPrimaryAttackMechanic();
	return primaryMechanic ? primaryMechanic->GetAmmoBasedAttackMode() : nullptr;
}

#ifndef CLIENT_DLL
TYPEDESCRIPTION CGenericWeapon::m_SaveData[] = {
	DEFINE_FIELD(CGenericWeapon, m_flInaccuracy, FIELD_FLOAT),
	DEFINE_FIELD(CGenericWeapon, m_PrimaryAttackMechanicIndex, FIELD_INTEGER),
	DEFINE_FIELD(CGenericWeapon, m_SecondaryAttackMechanicIndex, FIELD_INTEGER),
	DEFINE_FIELD(CGenericWeapon, m_EnqueuedMechanicIndex, FIELD_INTEGER),
	DEFINE_FIELD(CGenericWeapon, m_EnqueuedAttackMode, FIELD_INTEGER),
	DEFINE_FIELD(CGenericWeapon, m_NextEnqueuedAttackStep, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CGenericWeapon, CBasePlayerWeapon)
#endif
