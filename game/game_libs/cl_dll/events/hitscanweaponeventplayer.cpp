#include "hitscanweaponeventplayer.h"
#include "event_args.h"
#include "cdll_int.h"
#include "cl_dll.h"
#include "event_api.h"
#include "vector_classes.h"
#include "eventscripts.h"
#include "hud.h"
#include "cl_util.h"
#include "view.h"
#include "genericweapon.h"
#include "pm_defs.h"
#include "ev_hldm.h"
#include "rapidjson/document.h"
#include "rapidjson_helpers/rapidjson_helpers.h"
#include "debugging/hitscanweapondebugging.h"
#include "eventCommands.h"
#include "gameplay/inaccuracymodifiers.h"

namespace
{
	static inline void Debug_BeginBatch()
	{
		if ( EventCommands::HitscanEventDebuggingEnabled() )
		{
			HitscanWeaponDebugging::BeginBatch();
		}
	}

	static inline void Debug_AddTrace(const Vector& begin, const Vector& end)
	{
		if ( EventCommands::HitscanEventDebuggingEnabled() )
		{
			HitscanWeaponDebugging::AddTraceToBatch(begin, end);
		}
	}

	static inline void Debug_EndBatch()
	{
		if ( EventCommands::HitscanEventDebuggingEnabled() )
		{
			HitscanWeaponDebugging::EndBatch();
		}
	}
}

void HitscanWeaponEventPlayer::EventStart()
{
	if( EV_IsLocal(m_iEntIndex) )
	{
		EV_MuzzleFlash();
		AnimateViewModel();
		V_PunchAxis(0, m_pHitscanAttack->ViewPunchY);
	}

	EjectShellFromViewModel(m_iShellModelIndex, m_pHitscanAttack->ShellModelType);
	PlayFireSound();
	CreateBulletTracers();
}

void HitscanWeaponEventPlayer::CreateBulletTracers()
{
	const uint32_t numShots = m_pHitscanAttack->BulletsPerShot;

	WeaponAtts::AccuracyParameters accuracyParams(m_pHitscanAttack->Accuracy);

	if ( InaccuracyModifiers::IsInaccuracyDebuggingEnabled() )
	{
		InaccuracyModifiers::GetInaccuracyValuesFromDebugCvars(accuracyParams);
	}

	HitscanWeaponDebugging::Clear();
	Debug_BeginBatch();

	for ( uint32_t shot = 0; shot < numShots; ++shot )
	{
		vec3_t shotDir;
		const Vector2D spread = InaccuracyModifiers::GetInterpolatedSpread(accuracyParams, m_flSpreadInterp);
		float spreadX = 0.0f;
		float spreadY = 0.0f;

		CGenericWeapon::GetSharedCircularGaussianSpread(shot, m_iRandomSeed, spreadX, spreadY);

		for ( uint8_t axis = 0; axis < 3; ++axis )
		{
			shotDir[axis] = m_vecFwd[axis] + (spreadX * spread.x * m_vecRight[axis]) + (spreadY * spread.y * m_vecUp[axis]);
		}

		vec3_t traceEnd = m_vecGunPosition + (CGenericWeapon::DEFAULT_BULLET_TRACE_DISTANCE * shotDir);

		gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction(false, true);

		// Store off the old count
		gEngfuncs.pEventAPI->EV_PushPMStates();

		// Now add in all of the players.
		gEngfuncs.pEventAPI->EV_SetSolidPlayers(m_iEntIndex - 1);

		gEngfuncs.pEventAPI->EV_SetTraceHull(2);	// TODO: What's 2?

		pmtrace_t traceResult;
		gEngfuncs.pEventAPI->EV_PlayerTrace(m_vecGunPosition, traceEnd, PM_STUDIO_BOX, -1, &traceResult);

		// Only actually draw trace lines on multiples of the trace stride.
		if ( m_iTracerStride < 2 || (m_iShotsFired % m_iTracerStride) == 0 )
		{
			EV_HLDM_CheckTracer(m_iEntIndex, m_vecGunPosition, traceResult.endpos, m_vecFwd, m_vecRight, BULLET_GENERIC);
		}

		// Do damage + paint decals.
		if ( traceResult.fraction != 1.0 )
		{
			EV_HLDM_PlayTextureSound(m_iEntIndex, &traceResult, m_vecGunPosition, traceEnd, BULLET_GENERIC);
			EV_HLDM_DecalGunshotNew(m_iEntIndex, &traceResult, m_vecGunPosition, traceEnd);
		}

		Debug_AddTrace(m_vecGunPosition, traceResult.endpos);

		if ( m_iTracerStride > 1 )
		{
			m_iShotsFired = (m_iShotsFired + 1) % m_iTracerStride;
		}

		gEngfuncs.pEventAPI->EV_PopPMStates();
	}

	Debug_EndBatch();
}

bool HitscanWeaponEventPlayer::Initialise()
{
	if ( !BaseWeaponEventPlayer::Initialise() )
	{
		return false;
	}

	m_pHitscanAttack = dynamic_cast<const WeaponAtts::WAHitscanAttack*>(m_pAttackMode);
	if ( !m_pHitscanAttack )
	{
		return false;
	}

	m_iShellModelIndex = gEngfuncs.pEventAPI->EV_FindModelIndex(m_pHitscanAttack->ShellModelName);
	m_iRandomSeed = m_pEventArgs->iparam1;
	m_flSpreadInterp = m_pEventArgs->fparam1;

	return true;
}

void HitscanWeaponEventPlayer::ParseEventScript(const rapidjson::Document& document)
{
	m_iTracerStride = rapidjson::GetProperty<int>(document, "tracerStride", rapidjson::kNumberType, 1);
}
