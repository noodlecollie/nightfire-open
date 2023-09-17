#include "baseweaponeventplayer.h"
#include "hitscanweaponeventplayer.h"
#include "weaponregistry.h"
#include "EnginePublicAPI/event_args.h"
#include "EnginePublicAPI/cdll_int.h"
#include "cl_dll.h"
#include "EnginePublicAPI/event_api.h"
#include "eventscripts.h"
#include "hud.h"
#include "cl_util.h"
#include "view.h"
#include "in_defs.h"
#include "rapidjson/document.h"
#include "rapidjson_helpers/rapidjson_helpers.h"
#include "weaponatts_collection.h"
#include "MathLib/angles.h"
#include "MathLib/utils.h"

namespace
{
	// TODO: These probably need to be different per weapon?
	static constexpr float SHELLEJECT_FWD_SCALE = 20;
	static constexpr float SHELLEJECT_RIGHT_SCALE = -12;
	static constexpr float SHELLEJECT_UP_SCALE = 4;
}  // namespace

void BaseWeaponEventPlayer::LoadEventScript(const CUtlString& path)
{
	rapidjson::Document document;

	if ( !rapidjson::LoadJsonFile(path, document, "BaseWeaponEventPlayer") )
	{
		return;
	}

	if ( !document.IsObject() )
	{
		ALERT(at_error, "BaseWeaponEventPlayer: Weapon script %s root is not an object.\n", path.String());

		return;
	}

	ParseEventScript(document);
}

void BaseWeaponEventPlayer::PlayEvent(
	const event_args_t* eventArgs,
	const WeaponAtts::WABaseAttack::AttackModeSignature* signature)
{
	m_pEventArgs = eventArgs;
	m_pSignature = signature;

	if ( !Initialise() )
	{
		ASSERT(false);
		return;
	}

	EventStart();
}

void BaseWeaponEventPlayer::ParseEventScript(const rapidjson::Document&)
{
}

bool BaseWeaponEventPlayer::Initialise()
{
	// Cache all the variables we need to use when generating trace effects.

	if ( !m_pSignature || !m_pEventArgs )
	{
		return false;
	}

	const uint32_t attackModeIndex = static_cast<uint8_t>(m_pSignature->Index);
	const WeaponId_e weaponId = static_cast<WeaponId_e>(m_pSignature->WeaponId);
	const WeaponAtts::WACollection* atts = CWeaponRegistry::StaticInstance().Get(weaponId);

	if ( !atts || attackModeIndex >= static_cast<uint32_t>(atts->AttackModes.Count()) )
	{
		return false;
	}

	m_pAttackMode = atts->AttackModes[attackModeIndex].get();

	m_iEntIndex = m_pEventArgs->entindex;
	m_bWeaponIsEmpty = m_pEventArgs->bparam1;

	// We make local copies of the event vectors so that they can be operated on as objects instead of arrays.
	VectorCopy(m_pEventArgs->angles, m_vecEntAngles);
	VectorCopy(m_pEventArgs->origin, m_vecEntOrigin);
	VectorCopy(m_pEventArgs->velocity, m_vecEntVelocity);

	AngleVectors(m_vecEntAngles, m_vecFwd, m_vecRight, m_vecUp);
	EV_GetGunPosition(m_pEventArgs, m_vecGunPosition, m_vecEntOrigin);

	return true;
}

void BaseWeaponEventPlayer::AnimateViewModel()
{
	const WeightedValueList<int>* animList =
		m_bWeaponIsEmpty ? &m_pAttackMode->ViewModelAnimList_AttackEmpty : &m_pAttackMode->ViewModelAnimList_Attack;

	if ( m_bWeaponIsEmpty && animList->Count() < 1 )
	{
		// If the weapon doesn't have an animation for firing on empty,
		// fall back to the normal animation.
		animList = &m_pAttackMode->ViewModelAnimList_Attack;
	}

	const int animIndex = animList->ItemByProbabilisticValue(gEngfuncs.pfnRandomFloat(0.0f, 1.0f));

	int body = m_pAttackMode->ViewModelBodyOverride;

	if ( body < 0 )
	{
		// No model override, so use weapon's current body.
		const struct cl_entity_s* const viewModelEnt = GetViewEntity();

		if ( viewModelEnt )
		{
			body = viewModelEnt->curstate.body;
		}
		else
		{
			body = 0;
		}
	}

	gEngfuncs.pEventAPI->EV_WeaponAnimation(animIndex, body);
}

void BaseWeaponEventPlayer::EjectShellFromViewModel(int shellIndex, ShellType shellType)
{
	vec3_t ShellVelocity;
	vec3_t ShellOrigin;

	// TODO: This is an awful function call. Refactor?
	EV_GetDefaultShellInfo(
		m_pEventArgs,
		m_vecEntOrigin,
		m_vecEntVelocity,
		ShellVelocity,
		ShellOrigin,
		m_vecFwd,
		m_vecRight,
		m_vecUp,
		SHELLEJECT_FWD_SCALE,
		SHELLEJECT_RIGHT_SCALE,
		SHELLEJECT_UP_SCALE);

	EV_EjectBrass(
		ShellOrigin,
		ShellVelocity,
		m_vecEntAngles[YAW],
		shellIndex,
		shellType == ShellType::Shotgun ? TE_BOUNCE_SHOTSHELL : TE_BOUNCE_SHELL);
}

void BaseWeaponEventPlayer::PlayFireSound()
{
	const WeaponAtts::WASoundSet& soundSet = (EV_IsLocal(m_iEntIndex) && m_pAttackMode->HasViewModelAttackSounds())
		? m_pAttackMode->ViewModelAttackSounds
		: m_pAttackMode->AttackSounds;

	const WeightedValueList<const char*>& soundNames = soundSet.SoundNames;

	if ( soundNames.Count() < 1 )
	{
		return;
	}

	const float volume = (soundSet.MinVolume < soundSet.MaxVolume)
		? gEngfuncs.pfnRandomFloat(soundSet.MinVolume, soundSet.MaxVolume)
		: soundSet.MaxVolume;

	const int pitch = (soundSet.MinPitch < soundSet.MaxPitch)
		? gEngfuncs.pfnRandomLong(soundSet.MinPitch, soundSet.MaxPitch)
		: soundSet.MaxPitch;

	const char* const soundName = soundNames.ItemByProbabilisticValue(gEngfuncs.pfnRandomFloat(0.0f, 1.0f));

	gEngfuncs.pEventAPI->EV_PlaySound(m_iEntIndex, m_vecEntOrigin, CHAN_WEAPON, soundName, volume, ATTN_NORM, 0, pitch);
}
