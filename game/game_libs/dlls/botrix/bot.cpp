#include <cstdio>
#include <cmath>
#include <limits>

#include <good/string.h>
#include <good/string_buffer.h>

#include "botrix/bot.h"
#include "botrix/parameter_vars.h"
#include "EnginePublicAPI/eiface.h"
#include "botrix/type2string.h"
#include "engine_util.h"
#include "standard_includes.h"
#include "MathLib/utils.h"

bool CBotrixBot::bAssumeUnknownWeaponManual = false;
TBotIntelligence CBotrixBot::iMinIntelligence = EBotFool;
TBotIntelligence CBotrixBot::iMaxIntelligence = EBotPro;
TTeam CBotrixBot::iDefaultTeam = 0;
TClass CBotrixBot::iDefaultClass = -1;
int CBotrixBot::iChangeClassRound = 0;

TFightStrategyFlags CBotrixBot::iDefaultFightStrategy = FFightStrategyComeCloserIfFar;
float CBotrixBot::fNearDistanceSqr = SQR(200);
float CBotrixBot::fFarDistanceSqr = SQR(500);
float CBotrixBot::fInvalidWaypointSuicideTime = 10.0f;

float CBotrixBot::m_fTimeIntervalCheckUsingMachines = 0.5f;
int CBotrixBot::m_iCheckEntitiesPerFrame = 4;

#define NotifyIfFailed(exp, ...) \
	GOOD_SCOPE_START \
	if ( !(exp) ) \
	{ \
		BLOG_E(__VA_ARGS__); \
		BreakDebugger(); \
	} \
	GOOD_SCOPE_END

//----------------------------------------------------------------------------------------------------------------
CBotrixBot::CBotrixBot(edict_t* pEdict, TBotIntelligence iIntelligence, TClass iClass) :
	CPlayer(pEdict, true),
	m_iIntelligence(iIntelligence),
	m_iClass(iClass),
	m_iClassChange(0),
	r(rand() & 0xFF),
	g(rand() & 0xFF),
	b(rand() & 0xFF),
	m_aNearPlayers(CPlayers::Size()),
	m_aSeenEnemies(CPlayers::Size()),
	m_aEnemies(CPlayers::Size()),
	m_aAllies(CPlayers::Size()),
	m_iNextCheckPlayer(0),
	m_pCurrentEnemy(NULL),
	m_cAttackDuckRangeSqr(0, SQR(400)),
	m_bTest(false),
	m_bCommandAttack(true),
	m_bCommandPaused(false),
	m_bCommandStopped(false),
#if defined(DEBUG) || defined(_DEBUG)
	m_bDebugging(true),
#else
	m_bDebugging(false),
#endif
	m_bDontBreakObjects(false),
	m_bDontThrowObjects(false),
	m_bFeatureAttackDuckEnabled(iIntelligence < EBotNormal),
	m_bFeatureWeaponCheck(true),
	m_bSaidNoWaypoints(false)
{
	m_aPickedItems.reserve(16);
	for ( TItemType i = 0; i < EItemTypeCollisionTotal; ++i )
	{
		int iSize = CItems::GetItems(i).size() >> 4;
		if ( iSize == 0 )
		{
			iSize = 2;
		}
		m_aNearItems[i].reserve(iSize);
		m_aNearestItems[i].reserve(16);
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::ConsoleCommand(const char* szCommand)
{
	BotMessage("%s -> Executing command '%s'.", GetName(), szCommand);
	g_engfuncs.pfnClientCommand(m_pEdict, "%s", szCommand);
}

#ifdef BOTRIX_SEND_BOT_CHAT
//----------------------------------------------------------------------------------------------------------------
// Class used to send say messages to server.
//----------------------------------------------------------------------------------------------------------------
class UserRecipientFilter : public IRecipientFilter
{
public:
	UserRecipientFilter(int iUserIndex) :
		m_iUserIndex(iUserIndex)
	{
	}

	virtual bool IsReliable() const
	{
		return true;
	}
	virtual bool IsInitMessage() const
	{
		return false;
	}

	virtual int GetRecipientCount() const
	{
		return 1;
	}
	virtual int GetRecipientIndex(int) const
	{
		return m_iUserIndex;
	}

protected:
	int m_iUserIndex;
};

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::Say(bool bTeamOnly, const char* szFormat, ...)
{
	static char szBotBuffer[2048];
	good::string_buffer sBuffer(szBotBuffer, 2048, false);

	if ( bTeamOnly )
	{
		sBuffer << "(TEAM) " << GetName() << ": ";
	}

	// sBuffer << ( bTeamOnly ? "say_team " : "say" );

	int iSize = sBuffer.size();

	va_list vaList;
	va_start(vaList, szFormat);
	PlatformLib_VSNPrintF(szBotBuffer + iSize, sizeof(szBotBuffer) - iSize, szFormat, vaList);
	va_end(vaList);

	// CBotrixPlugin::pServerPluginHelpers->ClientCommand(m_pEdict, szBotBuffer);
	// CBotrixPlugin::pEngineServer->ClientCommand(m_pEdict, "%s", szBotBuffer);

	for ( int i = 0; i <= CPlayers::GetPlayersCount(); i++ )
	{
		CPlayer* pPlayer = CPlayers::Get(i);

		if ( !pPlayer || pPlayer->IsBot() )
			continue;

		if ( bTeamOnly && (pPlayer->GetTeam() != 0) && (pPlayer->GetTeam() != GetTeam()) )
			continue;

		UserRecipientFilter user(pPlayer->GetIndex() + 1);
		bf_write* bf = CBotrixPlugin::pEngineServer->UserMessageBegin(&user, 3);
		if ( bf )
		{
			bf->WriteByte(m_iIndex);
			bf->WriteString(szBotBuffer);
			bf->WriteByte(true);
		}
		CBotrixPlugin::pEngineServer->MessageEnd();
	}

	// Echo to server console.
	if ( CBotrixPlugin::pEngineServer->IsDedicatedServer() )
	{
		Msg("%s", szBotBuffer);
	}

	if ( !CBotrixPlugin::instance->GenerateSayEvent(m_pEdict, szBotBuffer, bTeamOnly) )
	{
		BLOG_E("%s -> %s", GetName(), szBotBuffer);
	}
}
#else
void CBotrixBot::Say(bool, const char*, ...)
{
}
#endif  // BOTRIX_SEND_BOT_CHAT

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::TestWaypoints(TWaypointId iFrom, TWaypointId iTo)
{
	BASSERT(CWaypoints::IsValid(iFrom) && CWaypoints::IsValid(iTo), CPlayers::KickBot(this));
	CWaypoint& wFrom = CWaypoints::Get(iFrom);

	Vector vSetOrigin = wFrom.vOrigin;

	// Make bot appear on the ground (waypoints are at eye level).
	// The player's origin is the middle of their bbox.
	vSetOrigin.z -= CBotrixParameterVars::PLAYER_EYE;
	vSetOrigin.z += (CBotrixParameterVars::PLAYER_HEIGHT / 2.0f) + 1;

	vSetOrigin.CopyToArray(m_pEdict->v.origin);

	iCurrentWaypoint = iFrom;
	m_iDestinationWaypoint = iTo;

	m_bDontAttack = m_bDestinationChanged = m_bNeedMove = m_bLastNeedMove = m_bUseNavigatorToMove = m_bTest = true;
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::AddWeapon(TWeaponId iWeaponId)
{
	if ( CWeapons::IsValid(iWeaponId) )  // As if grabbed a weapon: add default bullets and weapon.
	{
		GetPlayerClassPtr()->SelectItem(CWeapons::Get(iWeaponId)->GetBaseWeapon()->pWeaponClass->sClassName.c_str());
		m_aWeapons[iWeaponId].AddWeapon();
	}
	else
	{
		WeaponCheckCurrent(true);
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::Activated()
{
	CPlayer::Activated();
	BotDebug("%s -> Activated.", GetName());
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::Respawned()
{
	CPlayer::Respawned();
	BotDebug("%s -> Respawned.", GetName());

	m_bProtected = true;

	if ( iChangeClassRound && m_iClassChange )
	{
		m_iClassChange--;
	}

#ifdef BOTRIX_CHAT
	m_iPrevChatMate = m_iPrevTalk = -1;
	m_bTalkStarted = false;

	m_cChat.iSpeaker = m_iIndex;
	m_iObjective = EBotChatUnknown;

	m_bPerformingRequest = false;
#endif

	m_cCmd = {};
	m_cCmd.viewangles = m_pEdict->v.v_angle;

	m_cNavigator.Stop();

	// m_vLastVelocity.x = m_vLastVelocity.y = m_vLastVelocity.z = 0.0f;

	m_fPrevThinkTime = m_fStuckCheckTime = 0.0f;

	for ( TItemType i = 0; i < EItemTypeCollisionTotal; ++i )
	{
		m_iNextNearItem[i] = 0;
		m_aNearItems[i].clear();
		m_aNearestItems[i].clear();
	}

	m_aAvoidAreas.clear();

	m_iNextCheckPlayer = 0;
	m_aNearPlayers.reset();
	m_aSeenEnemies.reset();
	m_pCurrentEnemy = NULL;

	// Don't clear picked items, as bot still know which were taken previously.
	m_iCurrentPickedItem = 0;

	// Set default flags.
	m_bDontAttack = m_bFlee = m_bNeedSetWeapon = m_bNeedReload = m_bAttackDuck = false;

	m_bNeedAim = m_bUseSideLook = false;
	m_bDontAttack = m_bDestinationChanged = m_bNeedMove = m_bLastNeedMove = m_bUseNavigatorToMove = m_bTest;

	m_bPathAim = m_bLockNavigatorMove = m_bLockMove = false;
	m_bMoveFailure = false;

	m_bStuck = m_bResolvingStuck = m_bNeedCheckStuck = m_bStuckBreakObject = m_bStuckUsePhyscannon = false;
	m_bStuckTryingSide = m_bStuckTryGoLeft = m_bStuckGotoCurrent = m_bStuckGotoPrevious = m_bRepeatWaypointAction =
		false;

	m_bLadderMove = m_bNeedStop = m_bNeedDuck = m_bNeedWalk = m_bNeedSprint = false;
	m_bNeedFlashlight = m_bUsingFlashlight = false;
	m_bNeedUse = m_bAlreadyUsed = m_bUsingHealthMachine = m_bUsingArmorMachine = false;
	m_bNeedAttack = m_bNeedAttack2 = m_bNeedJumpDuck = m_bNeedJump = false;

	m_bInvalidWaypointStart = false;

	m_fNextDrawNearObjectsTime = 0.0f;

	// Check near items (skip objects).
	Vector vFoot = m_pEdict->v.origin;

	for ( TItemType iType = 0; iType < EItemTypeCollisionTotal; ++iType )
	{
		good::vector<TItemIndex>& aNear = m_aNearItems[iType];
		good::vector<TItemIndex>& aNearest = m_aNearestItems[iType];

		const good::vector<CItem>& aItems = CItems::GetItems(iType);

		if ( aItems.size() == 0 )
		{
			continue;
		}

		for ( TItemIndex i = 0; i < (int)aItems.size(); ++i )
		{
			const CItem* pItem = &aItems[i];

			if ( pItem->IsFree() || !pItem->IsOnMap() )  // Item is picked or broken.
			{
				continue;
			}

			Vector vOrigin = pItem->CurrentPosition();
			float fDistSqr = (vOrigin - vFoot).LengthSquared();

			if ( fDistSqr <= pItem->fPickupDistanceSqr )
			{
				aNearest.push_back(i);
			}
			else if ( fDistSqr <= CBotrixMod::iNearItemMaxDistanceSqr )  // Item is close.
			{
				aNear.push_back(i);
			}
		}
	}

	ConfigureRespawnWeapons();
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::ConfigureRespawnWeapons()
{
	if ( CBotrixMod::bRemoveWeapons )
	{
		WeaponsRemove();
	}

	if ( m_bFeatureWeaponCheck )
	{
		CWeapons::GetRespawnWeapons(m_aWeapons, m_PlayerInfo.GetTeamIndex(), m_iClass);
		WeaponsScan();

		if ( !CWeapons::IsValid(m_iWeapon) )
		{
			WeaponCheckCurrent(true);
		}
	}
	else
	{
		m_iMeleeWeapon = m_iPhyscannon = m_iBestWeapon = m_iWeapon = EWeaponIdInvalid;
	}

	for ( int i = 0; i < CBotrixMod::aDefaultWeapons.Count(); ++i )
	{
		TWeaponId iWeapon = CBotrixMod::aDefaultWeapons[i];

		if ( m_aWeapons[iWeapon].IsPresent() )  // First check if bot has that weapon.
		{
			ChangeWeapon(iWeapon);
		}
		else
		{
			AddWeapon(iWeapon);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::Dead()
{
	BotDebug("%s -> Dead.", GetName());
	CPlayer::Dead();

	if ( m_bTest )
	{
		CPlayers::KickBot(this);
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::PlayerDisconnect(int iPlayerIndex, CPlayer* pPlayer)
{
	m_aNearPlayers.reset(iPlayerIndex);
	m_aSeenEnemies.reset(iPlayerIndex);

	if ( pPlayer == m_pCurrentEnemy )
	{
		EraseCurrentEnemy();
	}
}

//----------------------------------------------------------------------------------------------------------------
#ifdef BOTRIX_CHAT
void CBotrixBot::ReceiveChatRequest(const CBotChat& cRequest)
{
	BASSERT((cRequest.iDirectedTo == m_iIndex) && (cRequest.iSpeaker != -1), return);
	if ( iChatMate == -1 )
		iChatMate = cRequest.iSpeaker;

	CBotChat cResponse(EBotChatUnknown, m_iIndex, cRequest.iSpeaker);  // TODO: m_cChat.
	if ( iChatMate == cRequest.iSpeaker )
	{
		// If conversation is not started, decide whether to help teammate or not (random).
		if ( !m_bTalkStarted )
		{
			m_bTalkStarted = true;
			m_iPrevTalk = -1;
			m_bHelpingMate = rand() & 1;
			if ( !m_bHelpingMate )  // Force to help admin of the server.
			{
				CPlayer* pSpeaker = CPlayers::Get(cRequest.iSpeaker);
				if ( !pSpeaker->IsBot() )
				{
					CClient* pClient = (CClient*)pSpeaker;
					if ( FLAG_ALL_SET_OR_0(FCommandAccessBot, pClient->iCommandAccessFlags) )
						m_bHelpingMate = true;
				}
			}
		}

		if ( m_iPrevTalk == -1 )  // This is a request from other player, generate response.
		{
			// We want to know what bot can answer.
			const good::vector<TBotChat>& aPossibleAnswers = CChat::PossibleAnswers(cRequest.iBotChat);
			if ( aPossibleAnswers.size() == 1 )
				cResponse.iBotChat = aPossibleAnswers[0];
			else if ( aPossibleAnswers.size() > 0 )
			{
				bool bAffirmativeNegativeAnswer =
					good::find(aPossibleAnswers.begin(), aPossibleAnswers.end(), EBotChatAffirm) !=
					aPossibleAnswers.end();
				if ( bAffirmativeNegativeAnswer )
				{
					bool bYesNoAnswer =
						good::find(aPossibleAnswers.begin(), aPossibleAnswers.end(), EBotChatAffirmative) !=
						aPossibleAnswers.end();
					cResponse.iBotChat =
						bYesNoAnswer ? ((rand() & 1) ? EBotChatAffirmative : EBotChatAffirm) : EBotChatAffirm;
					if ( m_bHelpingMate )
						StartPerformingChatRequest(cRequest);
					else
						cResponse.iBotChat += 1;  // Negate request (EBotChatAffirmative + 1 = EBotChatNegative).
				}
				else
					cResponse.iBotChat = aPossibleAnswers[rand() % aPossibleAnswers.size()];
			}
		}
		else  // This is a response to this bot's request.
		{
			// TODO: this part is not implemented yet.
			switch ( cRequest.iBotChat )
			{
				case EBotChatAffirm:
				case EBotChatAffirmative:

				case EBotChatNegative:
				case EBotChatNegate:
				case EBotChatBusy:
					// Ask another bot?
				default:;
			}
		}

		if ( cRequest.iBotChat == EBotChatBye )
		{
			m_iPrevChatMate = iChatMate;
			m_iPrevTalk = EBotChatBye;
			iChatMate = -1;
		}
	}
	else if ( cRequest.iBotChat == EBotChatBye )
	{
		if ( (m_iPrevTalk != EBotChatBye) || (m_iPrevChatMate != cRequest.iSpeaker) )
			cResponse.iBotChat = EBotChatBye;
	}
	else
		cResponse.iBotChat = EBotChatBusy;

	// Generate chat.
	if ( cResponse.iBotChat != -1 )
	{
		const good::string& sText = CChat::ChatToText(cResponse);
		Say(false, sText.c_str());
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::StartPerformingChatRequest(const CBotChat& cRequest)
{
	m_bPerformingRequest = true;
	m_iObjective = cRequest.iBotChat;

	switch ( m_iObjective )
	{
		case EBotChatStop:
			m_bNeedMove = false;
			m_bRequestTimeout = true;
			m_fEndTalkActionTime = CBotrixServerPlugin::GetTime() + 30.0f;
			break;
		case EBotChatCome:
		case EBotChatFollow:
		case EBotChatAttack:
		case EBotChatNoKill:
		case EBotChatSitDown:
		case EBotChatStandUp:
		case EBotChatJump:
		case EBotChatLeave:

		default:
			BASSERT(false);  // Should not enter.
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::EndPerformingChatRequest(bool bSayGoodbye)
{
	m_bPerformingRequest = m_bRequestTimeout = false;
	if ( bSayGoodbye )
	{
		m_cChat.cMap.clear();
		m_cChat.iBotChat = EBotChatBye;
		if ( rand() & 1 )
		{
			m_cChat.iDirectedTo = iChatMate;
			m_cChat.cMap.push_back(CChatVarValue(CChat::iPlayerVar, 0, iChatMate));
		}
		else
			m_cChat.iDirectedTo = -1;

		bool bTeam = (CPlayers::Get(iChatMate)->GetTeam() == GetTeam());
		Say(bTeam, CChat::ChatToText(m_cChat).c_str());
	}

	switch ( m_iObjective )
	{
		case EBotChatStop:
			m_bNeedMove = true;
			break;
		case EBotChatCome:
		case EBotChatFollow:
		case EBotChatAttack:
		case EBotChatNoKill:
		case EBotChatSitDown:
		case EBotChatStandUp:
		case EBotChatJump:
		case EBotChatLeave:

		default:
			BASSERT(false);  // Should not enter.
	}
}
#endif  // BOTRIX_CHAT

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::PreThink()
{
// #define DRAW_BEAM_TO_0_0_0
#if defined(DEBUG) && defined(DRAW_BEAM_TO_0_0_0)
	static float fBeamTo000 = 0.0f;
	if ( CBotrixServerPlugin::GetTime() > fBeamTo000 )
	{
		fBeamTo000 = CBotrixServerPlugin::GetTime() + 0.5f;
		CBotrixEngineUtil::DrawBeam(GetHead(), CBotrixEngineUtil::vZero, 5, 0.6f, 0xff, 0xff, 0xff);
	}
#endif

	Vector vPrevOrigin = m_vHead;
	int iPrevCurrWaypoint = iCurrentWaypoint;

	CPlayer::PreThink();  // Will invalidate current waypoint if away.

	if ( m_bCommandPaused )
	{
		return;
	}

	if ( m_pCurrentEnemy )
	{
		// Erase current enemy if the enemy is protected.
		if ( (m_pCurrentEnemy->IsProtected() ||
			  (CBotrixMod::iSpawnProtectionHealth > 0 &&
			   m_pCurrentEnemy->GetHealth() >= CBotrixMod::iSpawnProtectionHealth)) )
		{
			EraseCurrentEnemy();
		}
	}

	if ( CWaypoint::IsValid(iCurrentWaypoint) )
	{
		m_bInvalidWaypointStart = false;
	}
	else if ( m_bAlive && fInvalidWaypointSuicideTime )
	{
		if ( m_bInvalidWaypointStart )
		{
			if ( CBotrixServerPlugin::GetTime() >= m_fInvalidWaypointEnd )  // TODO: config.
			{
				BLOG_E("%s -> Staying away from waypoints too long, suiciding.", GetName());
				ConsoleCommand("kill");
			}
		}
		else
		{
			m_bInvalidWaypointStart = true;
			m_fInvalidWaypointEnd = CBotrixServerPlugin::GetTime() + CBotrixBot::fInvalidWaypointSuicideTime;
		}

		return;
	}

	// Reset bot's command.
	m_cCmd.forwardmove = 0;
	m_cCmd.sidemove = 0;
	m_cCmd.upmove = 0;
	m_cCmd.buttons = 0;
	m_cCmd.impulse = 0;

	if ( m_bAlive )  // Update near objects, items, players and current weapon.
	{
		UpdateWorld();
	}

	if ( !m_bTest )
	{
		Think();  // Mod's think.
	}

	if ( m_bAlive )
	{
		PerformMove(iPrevCurrWaypoint, vPrevOrigin);

		if ( m_bNeedMove && m_bUseNavigatorToMove && m_cNavigator.SearchEnded() )
		{
			m_cNavigator.DrawPath(r, g, b, m_vHead);
		}

		// Show near items in white and nearest in red.
		if ( m_bDebugging )
		{
			static const float fDrawNearObjectsTime = 0.1f;
			if ( CBotrixServerPlugin::GetTime() > m_fNextDrawNearObjectsTime )
			{
				m_fNextDrawNearObjectsTime = CBotrixServerPlugin::GetTime() + fDrawNearObjectsTime;
				for ( TItemType iType = 0; iType < EItemTypeCollisionTotal; ++iType )
				{
					const good::vector<CItem>& aItems = CItems::GetItems(iType);
					for ( int i = 0; i < m_aNearItems[iType].size(); ++i )  // Draw near items with white color.
					{
						edict_t* pCollide = aItems[m_aNearItems[iType][i]].pEdict;
						CBotrixEngineUtil::DrawBox(
							pCollide->v.origin,
							pCollide->v.mins,
							pCollide->v.maxs,
							fDrawNearObjectsTime,
							0xFF,
							0xFF,
							0xFF
						);
					}

					for ( int i = 0; i < m_aNearestItems[iType].size(); ++i )  // Draw nearest items with red color.
					{
						edict_t* pCollide = aItems[m_aNearestItems[iType][i]].pEdict;
						CBotrixEngineUtil::DrawBox(
							pCollide->v.origin,
							pCollide->v.mins,
							pCollide->v.maxs,
							fDrawNearObjectsTime,
							0xFF,
							0x00,
							0x00
						);
					}
				}
			}
		}
	}

	GoodAssert(
		static_cast<size_t>(gpGlobals->frametime * 1000.0f) <= static_cast<size_t>(std::numeric_limits<byte>::max())
	);

	// Not great that this needs to be inserted wantonly into the code.
	// Really things should be tidied up so that this is done
	// in one canonical place.
	// Taken from Rho-Bot code.
	// Scott: SDK 2.x fix for body direction. From botman's HPB Bot
	if ( m_cCmd.viewangles[YAW] > 180 )
	{
		m_cCmd.viewangles[YAW] -= 360;
	}

	if ( m_cCmd.viewangles[PITCH] > 180 )
	{
		m_cCmd.viewangles[PITCH] -= 360;
	}

	m_cCmd.viewangles[PITCH] = -m_cCmd.viewangles[PITCH];  // invert this dimension for engine
	// End Fix

	g_engfuncs.pfnRunPlayerMove(
		m_pEdict,
		m_cCmd.viewangles,
		m_cCmd.forwardmove,
		m_cCmd.sidemove,
		m_cCmd.upmove,
		static_cast<unsigned short>(m_cCmd.buttons),
		m_cCmd.impulse,
		static_cast<byte>(gpGlobals->frametime * 1000.0f)
	);

	VectorCopy(m_cCmd.viewangles, m_pEdict->v.v_angle);

	// Not great that this needs to be inserted wantonly into the code.
	// Really things should be tidied up so that this is done
	// in one canonical place.
	// Taken from Rho-Bot code.
	m_pEdict->v.angles[PITCH] = m_pEdict->v.v_angle[PITCH] / 3;
	m_pEdict->v.angles[YAW] = m_pEdict->v.v_angle[YAW];
	m_pEdict->v.angles[ROLL] = 0.0;

	m_fPrevThinkTime = CBotrixServerPlugin::GetTime();

	// Bot created for testing and couldn't find path or reached destination and finished using health/armor
	// machine.
	if ( m_bTest &&
		 (m_bMoveFailure || (!m_bNeedMove && !m_bUsingButton && !m_bUsingHealthMachine && !m_bUsingArmorMachine)) )
	{
		if ( m_bMoveFailure )
		{
			BLOG_E("Kicking bot \"%s\" because of move failure", GetPlayerInfo()->GetName());
		}

		CPlayers::KickBot(this);
	}
}

//================================================================================================================
// CBotrixBot virtual protected methods.
//================================================================================================================
void CBotrixBot::CurrentWaypointJustChanged()
{
	if ( m_bNeedMove && m_bUseNavigatorToMove )
	{
		if ( m_cNavigator.SearchEnded() )
		{
			if ( iCurrentWaypoint == iNextWaypoint )  // Bot becomes closer to next waypoint in path.
			{
				if ( CWaypoint::IsValid(m_iAfterNextWaypoint) )
				{
					// Set forward look to next waypoint in path.
					m_vForward = CWaypoints::Get(m_iAfterNextWaypoint).vOrigin;

					// Don't change look while using actions.
					if ( !m_bEnemyAim && !m_bPathAim )
					{
						m_bNeedAim = true;
						m_vLook = m_vForward;
						m_fEndAimTime = CBotrixServerPlugin::GetTime() + GetEndLookTime();
					}
				}
			}
			else if ( CWaypoint::IsValid(iNextWaypoint) )  // Something went wrong (bot falls or pushed?).
			{
				BLOG_W("%s -> Invalid current waypoint %d (should be %d).", GetName(), iCurrentWaypoint, iNextWaypoint);
				m_bMoveFailure = true;
				iCurrentWaypoint = EWaypointIdInvalid;
				m_cNavigator.Stop();
			}
			else
			{
				// Can't touch last waypoint, just stop.
				m_bNeedMove = false;
				m_cNavigator.Stop();
			}
		}
		else
		{
			m_bDestinationChanged =
				true;  // Search not ended, but waypoint changed (?), restart search from current waypoint.
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
bool CBotrixBot::DoWaypointAction()
{
	if ( m_bEnemyAim /*|| !m_bNeedMove || !m_bUseNavigatorToMove - bot test fails */ )
	{
		return false;
	}

	BASSERT(CWaypoint::IsValid(iCurrentWaypoint), return false);
	CWaypoint& w = CWaypoints::Get(iCurrentWaypoint);

	// Check if needs to USE.
	if ( m_bAlreadyUsed )
	{
		// When coming back to current waypoint (if stuck) don't use it again, but make this variable false at
		// next waypoint.
		m_bAlreadyUsed = FLAG_SOME_SET(FWaypointButton | FWaypointHealthMachine | FWaypointArmorMachine, w.iFlags);
	}
	else
	{
		// Check if need health/armor.
		if ( FLAG_SOME_SET(FWaypointHealthMachine, w.iFlags) )
		{
			m_iLastHealthArmor = -1;
			m_bNeedUse = m_PlayerInfo.GetHealth() < m_PlayerInfo.GetMaxHealth();
			m_bUsingHealthMachine = m_bNeedUse;
			m_bUsingArmorMachine = false;
			m_bUsingButton = false;
		}
		else if ( FLAG_SOME_SET(FWaypointArmorMachine, w.iFlags) )
		{
			m_iLastHealthArmor = -1;
			m_bNeedUse = m_PlayerInfo.GetArmorValue() < CBotrixParameterVars::PLAYER_MAX_ARMOUR;
			m_bUsingHealthMachine = false;
			m_bUsingArmorMachine = m_bNeedUse;
			m_bUsingButton = false;
		}
		else if ( FLAG_SOME_SET(FWaypointUse | FWaypointButton, w.iFlags) )
		{
			m_bNeedUse = FLAG_SOME_SET(FWaypointUse, w.iFlags);
			if ( !m_bNeedUse )
			{
				// FWaypointUse not set. FWaypointButton is set.
				CWaypointPath* pCurrentPath = CWaypoints::GetPath(iCurrentWaypoint, iNextWaypoint);
				if ( pCurrentPath )
				{
					TItemIndex iDoor = pCurrentPath->GetDoorNumber();
					m_bNeedUse = pCurrentPath && FLAG_SOME_SET(FPathDoor | FPathElevator, pCurrentPath->iFlags) &&
						iDoor == CWaypoint::GetDoor(w.iArgument);  // Both are for same door or both are not set.
					if ( m_bNeedUse && pCurrentPath->IsDoor() && iDoor != EItemIndexInvalid )
					{
						m_bNeedUse = CItems::IsDoorClosed(iDoor) != 0;  // Only use the button if the door is closed.
					}
				}
			}
			m_bUsingHealthMachine = false;
			m_bUsingArmorMachine = false;
			m_bUsingButton = m_bNeedUse;
		}

		if ( m_bNeedUse )
		{
			Vector angNeeded;
			CWaypoint::GetFirstAngle(angNeeded, w.iArgument);
			CBotrixEngineUtil::NormalizeAngle(angNeeded.x);
			CBotrixEngineUtil::NormalizeAngle(angNeeded.y);

			AngleVectors(angNeeded, m_vForward, nullptr, nullptr);
			m_vForward *= 1000.0f;
			m_vForward += m_vHead;
			m_vLook = m_vForward;

			m_fStartActionTime = m_fEndAimTime = CBotrixServerPlugin::GetTime() + GetEndLookTime();
			m_fEndActionTime = m_fStartActionTime + m_bUsingHealthMachine || m_bUsingArmorMachine
				? m_fTimeIntervalCheckUsingMachines
				: 0.0f;

			m_bLockNavigatorMove = m_bPathAim = m_bNeedAim = true;
		}
	}

	// To stop, bot must not start from current waypoint.
	// if ( CWaypoint::IsValid(iPrevWaypoint) && (iCurrentWaypoint != iPrevWaypoint) &&
	//     FLAG_SOME_SET(FWaypointStop, CWaypoints::Get(iCurrentWaypoint).iFlags) )
	//    m_bNeedStop = true;

	m_bNeedStop = FLAG_SOME_SET(FWaypointStop, w.iFlags);

	return m_bNeedUse;
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::ApplyPathFlags()
{
	BASSERT(m_bNeedMove, return);
	// BotTrace("%s -> Waypoint %d", m_PlayerInfo.GetName(), iCurrentWaypoint);

	// Release buttons and locks.
	m_bNeedUse = m_bAlreadyUsed = false;
	m_bNeedAttack = m_bNeedAttack2 = false;
	m_bNeedJumpDuck = m_bNeedJump = m_bNeedSprint = m_bNeedDuck = m_bNeedWalk = m_bNeedFlashlight = false;
	m_bLockNavigatorMove = m_bPathAim = false;

	m_bPathAim = false;
	if ( CWaypoint::IsValid(iNextWaypoint) )
	{
		CWaypoint& wNext = CWaypoints::Get(iNextWaypoint);
		m_vDestination = wNext.vOrigin;

		if ( iCurrentWaypoint == iNextWaypoint )  // Happens when bot stucks.
		{
			if ( CWaypoint::IsValid(m_iAfterNextWaypoint) )
			{
				m_vForward = CWaypoints::Get(m_iAfterNextWaypoint).vOrigin;
			}
			else
			{
				m_vForward = wNext.vOrigin;
			}
		}
		else
		{
			m_vForward = wNext.vOrigin;

			CWaypointPath* pCurrentPath = CWaypoints::GetPath(iCurrentWaypoint, iNextWaypoint);
			if ( !pCurrentPath )
			{
				return;  // TODO: check why it is happening sometimes?
			}

			m_bLadderMove = FLAG_ALL_SET(FPathLadder, pCurrentPath->iFlags);

			if ( FLAG_ALL_SET(FPathStop, pCurrentPath->iFlags) )
			{
				m_bNeedStop = true;
			}

			// Door in the middle that is not locked, may need to USE to open it.
			if ( pCurrentPath->IsDoor() && !pCurrentPath->HasButtonNumber() )
			{
				TItemIndex iDoor = pCurrentPath->GetDoorNumber();
				m_bNeedUse = iDoor != EItemIndexInvalid && CItems::IsDoorClosed(iDoor);
			}

			if ( FLAG_ALL_SET(FPathSprint, pCurrentPath->iFlags) )
			{
				m_bNeedSprint = true;
			}

			if ( FLAG_ALL_SET(FPathFlashlight, pCurrentPath->iFlags) )
			{
				m_bNeedFlashlight = true;
			}

			if ( FLAG_ALL_SET(FPathCrouch, pCurrentPath->iFlags) && !FLAG_ALL_SET(FPathJump, pCurrentPath->iFlags) )
			{
				m_bNeedDuck = true;  // Crouch only when not jumping.
			}

			// Dont use path aim if aim is locked.
			m_bPathAim = !m_bEnemyAim &&
				FLAG_SOME_SET(
					FPathDoor | FPathJump | FPathBreak | FPathSprint | FPathLadder | FPathStop,
					pCurrentPath->iFlags
				);
		}
	}

	if ( m_bPathAim )
	{
		m_bNeedAim = true;
		m_vLook = m_vForward;  // Always look to next waypoint when aim is locked.
		m_fEndAimTime = CBotrixServerPlugin::GetTime() + GetEndLookTime();
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::DoPathAction()
{
	BASSERT(m_bNeedMove, return);
	BASSERT(CWaypoint::IsValid(iCurrentWaypoint), return);

	if ( CWaypoint::IsValid(iNextWaypoint) && (iCurrentWaypoint != iNextWaypoint) )
	{
		CWaypointPath* pCurrentPath = CWaypoints::GetPath(iCurrentWaypoint, iNextWaypoint);

		NotifyIfFailed(
			pCurrentPath,
			"Expected valid path from waypoint %d to waypoint %d\n",
			iCurrentWaypoint,
			iNextWaypoint
		);

		if ( !pCurrentPath )
		{
			return;
		}

		if ( FLAG_SOME_SET(FPathBreak, pCurrentPath->iFlags) )
		{
			m_bNeedAttack = true;
		}

		if ( FLAG_SOME_SET(FPathJump | FPathTotem, pCurrentPath->iFlags) )
		{
			m_bNeedJump = true;
		}

		if ( FLAG_SOME_SET(FPathCrouch | FPathJump, pCurrentPath->iFlags) ||
			 FLAG_SOME_SET(FPathTotem, pCurrentPath->iFlags) )
		{
			m_bNeedJumpDuck = true;
		}

		if ( m_bNeedAttack || m_bNeedJump || m_bNeedJumpDuck )
		{
			int iStartTime =
				pCurrentPath->ActionTime();  // Action (jump/ jump with duck / attack) start time in deciseconds.
			int iDuration =
				pCurrentPath->ActionDuration();  // Action duration (duck for jump / attack for break) in deciseconds.
			if ( iDuration == 0 )
			{
				iDuration = 10;  // Duration 1 second by default. Good for either duck while jumping or attack (to
								 // break something).
			}

			m_fStartActionTime = CBotrixServerPlugin::GetTime() + (iStartTime / 10.0f);
			m_fEndActionTime = m_fStartActionTime + iDuration / 10.0f;
		}
	}

	m_bRepeatWaypointAction = false;
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::PickItem(const CItem& cItem, TItemType iEntityType, TItemIndex iIndex)
{
	switch ( iEntityType )
	{
		case EItemTypeHealth:
		{
			BotDebug(
				"%s -> Picked %s. Health now %d.",
				GetName(),
				cItem.pItemClass->sClassName.c_str(),
				m_PlayerInfo.GetHealth()
			);
			break;
		}

		case EItemTypeArmor:
		{
			BotDebug(
				"%s -> Picked %s. Armor now %d.",
				GetName(),
				cItem.pItemClass->sClassName.c_str(),
				m_PlayerInfo.GetArmorValue()
			);
			break;
		}

		case EItemTypeWeapon:
		{
			if ( m_bFeatureWeaponCheck )
			{
				TWeaponId iWeapon = CWeapons::AddWeapon(cItem.pItemClass, m_aWeapons);
				if ( CWeapons::IsValid(iWeapon) )
				{
					CWeaponWithAmmo& cWeapon = m_aWeapons[iWeapon];
					BotDebug(
						"%s -> Picked weapon %s (%d/%d, %d/%d).",
						GetName(),
						cItem.pItemClass->sClassName.c_str(),
						cWeapon.Bullets(CWeapon::PRIMARY),
						cWeapon.ExtraBullets(CWeapon::PRIMARY),
						cWeapon.Bullets(CWeapon::SECONDARY),
						cWeapon.ExtraBullets(CWeapon::SECONDARY)
					);
					WeaponChoose();
				}
				else if ( CBotrixMod::aClassNames.size() )
				{
					BLOG_W(
						"%s -> Picked weapon %s, but there is no such weapon for class %s.",
						GetName(),
						cItem.pItemClass->sClassName.c_str(),
						CTypeToString::ClassToString(m_iClass).c_str()
					);
				}
				else
				{
					BLOG_W(
						"%s -> Picked weapon %s, but there is no such weapon.",
						GetName(),
						cItem.pItemClass->sClassName.c_str()
					);
				}
			}
			break;
		}

		case EItemTypeAmmo:
		{
			if ( m_bFeatureWeaponCheck )
			{
				if ( CWeapons::AddAmmo(cItem.pItemClass, m_aWeapons) )
				{
					BotDebug("%s -> Picked ammo %s.", GetName(), cItem.pItemClass->sClassName.c_str());
					WeaponChoose();
				}
				else
				{
					BLOG_W(
						"%s -> Picked ammo %s, but bot doesn't have that weapon.",
						GetName(),
						cItem.pItemClass->sClassName.c_str()
					);
				}
			}
			break;
		}

		case EItemTypeObject:
		{
			BotDebug("%s -> Breaked object %s", GetName(), cItem.pItemClass->sClassName.c_str());
			break;
		}

		default:
		{
			BASSERT(false);
		}
	}

	if ( iEntityType != EItemTypeObject )
	{
		CPickedItem cPickedItem(iEntityType, iIndex, CBotrixServerPlugin::GetTime());

		// If item is not respawnable (or just bad configuration), force to not to search for it again right away,
		// but in 1 minute at least.
		// TODO: check if item is respawnable.
		cPickedItem.fRemoveTime +=
			/*FLAG_ALL_SET_OR_0(FEntityRespawnable, cItem.iFlags) ? cItem.pItemClass->GetArgument() : */ 60.0f;
		m_aPickedItems.push_back(cPickedItem);
	}
}

//================================================================================================================
// CBotrixBot protected methods.
//================================================================================================================
#ifdef BOTRIX_SEND_BOT_CHAT
void CBotrixBot::Speak(bool bTeamSay)
{
	TPlayerIndex iPlayerIndex = m_cChat.iDirectedTo;
	if ( iPlayerIndex == EPlayerIndexInvalid )
		iPlayerIndex = m_iPrevChatMate;
	if ( iPlayerIndex != EPlayerIndexInvalid )
		m_cChat.cMap.push_back(CChatVarValue(CChat::iPlayerVar, 0, iPlayerIndex));
	const good::string& sText = CChat::ChatToText(m_cChat);
	BLOG_D("%s: %s %s", GetName(), bTeamSay ? "say_team" : "say", sText.c_str());
	Say(bTeamSay, sText.c_str());
}
#endif  // BOTRIX_SEND_BOT_CHAT

//----------------------------------------------------------------------------------------------------------------
bool CBotrixBot::IsVisible(CPlayer* pPlayer, bool bViewCone) const
{
	// Check PVS first. Get visible clusters from player's position.
	Vector vAim(pPlayer->GetHead());

	// First check if other player is in bot's view cone.
	static const float fFovHorizontal = 70.0f;  // Giving 140 degree of view horizontally.
	static const float fFovVertical = fFovHorizontal * 3 / 4;  // Normal monitor has 4:3 aspect ratio.

	if ( bViewCone )  // Check view cone.
	{
		vAim -= m_vHead;  // vAim contains vector to enemy relative from bot.

		Vector angPlayer;
		VectorAngles(vAim, angPlayer);

		CBotrixEngineUtil::GetAngleDifference(angPlayer, m_cCmd.viewangles, angPlayer);
		bViewCone =
			((-fFovHorizontal <= angPlayer.x) && (angPlayer.x <= fFovHorizontal) && (-fFovVertical <= angPlayer.y) &&
			 (angPlayer.y <= fFovVertical));
	}
	else
	{
		bViewCone = true;  // Assume enemy is in view cone.
	}

	if ( !bViewCone )
	{
		return false;
	}

	return CBotrixEngineUtil::IsVisible(m_vHead, pPlayer->GetEdict());
}

//----------------------------------------------------------------------------------------------------------------
float CBotrixBot::GetEndLookTime()
{
	// angle 180 degrees = aAimSpeed time   ->   Y time = angle X * aAimSpeed / 180 degrees
	// angle X           = Y time
	// static const float aAimSpeed[EBotIntelligenceTotal] = { 1.20f, 0.85f, 0.67f, 0.57f, 1.42f };
	static const int iAngDiff = 40;  // 30 degrees to make time difference in bot's aim.
	static const int iEndAimSize = 180 / iAngDiff + 1;
	static const float aEndAimTime[EBotIntelligenceTotal][iEndAimSize] = {
		//  40    80     120    160    180
		{0.40f, 0.55f, 0.70f, 0.85f, 1.00f},
		{0.30f, 0.42f, 0.54f, 0.66f, 0.80f},
		{0.20f, 0.30f, 0.40f, 0.50f, 0.60f},
		{0.15f, 0.20f, 0.25f, 0.30f, 0.35f},
		{0.10f, 0.10f, 0.15f, 0.20f, 0.25f},
	};

	Vector vDestinationAim(m_vLook);
	vDestinationAim -= m_vHead;  // Get abs vector.

	Vector angNeeded;
	VectorAngles(vDestinationAim, angNeeded);

	CBotrixEngineUtil::GetAngleDifference(m_cCmd.viewangles, angNeeded, angNeeded);

	int iPitch = abs((int)angNeeded.x);
	int iYaw = abs((int)angNeeded.y);

	iPitch /= iAngDiff;
	iYaw /= iAngDiff;

	BASSERT((iPitch <= iEndAimSize) && (iYaw <= iEndAimSize), return 0.0f);

	if ( iPitch < iYaw )
	{
		iPitch = iYaw;  // iPitch = MAX2( iPitch, iYaw );
	}

	float fAimTime = aEndAimTime[m_iIntelligence][iPitch];
	BASSERT(fAimTime < 2.0f, return 2.0f);

	return fAimTime;
	// return CBotrixServerPlugin::GetTime() + iPitch * aAimSpeed[m_iIntelligence] / 180.0f;
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::WeaponCheckCurrent(bool bAddToBotWeapons)
{
	// Check weapon bot has in hands.
	const char* szCurrentWeapon = m_PlayerInfo.GetWeaponName();
	if ( !szCurrentWeapon )
	{
		return;
	}

	GoodAssert(WeaponSearch(szCurrentWeapon) == EWeaponIdInvalid);

	good::string sCurrentWeapon(szCurrentWeapon);
	TWeaponId iId = CWeapons::GetIdFromWeaponName(sCurrentWeapon);
	if ( iId == EWeaponIdInvalid )
	{
		// Add weapon class first.
		BLOG_W("%s -> Adding new weapon class %s.", GetName(), szCurrentWeapon);
		CItemClass cWeaponClass;
		cWeaponClass.fPickupDistanceSqr = static_cast<float>(CBotrixMod::iPlayerRadius) * 4.0f;  // 4*player's radius.
		cWeaponClass.fPickupDistanceSqr *= cWeaponClass.fPickupDistanceSqr;
		// Don't set engine name so mod will think that there is no such weapon in this map.
		// cWeaponClass.szEngineName = szCurrentWeapon;
		cWeaponClass.sClassName = szCurrentWeapon;
		const CItemClass* pClass = CItems::AddItemClassFor(EItemTypeWeapon, cWeaponClass);

		// Add new weapon to default weapons.
		BLOG_W(
			"%s -> Adding new weapon %s, %s.",
			GetName(),
			szCurrentWeapon,
			bAssumeUnknownWeaponManual ? "melee" : "ranged"
		);
		CWeapon* pNewWeapon = new CWeapon();
		pNewWeapon->pWeaponClass = pClass;

		// Make it usable by all classes and teams.
		pNewWeapon->iClass = -1;
		pNewWeapon->iTeam = -1;
		pNewWeapon->fDamage[0] = 0.01f;  // DamagePerSecond() will be low to not select unknown weapons by default.

		// Make it has infinite ammo.
		pNewWeapon->iAttackBullets[CWeapon::PRIMARY] = 0;

		if ( bAssumeUnknownWeaponManual )
		{
			pNewWeapon->iType = EWeaponMelee;
			pNewWeapon->fShotTime[0] = 0.2f;  // Manual weapon: 5 times in a second.
		}
		else
		{
			pNewWeapon->iType = EWeaponRifle;
			pNewWeapon->iClipSize[CWeapon::PRIMARY] = 1;
			pNewWeapon->iDefaultAmmo[CWeapon::PRIMARY] = 1;
			pNewWeapon->fShotTime[0] = 0.01f;  // This will make bot always press attack button.
		}
		CWeaponWithAmmo cNewWeapon(pNewWeapon);
		iId = CWeapons::Add(cNewWeapon);
	}

	if ( bAddToBotWeapons )
	{
		const CWeaponWithAmmo& cNewWeapon = *CWeapons::Get(iId);
		m_aWeapons.push_back(cNewWeapon);
		iId = m_aWeapons.size() - 1;
		m_aWeapons[iId].AddWeapon();

		if ( !CWeapons::IsValid(m_iMeleeWeapon) && cNewWeapon.IsMelee() )
		{
			m_iMeleeWeapon = iId;
		}
		else if ( !CWeapons::IsValid(m_iPhyscannon) && cNewWeapon.IsPhysics() )
		{
			m_iPhyscannon = iId;
		}
		else if ( !CWeapons::IsValid(m_iBestWeapon) && cNewWeapon.IsRanged() )
		{
			m_iBestWeapon = iId;
		}

		ChangeWeapon(iId);  // As if just switching to this weapon.
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::WeaponsScan()
{
	GoodAssert(m_bFeatureWeaponCheck);
	BotDebug("%s -> Scan weapons.", GetName());

	// Check if bot has physcannon / melee weapon.
	m_iPhyscannon = m_iMeleeWeapon = -1;
	for ( int i = 0; i < m_aWeapons.size(); ++i )
	{
		bool bPresent = WeaponSet(m_aWeapons[i].GetName());
		if ( bPresent )
		{
			m_aWeapons[i].AddWeapon();
			if ( m_aWeapons[i].IsPhysics() )
			{
				m_iPhyscannon = i;
			}
			else if ( m_aWeapons[i].IsMelee() )
			{
				m_iMeleeWeapon = i;
			}
		}
	}

	// Set bot's best weapon.
	m_iWeapon = CWeapons::GetBestWeapon(m_aWeapons);
	if ( !CWeapons::IsValid(m_iWeapon) )
	{
		m_iWeapon = m_iMeleeWeapon;  // Get melee weapon.
	}

	m_iBestWeapon = m_iWeapon;
	if ( CWeapons::IsValid(m_iWeapon) )
	{
		ChangeWeapon(m_iWeapon);
	}
	else
	{
		BLOG_W("%s -> No weapons available.", GetName());
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::UpdateWeapon()
{
	if ( !m_bFeatureWeaponCheck )
	{
		return;
	}

	const char* szCurrentWeapon = m_PlayerInfo.GetWeaponName();
	if ( !szCurrentWeapon )
	{
		m_iWeapon = EWeaponIdInvalid;
		return;
	}

	if ( !CWeapons::IsValid(m_iWeapon) || (m_aWeapons[m_iWeapon].GetName() != szCurrentWeapon) )
	{
		// Happens when out of bullets automatically.
		if ( CWeapons::IsValid(m_iWeapon) )
		{
			BLOG_W(
				"%s -> Current weapon is %s, should be %s.",
				GetName(),
				szCurrentWeapon,
				m_aWeapons[m_iWeapon].GetName().c_str()
			);
		}
		else
		{
			BLOG_W("%s -> Current weapon is %s.", GetName(), szCurrentWeapon);
		}

		TWeaponId iCurrentWeapon = WeaponSearch(szCurrentWeapon);

		if ( iCurrentWeapon == EWeaponIdInvalid )
		{
			WeaponCheckCurrent(true);
		}
		else
		{
			if ( CWeapons::IsValid(m_iWeapon) )
			{
				if ( m_aWeapons[m_iWeapon].IsMelee() || m_aWeapons[m_iWeapon].IsPhysics() )
				{
					m_aWeapons[m_iWeapon].SetPresent(false);
				}
				else
				{
					m_aWeapons[m_iWeapon].SetEmpty();
				}
			}

			ChangeWeapon(iCurrentWeapon);  // As if just switching to this weapon.
		}
	}
	else
	{
		CWeaponWithAmmo& cWeapon = m_aWeapons[m_iWeapon];
		cWeapon.GameFrame(m_cCmd.buttons);

		if ( m_bDontAttack || !m_bCommandAttack )
		{
			m_cCmd.buttons = 0;
		}

		if ( cWeapon.CanUse() && (cWeapon.GetBaseWeapon()->bForbidden || cWeapon.Empty()) )
		{
			WeaponChoose();  // Select other weapon.
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::UpdateWorld()
{
	// Update picked items.
	if ( m_aPickedItems.size() )
	{
		if ( CBotrixServerPlugin::GetTime() >= m_aPickedItems[m_iCurrentPickedItem].fRemoveTime )
		{
			m_aPickedItems.erase(m_aPickedItems.begin() + m_iCurrentPickedItem);
		}
		else
		{
			++m_iCurrentPickedItem;
		}

		if ( m_iCurrentPickedItem >= m_aPickedItems.size() )
		{
			m_iCurrentPickedItem = 0;
		}
	}

	// Update bot's weapon.
	UpdateWeapon();

	// Get near items.
	Vector vFoot = m_pEdict->v.origin;

	for ( TItemType iType = 0; iType < EItemTypeCollisionTotal; ++iType )
	{
		const good::vector<CItem>& aItems = CItems::GetItems(iType);
		if ( aItems.size() == 0 )
		{
			continue;
		}

		good::vector<TItemIndex>& aNearest = m_aNearestItems[iType];
		int iNearestSize = aNearest.size();

		good::vector<TItemIndex>& aNear = m_aNearItems[iType];
		int iNearSize = aNear.size();

		// Update nearest items.
		for ( int i = 0; i < iNearestSize; )
		{
			int index = aNearest[i];
			const CItem* cItem = index < aItems.size() ? &aItems[index] : NULL;

			if ( cItem == NULL || cItem->IsFree() )  // Remove object if it is removed from game.
			{
				aNearest.erase(aNearest.begin() + i);
				--iNearestSize;
			}
			else if ( !cItem->IsOnMap() )  // Was on map before, but disappeared, bot could grab it or break it.
			{
				PickItem(*cItem, iType, aNearest[i]);
				aNearest.erase(aNearest.begin() + i);
				--iNearestSize;
			}
			else if ( (cItem->CurrentPosition() - vFoot).LengthSquared() > cItem->fPickupDistanceSqr )  // Item becomes
																										// far.
			{
				aNear.push_back(aNearest[i]);
				aNearest.erase(aNearest.begin() + i);
				--iNearestSize;
			}
			else
			{
				++i;
			}
		}

		// Check if bot becomes too close to near items, to pass it to nearest items (and viceversa).
		for ( int i = 0; i < iNearSize; )
		{
			int index = aNear[i];
			const CItem* cItem = index < aItems.size() ? &aItems[index] : NULL;

			if ( cItem == NULL || cItem->IsFree() || !cItem->IsOnMap() )
			{
				aNear.erase(aNear.begin() + i);
				--iNearSize;
			}
			else
			{
				float fDistSqr = (cItem->CurrentPosition() - vFoot).LengthSquared();

				if ( CBotrixMod::iNearItemMaxDistanceSqr < fDistSqr )  // Item becomes far.
				{
					aNear.erase(aNear.begin() + i);
					--iNearSize;
				}
				else
				{
					if ( fDistSqr <= cItem->fPickupDistanceSqr )  // Can pick up.
					{
						aNearest.push_back(aNear[i]);
						aNear.erase(aNear.begin() + i);
						--iNearSize;
					}
					else
					{
						++i;
					}
				}
			}
		}

		// Check if bot becomes close to items to consider them near (m_iCheckEntitiesPerFrame items max).
		int iCheckTo = m_iNextNearItem[iType] + m_iCheckEntitiesPerFrame;
		if ( iCheckTo > aItems.size() )
		{
			iCheckTo = aItems.size();
		}

		for ( int i = m_iNextNearItem[iType]; i < iCheckTo; ++i )
		{
			const CItem* pItem = &aItems[i];

			if ( pItem->IsFree() || !pItem->IsOnMap() ||  // Item is picked or broken.
				 (find(aNear.begin(), aNear.end(), i) !=
				  aNear.end()) ||  // Item is near, all checks were already made before for all near items.
				 (find(aNearest.begin(), aNearest.end(), i) !=
				  aNearest.end()) )  // Item is nearest, all checks were already made before for all nearest items.
			{
				continue;
			}

			float fDistSqr = (pItem->CurrentPosition() - vFoot).LengthSquared();

			if ( fDistSqr <= pItem->fPickupDistanceSqr )
			{
				aNearest.push_back(i);
			}
			else if ( fDistSqr <= CBotrixMod::iNearItemMaxDistanceSqr )  // Item is close.
			{
				aNear.push_back(i);
			}
		}

		m_iNextNearItem[iType] = (iCheckTo == aItems.size()) ? 0 : iCheckTo;
	}

	// Check only 1 player per frame.
	if ( m_bEnemyOffSight && (CBotrixServerPlugin::GetTime() >= m_fTimeToEraseEnemy) )
	{
		CurrentEnemyNotVisible();
	}

	CPlayer* pCheckPlayer = NULL;

	while ( m_iNextCheckPlayer < CPlayers::Size() )
	{
		CPlayer* pPlayer = CPlayers::Get(m_iNextCheckPlayer);
		if ( pPlayer && (this != pPlayer) )
		{
			pCheckPlayer = pPlayer;
			break;
		}

		m_iNextCheckPlayer++;
	}

	if ( pCheckPlayer )
	{
		if ( pCheckPlayer->IsAlive() )
		{
			float fDistSqr = (pCheckPlayer->GetHead() - m_vHead).LengthSquared();

			if ( fDistSqr <= CBotrixMod::iNearItemMaxDistanceSqr )
			{
				m_aNearPlayers.set(m_iNextCheckPlayer);

				// Check if players are not stuck with each other.
				if ( m_bStuck && !m_bStuckTryingSide && (fDistSqr <= (SQR(CBotrixMod::iPlayerRadius) << 2)) )
				{
					Vector vNeeded(m_vDestination);
					Vector vOther(pCheckPlayer->GetHead());
					vNeeded -= m_vHead;
					vOther -= m_vHead;

					Vector angNeeded;
					Vector angOther;
					VectorAngles(vNeeded, angNeeded);
					VectorAngles(vOther, angOther);

					float fYaw = angNeeded.y - angOther.y;
					CBotrixEngineUtil::DeNormalizeAngle(fYaw);

					if ( (-45.0f < fYaw) && (fYaw < 45.0f) )
					{
						// Player stuck with this one (move right for 1/4 seconds).
						m_bResolvingStuck = true;
						m_bStuckTryingSide = true;
						m_bStuckTryGoLeft = false;
						m_fEndActionTime = CBotrixServerPlugin::GetTime() + 0.25f;
					}
				}
			}
			else
			{
				m_aNearPlayers.reset(m_iNextCheckPlayer);
			}
		}
		else
		{
			m_aNearPlayers.reset(m_iNextCheckPlayer);

			if ( m_pCurrentEnemy == pCheckPlayer )
			{
				EraseCurrentEnemy();
			}
		}

		// Check if this enemy can be seen / should be attacked.
		if ( !m_bTest && !m_bDontAttack && IsEnemy(pCheckPlayer) )
		{
			CheckEnemy(m_iNextCheckPlayer, pCheckPlayer, true);
		}
	}

	m_iNextCheckPlayer++;

	if ( m_iNextCheckPlayer >= CPlayers::Size() )
	{
		m_iNextCheckPlayer = 0;
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::CheckEnemy(int iPlayerIndex, CPlayer* pPlayer, bool bCheckVisibility)
{
	BASSERT(!m_bDontAttack && (m_pCurrentEnemy != this), return);

	// Assume that current enemy is in view cone.
	bool bIsDifferentEnemy = (m_pCurrentEnemy != pPlayer);
	bool bEnemyChanged = false;

	if ( pPlayer->IsAlive() && (!bCheckVisibility || IsVisible(pPlayer, bIsDifferentEnemy)) )
	{
		// Currently seeing this player.
		m_aSeenEnemies.set(iPlayerIndex);

		float fDistanceSqr = (pPlayer->GetHead() - m_vHead).LengthSquared();

		// Add 128 units to not to change enemy too often. TODO: strategy parameter.
		bEnemyChanged =
			(m_pCurrentEnemy == NULL) || (bIsDifferentEnemy && (fDistanceSqr < m_fDistanceSqrToEnemy + 16384));
		if ( bEnemyChanged || (m_pCurrentEnemy == pPlayer) )  // Update distance.
		{
			m_bEnemyOffSight = false;
			m_pCurrentEnemy = pPlayer;
			m_fDistanceSqrToEnemy = fDistanceSqr;
		}
	}
	else  // Can't see this player anymore or player is dead.
	{
		m_aSeenEnemies.reset(iPlayerIndex);
		if ( m_pCurrentEnemy == pPlayer )
		{
			if ( pPlayer->IsAlive() )
			{
				if ( !m_bEnemyOffSight )
				{
					m_bEnemyOffSight = true;
					m_fTimeToEraseEnemy =
						CBotrixServerPlugin::GetTime() + 1.5f;  // TODO: CONCOMMAND. Give some time to erase enemy.
					m_bAttackDuck = false;
				}
			}
			else
			{
				EraseCurrentEnemy();
			}
		}
	}

	if ( bEnemyChanged )
	{
		m_bEnemyAimed = m_bEnemyOffSight = false;  // Still need to aim at enemy before shooting.
		if ( !m_bDontAttack )
		{
			EnemyAim();
		}
	}

	if ( m_bFeatureAttackDuckEnabled )
	{
		CheckAttackDuck(pPlayer);
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::CheckAttackDuck(CPlayer* pPlayer)
{
	GoodAssert(m_bFeatureAttackDuckEnabled && !m_bDontAttack);

	// Don't duck if using melee weapon or too far away.
	if ( !CWeapons::IsValid(m_iWeapon) || m_aWeapons[m_iWeapon].IsMelee() ||
		 !m_aWeapons[m_iWeapon].CanBeUsed(m_fDistanceSqrToEnemy) )
	{
		m_bAttackDuck = false;
		return;
	}

	if ( m_pCurrentEnemy == pPlayer )  // Check if need to duck to attack.
	{
		bool bInRangeDuck = (m_cAttackDuckRangeSqr.first <= m_fDistanceSqrToEnemy) &&
			(m_fDistanceSqrToEnemy <= m_cAttackDuckRangeSqr.second);

		if ( !m_bNeedDuck && !m_bAttackDuck & bInRangeDuck )  // Duck only if not ducking already.
		{
			Vector vSrc(m_vHead);
			vSrc.z -= CBotrixParameterVars::PLAYER_EYE;
			vSrc.z += CBotrixParameterVars::PLAYER_EYE_CROUCHED;

			// Duck, if enemy is visible while ducking.
			m_bAttackDuck = CBotrixEngineUtil::IsVisible(vSrc, m_pCurrentEnemy->GetHead(), EVisibilityBots);
		}
		else
		{
			m_bAttackDuck &= bInRangeDuck;  // Stop ducking if enemy is far.
		}
	}

	m_bAttackDuck = m_bAttackDuck && !m_bFlee;  // Don't duck if fleeing.
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::EnemyAim()
{
	GoodAssert(m_pCurrentEnemy && !m_bDontAttack);

	if ( m_bStuckUsePhyscannon || m_bStuckBreakObject )
	{
		return;
	}

	m_vLook = m_pCurrentEnemy->GetHead();
	if ( CWeapons::IsValid(m_iWeapon) )
	{
		CWeaponWithAmmo& cWeapon = m_aWeapons[m_iWeapon];
		for ( int i = CWeapon::SECONDARY; i >= CWeapon::PRIMARY; --i )  // Prefer secondary.
		{
			if ( cWeapon.HasAmmoInClip(i) && cWeapon.IsDistanceSafe(m_fDistanceSqrToEnemy, i) )
			{
				m_aWeapons[m_iWeapon]
					.GetLook(GetHead(), m_pCurrentEnemy, m_fDistanceSqrToEnemy, m_iIntelligence, i, m_vLook);
				break;
			}
		}
	}
	else
	{
		m_pCurrentEnemy->GetCenter(m_vLook);
	}

	if ( m_iIntelligence < EBotPro )
	{
		// Smart: -4..+4, normal -8..+8, stupid -12..+12, fool -16..+16.
		int iError = (EBotPro - m_iIntelligence) * 4;
		m_vLook.x += iError - (rand() % (iError << 1));
		m_vLook.y += iError - (rand() % (iError << 1));
		m_vLook.z += iError - (rand() % (iError << 1));
	}

	m_fEndAimTime = GetEndLookTime();

	// Make sure to look far away.
	// m_vLook -= m_vHead;
	// m_vLook *= 1000.0f;
	// m_vLook += m_vHead;

	// Take player's speed into account.
	if ( m_iIntelligence >= EBotNormal )
	{
		Vector vInc(m_pCurrentEnemy->GetHead());
		vInc -= m_pCurrentEnemy->GetPreviousHead();

		float fFramesLeft =
			m_fEndAimTime / std::max<float>(gpGlobals->frametime, 0.01f);  // How many frames left to aim?
		vInc *= fFramesLeft;

		m_vLook += vInc;
	}

	m_fEndAimTime += CBotrixServerPlugin::GetTime();
	m_bNeedAim = m_bEnemyAim = true;
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::WeaponChoose()
{
	if ( m_bStuckBreakObject || m_bStuckUsePhyscannon )
	{
		return;
	}

	GoodAssert(CWeapons::IsValid(m_iWeapon));
	CWeaponWithAmmo& cWeapon = m_aWeapons[m_iWeapon];

	if ( !cWeapon.CanUse() ||
		 (m_pCurrentEnemy &&
		  (cWeapon.CanShoot(CWeapon::PRIMARY, m_fDistanceSqrToEnemy) ||
		   cWeapon.CanShoot(CWeapon::SECONDARY, m_fDistanceSqrToEnemy))) )
	{
		return;  // Don't change weapon if enemy is close... TODO: sometimes using melee and not changing.
	}

	// If not engaging enemy, reload some weapons. Here cWeapon.CanUse() is true.
	if ( (m_pCurrentEnemy == NULL) && m_bNeedReload )
	{
		if ( cWeapon.IsReloading() )
		{
			return;
		}

		if ( cWeapon.NeedReload(0) )
		{
			WeaponReload();
			return;
		}

		TWeaponId iIdx = EWeaponIdInvalid;
		for ( int i = 0; i < m_aWeapons.size(); ++i )
		{
			if ( m_aWeapons[i].IsPresent() && m_aWeapons[i].NeedReload(CWeapon::PRIMARY) )
			{
				iIdx = i;
				break;
			}
		}

		if ( iIdx == EWeaponIdInvalid )
		{
			m_bNeedReload = false;  // Continue to select best weapon.
		}
		else
		{
			GoodAssert(iIdx != m_iWeapon);
			BotDebug("%s -> Change weapon to reload %s.", GetName(), m_aWeapons[iIdx].GetName().c_str());
			ChangeWeapon(iIdx);
			return;
		}
	}

	// Choose best ranged weapon.
	m_iBestWeapon = CWeapons::GetBestWeapon(m_aWeapons);

	// Choose melee weapon.
	if ( !CWeapons::IsValid(m_iBestWeapon) && CWeapons::IsValid(m_iMeleeWeapon) )
	{
		if ( m_aWeapons[m_iMeleeWeapon].IsPresent() )
		{
			m_iBestWeapon = m_iMeleeWeapon;
		}
		else
		{
			m_iMeleeWeapon = EWeaponIdInvalid;
		}
	}

	if ( m_iBestWeapon == m_iWeapon )
	{
		if ( !m_aWeapons[m_iWeapon].HasAmmoInClip(0) )
		{
			m_bStayReloading = true;
		}
	}
	else if ( CWeapons::IsValid(m_iBestWeapon) )
	{
		GoodAssert(m_aWeapons[m_iBestWeapon].IsPresent());
		BotDebug("%s -> Set best weapon %s.", GetName(), m_aWeapons[m_iBestWeapon].GetName().c_str());
		ChangeWeapon(m_iBestWeapon);
	}
	else
	{
		m_iBestWeapon = m_iWeapon;
	}

	m_bNeedSetWeapon = false;
}

//----------------------------------------------------------------------------------------------------------------
bool CBotrixBot::WeaponSet(const good::string& sWeapon)
{
	GoodAssert(m_bFeatureWeaponCheck);

	// This will create new weapon to the bot. It is not what we need.
	// m_pController->WeaponSet(sWeapon.c_str());

	g_engfuncs.pfnClientCommand(m_pEdict, "use %s", sWeapon.c_str());

	const char* szWeapon = m_PlayerInfo.GetWeaponName();
	bool bOk = (szWeapon && (sWeapon == szWeapon));
	BotDebug("%s -> Set active weapon %s: %s.", GetName(), sWeapon.c_str(), bOk ? "ok" : "error");
	return bOk;
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::ChangeWeapon(TWeaponId iIndex)
{
	// GoodAssert( iIndex != m_iWeapon ); // This can happen when out of bullets, and autoswitch is made to other
	// weapon.
	GoodAssert(m_bFeatureWeaponCheck && CWeapons::IsValid(iIndex));

	CWeaponWithAmmo* pOld = NULL;
	if ( CWeapons::IsValid(m_iWeapon) )
	{
		pOld = &m_aWeapons[m_iWeapon];
	}

	CWeaponWithAmmo& cNew = m_aWeapons[iIndex];
	// GoodAssert( cNew.IsPresent() ); // Happens when out of bullets & forbidden.

	const good::string& sWeaponName = cNew.GetName();
	if ( WeaponSet(sWeaponName) )
	{
		CWeaponWithAmmo::Holster(pOld, cNew);
		m_iWeapon = iIndex;
	}
	else if ( m_PlayerInfo.GetWeaponName() )
	{
		if ( cNew.IsMelee() )
		{
			BLOG_W("%s -> Could not set weapon %s, not present?", GetName(), sWeaponName.c_str());
			cNew.SetPresent(false);
			if ( m_iMeleeWeapon == iIndex )
			{
				m_iMeleeWeapon = EWeaponIdInvalid;
			}

			if ( m_iPhyscannon == iIndex )
			{
				m_iPhyscannon = EWeaponIdInvalid;
			}

			if ( m_iBestWeapon == iIndex )
			{
				m_iBestWeapon = EWeaponIdInvalid;
			}
		}
		else
		{
			BLOG_W("%s -> Could not set weapon %s, no bullets?", GetName(), sWeaponName.c_str());
			cNew.SetEmpty();
		}
	}
	else
	{
		BLOG_W("%s -> current weapon is null.", GetName());
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::WeaponShoot(int iSecondary)
{
	if ( !m_bStuckBreakObject && !m_bStuckUsePhyscannon && !m_bNeedAttack && (m_bDontAttack || !m_bCommandAttack) )
	{
		return;
	}

	if ( m_bFeatureWeaponCheck )
	{
		GoodAssert(CWeapons::IsValid(m_iWeapon));
		CWeaponWithAmmo& cWeapon = m_aWeapons[m_iWeapon];
		GoodAssert(cWeapon.CanUse());

		BotDebug(
			"%s -> Shoot %s %s, ammo %d with %d extra.",
			GetName(),
			(iSecondary) ? "secondary" : "primary",
			cWeapon.GetName().c_str(),
			cWeapon.Bullets(iSecondary),
			cWeapon.ExtraBullets(iSecondary)
		);

		cWeapon.Shoot(iSecondary);
		m_bNeedReload = cWeapon.IsRanged();
	}

	FLAG_SET(iSecondary ? IN_ATTACK2 : IN_ATTACK, m_cCmd.buttons);
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::CheckSideLook(bool bIsMoving, bool /*bNewDestination*/)
{
	if ( m_pCurrentEnemy || !bIsMoving )
	{
		return;
	}

	/*if ( m_bUseSideLook && !m_bPathAim )
	{
		// Check if need to change look forward/backward/left/right while moving.
		bool bIsTime = CBotrixServerPlugin::GetTime() >= m_fRandomSideLookTime;

		// Bot needs to change look when it is moving and:
		//  - either it is time to change side look.
		//  - either waypoint is reached while looking forward.
		if ( bIsTime || (bNewDestination && (m_iCurrentSideLook == 0)) )
		{
			m_bNeedAim = true;

			Vector v[4]; // Forward, left, right, back.
			v[0] = m_vForward - m_vHead; // Forward.
			VectorVectors(v[0], v[2], v[3]); // Get right side.
			v[1] = -v[2]; // Left = -Right.
			v[3] = -v[0]; // Back = -Forward.

			// Get time to finish changing side look.
			//m_fEndAimTime = CBotrixServerPlugin::GetTime() + aEndAimTime[m_iIntelligence];

			if ( bIsTime )
			{
				static const float aBaseTime[4][EBotIntelligenceTotal] =
				{
					{ 11.0f, 9.0f, 7.0f, 5.0f, 3.0f }, // Base time to stay looking forward.
					{ 2.5f,  2.0f, 1.5f, 1.0f, 0.5f }, // Base time to stay looking left.
					{ 2.5f,  2.0f, 1.5f, 1.0f, 0.5f }, // Base time to stay looking right.
					{ 2.5f,  2.0f, 1.5f, 1.0f, 0.5f }, // Base time to stay looking back.
				};
				static const int aRandTime[4][EBotIntelligenceTotal] =
				{
					{ 7, 6, 5, 4, 3 }, // Random time to stay looking forward.
					{ 4, 4, 3, 3, 2 }, // Random time to stay looking left.
					{ 4, 4, 3, 3, 2 }, // Random time to stay looking right.
					{ 3, 3, 2, 2, 1 }, // Random time to stay looking back.
				};

				// Bot changes side look when time comes or destination is changed and it is looking forward.
				if (m_iCurrentSideLook == 0) // We were looking forward.
				{
					do {
						m_iCurrentSideLook = rand() & 3;
					} while (m_iCurrentSideLook == 0);
				}
				else
					m_iCurrentSideLook = 0; // Always look forward after looking other side.

				// Get time to start changing side look again.
				m_fRandomSideLookTime = aBaseTime[m_iCurrentSideLook][m_iIntelligence]  +  (rand() %
	aRandTime[m_iCurrentSideLook][m_iIntelligence]);

				// TODO: erase me.
				switch (m_iCurrentSideLook)
				{
				case 0: BotDebug("Forward %.1f", m_fRandomSideLookTime); break;
				case 1: BotDebug("Left %.1f", m_fRandomSideLookTime); break;
				case 2: BotDebug("Right %.1f", m_fRandomSideLookTime); break;
				case 3: BotDebug("Back %.1f", m_fRandomSideLookTime); break;
				}
			}

			// Make vector larger, so bot will not change it when pass it.
			v[m_iCurrentSideLook] *= 1000.0f;
			m_vLook = m_vHead;
			m_vLook += v[m_iCurrentSideLook];

			m_fEndAimTime = CBotrixServerPlugin::GetTime() + GetEndLookTime();

			if ( bIsTime )
				m_fRandomSideLookTime += m_fEndAimTime;
		}
	}

	else // Not using side look.*/
	{
		// Bot is moving using navigator, but doesn't need to change look, look at next waypoint.
		if ( m_bUseNavigatorToMove && !m_bNeedAim )
		{
			m_bNeedAim = true;
			m_vLook = m_vForward;
			m_vLook -= m_vHead;
			m_vLook *= 1000.0f;
			m_vLook += m_vHead;  // m_vLook = m_vHead + (m_vForward - m_vHead)*1000.0f
			m_fEndAimTime = CBotrixServerPlugin::GetTime() + GetEndLookTime();
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
bool CBotrixBot::ResolveStuckMove()
{
	TWaypointId iPrevCurrWaypoint = iCurrentWaypoint;

	// Reset waypoint.
	iCurrentWaypoint = CWaypoints::GetNearestWaypoint(m_vHead);
	if ( !CWaypoint::IsValid(iCurrentWaypoint) )
	{
		m_bMoveFailure = true;
		m_cNavigator.Stop();
		BotDebug("%s -> Current waypoint invalid.", GetName());
		return false;
	}

	// Check if bot is touching some door/elevator.
	if ( !m_bTest && m_aNearestItems[EItemTypeDoor].size() )
	{
		m_bMoveFailure = true;  // Let mod decide what to do.
		return false;
	}

	pStuckObject = NULL;
	CWaypoint& wCurr = CWaypoints::Get(iCurrentWaypoint);

	// Check if bot is stuck with some object.
	for ( int iIndexStuckObject = 0; iIndexStuckObject < m_aNearestItems[EItemTypeObject].size(); ++iIndexStuckObject )
	{
		pStuckObject = &CItems::GetItems(EItemTypeObject)[m_aNearestItems[EItemTypeObject][iIndexStuckObject]];
		// If object is behind, it doesn't disturb.
		Vector vObject = pStuckObject->CurrentPosition();
		vObject -= m_vHead;

		Vector vGoing(m_vDestination);
		vGoing -= m_vHead;

		Vector angGoing;
		Vector angObject;
		VectorAngles(vObject, angObject);
		VectorAngles(vGoing, angGoing);

		CBotrixEngineUtil::GetAngleDifference(angGoing, angObject, angGoing);

		if ( angGoing.y <= -90.0f || angGoing.y >= 90.0f )
		{
			pStuckObject = NULL;
		}
		else
		{
			break;
		}
	}

	if ( pStuckObject )
	{
		bool bCanThrow = !m_bDontThrowObjects && CWeapons::IsValid(m_iPhyscannon) &&
			m_aWeapons[m_iPhyscannon].IsPresent() && !pStuckObject->IsHeavy();
		bool bThrowAtEnemy = bCanThrow && m_pCurrentEnemy && !m_bTest && !m_bDontAttack &&
			!m_aWeapons[m_iPhyscannon].GetBaseWeapon()->bForbidden;

		if ( !m_bDontBreakObjects && pStuckObject->IsBreakable() && !pStuckObject->IsExplosive() &&
			 !bThrowAtEnemy )  // Prefer to throw at enemy.
		{
			BotDebug("%s -> Stuck, will break object %s.", GetName(), pStuckObject->pItemClass->sClassName.c_str());

			// Look at origin of the object.
			m_vLook = pStuckObject->CurrentPosition();
			m_fStartActionTime = m_fEndAimTime =
				CBotrixServerPlugin::GetTime() + GetEndLookTime();  // When start to break object.
			m_bResolvingStuck = m_bStuckBreakObject = m_bPathAim = true;
		}
		else if ( bCanThrow )
		{
			BotDebug("%s -> Stuck, will throw object %s.", GetName(), pStuckObject->pItemClass->sClassName.c_str());

			// Look at origin of the object.
			m_vLook = m_vDisturbingObjectPosition = pStuckObject->CurrentPosition();
			m_fStartActionTime = m_fEndAimTime = CBotrixServerPlugin::GetTime() + GetEndLookTime();
			m_fEndActionTime = m_fStartActionTime + 10.0f;  // Give enough time to switch weapon/aim/look back/throw.

			m_bResolvingStuck = m_bStuckUsePhyscannon = m_bPathAim = true;
			m_bStuckPhyscannonEnd = false;
		}
		else
		{
			float zDistance = pStuckObject->pEdict->v.maxs[VEC3_Z] - pStuckObject->pEdict->v.mins[VEC3_Z];
			if ( zDistance <= CBotrixParameterVars::CalcMaxHeightOfCrouchJump() )  // Can jump on it.
			{
				BotDebug(
					"%s -> Stuck, will jump on object %s.",
					GetName(),
					pStuckObject->pItemClass->sClassName.c_str()
				);

				m_bNeedJump = m_bNeedJumpDuck = true;
				m_fStartActionTime = CBotrixServerPlugin::GetTime();
				m_fEndActionTime = CBotrixServerPlugin::GetTime() + 0.25f;
			}
			else  // Try going left/right and then jump.
			{
				BotDebug("%s -> Stuck with object, will go %s.", GetName(), m_bStuckTryGoLeft ? "left" : "right");

				m_bResolvingStuck = m_bStuckTryingSide = true;
				m_bStuckTryGoLeft = !m_bStuckTryGoLeft;
				m_bNeedJump = m_bNeedJumpDuck = true;
				m_fStartActionTime = CBotrixServerPlugin::GetTime() + 0.75f;
				m_fEndActionTime = CBotrixServerPlugin::GetTime() + 1.0f;
			}
		}
	}

	else if ( m_bUseNavigatorToMove )
	{
		bool bSameWaypoint = iCurrentWaypoint == iPrevCurrWaypoint;

		if ( bSameWaypoint )
		{
			m_cNavigator.SetPreviousPathPosition();  // Curreint -> next.
			m_cNavigator.SetPreviousPathPosition();  // Previous -> current.
			m_cNavigator.GetNextWaypoints(iNextWaypoint, m_iAfterNextWaypoint);
			if ( iCurrentWaypoint == iNextWaypoint )
			{
				m_cNavigator.GetNextWaypoints(iNextWaypoint, m_iAfterNextWaypoint);
			}
		}

		// Mark path as unreachable.
		if ( CWaypoints::HasPath(iCurrentWaypoint, iNextWaypoint) )
		{
			BotDebug("%s -> Mark unreachable path from %d to %d.", GetName(), iCurrentWaypoint, iNextWaypoint);
			CWaypoints::MarkUnreachablePath(iCurrentWaypoint, iNextWaypoint);
		}

		if ( bSameWaypoint )  // Got stuck because action didn't work?
		{
			m_bStuck = false;

			// First test if we are touching current waypoint.
			bool bTouch = wCurr.IsTouching(m_vHead, m_bLadderMove);
			if ( bTouch || m_bStuckGotoCurrent )
			{
				m_bResolvingStuck = m_bRepeatWaypointAction = true;

				if ( !bTouch )
				{
					ApplyPathFlags();  // Apply path flags from previous waypoint (to go back to current waypoint
									   // again).
					m_bNeedJump = rand() & 1;  // Jump for just in case sometimes.
				}
				m_bStuckGotoCurrent = false;  // Try to move left right next time.

				BotDebug(
					"%s -> Stuck, will go to previous waypoint %d (from %d).",
					GetName(),
					iNextWaypoint,
					iCurrentWaypoint
				);
			}
			else
			{
				// Check if next waypoint is more on left or right.
				CWaypoint& wNext = CWaypoints::Get(iNextWaypoint);
				Vector wNeed(wNext.vOrigin);
				wNeed -= wCurr.vOrigin;
				Vector wNow(m_vHead);
				wNow -= wCurr.vOrigin;

				Vector angNeed;
				Vector angNow;
				VectorAngles(wNeed, angNeed);
				VectorAngles(wNow, angNow);

				CBotrixEngineUtil::GetAngleDifference(angNow, angNeed, angNow);

				m_bStuckTryGoLeft = (angNow.y >= 0.0f);  // Need to increment angles, move left.
				m_bStuckTryingSide = true;
				m_bNeedJump = m_bNeedJumpDuck = true;
				m_fStartActionTime = CBotrixServerPlugin::GetTime() + 0.5f;
				m_fEndActionTime = CBotrixServerPlugin::GetTime() + 1.0f;

				m_bResolvingStuck = m_bStuckGotoCurrent = true;  // Try to go back to current waypoint next time.

				BotDebug("%s -> Stuck, try to move %s.", GetName(), m_bStuckTryGoLeft ? "left" : "right");
			}
			return true;
		}
		else  // Got stuck, because bot falled down or someone pushed it.
		{
			m_bPathAim = m_bLockNavigatorMove = false;  // Release locks.
			m_bNeedJumpDuck = m_bNeedJump = m_bNeedAttack2 = m_bNeedAttack = m_bNeedSprint = m_bNeedDuck = m_bNeedWalk =
				false;

			BotDebug("%s -> Stuck and lost, because of failure following path.", GetName());
			if ( m_bTest )
			{
				m_bStuck = false;
				m_bDestinationChanged = true;
			}
			else  // Lost path and stuck, move failure.
			{
				m_bMoveFailure = true;  // Let mod decide what to do.
			}

			m_cNavigator.Stop();
		}
	}
	else  // Not using navigator and stuck?
	{
		m_bMoveFailure = true;  // Let mod decide what to do.
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------
bool CBotrixBot::MoveBetweenWaypoints()
{
	if ( !CWaypoints::Get(iNextWaypoint).IsTouching(m_vHead, m_bLadderMove) )
	{
		return false;
	}

	iCurrentWaypoint = iNextWaypoint;  // Force current waypoint become the one we just reached.

	if ( m_bUseNavigatorToMove )
	{
		m_bNeedMove = m_bNeedMove && m_cNavigator.HasMoreCoords();

		if ( m_bNeedMove )
		{
			m_cNavigator.GetNextWaypoints(iNextWaypoint, m_iAfterNextWaypoint);
		}
		else
		{
			m_iAfterNextWaypoint = iNextWaypoint = EWaypointIdInvalid;
		}
	}
	else
	{
		m_iAfterNextWaypoint = iNextWaypoint = EWaypointIdInvalid;
		m_bNeedMove = false;
	}

	bool bDoingAction = DoWaypointAction();

	if ( m_bNeedMove && !bDoingAction )  // If not doing some waypoint action.
	{
		ApplyPathFlags();
		DoPathAction();
	}

	return true;  // We arrived to next waypoint.
}

//----------------------------------------------------------------------------------------------------------------
bool CBotrixBot::NavigatorMove()
{
	GoodAssert(m_bUseNavigatorToMove);
	bool bArrived = false;

	if ( m_bDestinationChanged )
	{
		// Destination changed, make sure to start new path search.
		m_bDestinationChanged = false;
		m_cNavigator.Stop();

		if ( iCurrentWaypoint == m_iDestinationWaypoint )
		{
			m_bNeedMove = false;
			return true;
		}

		BASSERT(CWaypoint::IsValid(iCurrentWaypoint) && CWaypoint::IsValid(m_iDestinationWaypoint), return true);
		m_bMoveFailure = !m_cNavigator.SearchSetup(iCurrentWaypoint, m_iDestinationWaypoint, m_aAvoidAreas);
		iPrevWaypoint = iCurrentWaypoint;
	}
	else if ( m_cNavigator.PathFound() )  // Check if bot arrived to destination. Set up new destination to next
										  // waypoint in path.
	{
		return MoveBetweenWaypoints();
	}

	if ( !m_bMoveFailure )
	{
		// Here search is still not finished.
		bArrived = m_cNavigator.SearchStep();

		if ( bArrived )  // Ended search just now.
		{
			m_bMoveFailure = !m_cNavigator.PathFound();

			if ( m_bMoveFailure )
			{
				BotDebug("%s -> Can't reach waypoint %d.", GetName(), m_iDestinationWaypoint);
			}
			else
			{
				GoodAssert(m_cNavigator.PathFound() && m_cNavigator.HasMoreCoords());
				m_cNavigator.GetNextWaypoints(iNextWaypoint, m_iAfterNextWaypoint);
				ApplyPathFlags();

				// First coord in path must be current waypoint.
				// GoodAssert( iCurrentWaypoint == iNextWaypoint );

				// If lost and iCurrentWaypoint == iNextWaypoint, perform waypoint 'touch'.
				if ( CWaypoints::Get(iNextWaypoint).IsTouching(m_vHead, m_bLadderMove) )
				{
					if ( m_cNavigator.HasMoreCoords() )
					{
						m_cNavigator.GetNextWaypoints(iNextWaypoint, m_iAfterNextWaypoint);
						if ( DoWaypointAction() == false )  // No need to perform waypoint action.
						{
							ApplyPathFlags();
							DoPathAction();
						}
					}
					else
					{
						GoodAssert(iCurrentWaypoint == m_iDestinationWaypoint);
						m_bNeedMove = false;  // We have arrived at destination.
					}
				}
			}
		}
	}
	return bArrived;
}

//----------------------------------------------------------------------------------------------------------------
bool CBotrixBot::NormalMove()
{
	GoodAssert(m_bNeedMove);
	bool bArrived = false;

	if ( m_bDestinationChanged && CWaypoint::IsValid(iNextWaypoint) )
	{
		NotifyIfFailed(
			iNextWaypoint != iCurrentWaypoint,
			"Expected next waypoint %d to be different to current waypoint\n",
			iNextWaypoint
		);

		if ( iNextWaypoint == iCurrentWaypoint )
		{
			return true;
		}

		m_vDestination = CWaypoints::Get(iNextWaypoint).vOrigin;
		DoPathAction();
		m_bDestinationChanged = false;
	}
	else
	{
		// Need to move only when not arrived.
		bArrived = CWaypoint::IsValid(iNextWaypoint) ? MoveBetweenWaypoints()
													 : CBotrixEngineUtil::IsPointTouch3d(
														   m_vHead,
														   m_vDestination,
														   CBotrixMod::iPointTouchSquaredZ,
														   CBotrixMod::iPointTouchSquaredXY
													   );
		m_bNeedMove = !bArrived;
	}

	return bArrived;
}

//----------------------------------------------------------------------------------------------------------------
void CBotrixBot::PerformMove(TWaypointId iPreviousWaypoint, const Vector& vPrevOrigin)
{
	// m_cCmd.viewangles = m_pController->GetLocalAngles(); // TODO: WTF?
	// m_cCmd.viewangles = m_PlayerInfo.GetAbsAngles(); // WTF?

	if ( CheckMoveFailure() )
	{
		m_cNavigator.Stop();
		return;
	}

	if ( !m_bLastNeedMove && m_bNeedMove )  // Bot started to move just now, update stuck check time.
	{
		m_fStuckCheckTime = CBotrixServerPlugin::GetTime() + 1.0f;
	}

	m_bLastNeedMove = m_bNeedMove;

	// Waypoint just changed from previous and valid waypoint.
	bool bCurrentWaypointChanged = CWaypoint::IsValid(iPreviousWaypoint) && (iPreviousWaypoint != iCurrentWaypoint);
	if ( bCurrentWaypointChanged )
	{
		CurrentWaypointJustChanged();
	}

	bool bArrived = false;  // Set to true if reached next waypoint in path or path searching just finished (then
							// touched start waypoint).

	if ( m_bNeedMove && !m_bMoveFailure )
	{
		bArrived = (m_bUseNavigatorToMove) ? NavigatorMove() : NormalMove();
	}

	bool bMove = m_bNeedMove && !m_bMoveFailure && !m_bStuckBreakObject && !m_bStuckUsePhyscannon &&
		!m_bCommandStopped && !m_bAttackDuck;

	if ( m_bUseNavigatorToMove && (!m_cNavigator.SearchEnded() || m_bLockNavigatorMove) )
	{
		bMove = false;  // Check if search is finished (m_bMoveFailure will be set if search fails).
	}

	// Check if need to look left/right/back/forward.
	CheckSideLook(bMove, bArrived);

	// Need to aim somewhere? TODO: faster...
	if ( m_bNeedAim )
	{
		// QAngle angOld = m_cCmd.viewangles;

		// Calculate how much frames left to m_fEndAimTime.
		float fTimeLeft = m_fEndAimTime - CBotrixServerPlugin::GetTime();
		int iFramesLeft = (int)(fTimeLeft / std::max<float>(gpGlobals->frametime, 0.01f));

		if ( iFramesLeft > 0 )
		{
			// Make smoother angle change (break in several angle changes).
			// https://developer.valvesoftware.com/wiki/QAngle
			Vector vDestinationAim(m_vLook);
			vDestinationAim -= m_vHead;  // Get abs vector.

			Vector angNeeded;
			VectorAngles(vDestinationAim, angNeeded);

			CBotrixEngineUtil::GetAngleDifference(m_cCmd.viewangles, angNeeded, angNeeded);
			angNeeded /= static_cast<float>(iFramesLeft);

			m_cCmd.viewangles += angNeeded;
		}
		else
		{
			Vector vAbs(m_vLook);
			vAbs -= m_vHead;
			VectorAngles(vAbs, m_cCmd.viewangles);  // Convert vector to angles.
			m_bNeedAim = false;
		}

		CBotrixEngineUtil::DeNormalizeAngle(m_cCmd.viewangles.x);
		CBotrixEngineUtil::DeNormalizeAngle(m_cCmd.viewangles.y);

		if ( !m_bEnemyAim )
		{
			if ( m_cCmd.viewangles.x > 60.0f )
			{
				m_cCmd.viewangles.x = 60.0f;
			}
			else if ( m_cCmd.viewangles.x < -60.0f )
			{
				m_cCmd.viewangles.x = -60.0f;
			}
		}
		// CBotrixEngineUtil::DrawLine( m_vHead, m_vLook, 0.1f, 0xFF, 0xFF, 0xFF );
	}

	// Check if need moving.
	if ( bMove )
	{
		float fDeltaTime = CBotrixServerPlugin::GetTime() - m_fPrevThinkTime;

		if ( fDeltaTime == 0 )
		{
			fDeltaTime = 0.000001f;  // Should not happen, sometimes happens. TODO: don't think in PreThink()...
		}

		// Calculate distance from last frame.
		Vector vSpeed(m_vHead);
		vSpeed -= vPrevOrigin;
		vSpeed /= fDeltaTime;  // v = (x - x0)/t

		// Cancel stuck check if bot needs to stop, or use, or perform action, or it is stuck already.
		if ( m_bNeedStop || m_bNeedUse || m_bStuck || m_bStuckTryingSide ||
			 CBotrixServerPlugin::GetTime() <= m_fEndActionTime )
		{
			m_bNeedCheckStuck = false;
		}
		else if ( !m_bNeedCheckStuck )  // Bot starts to move, set up next stuck check time.
		{
			// Check if bot is stuck after one second.
			m_bNeedCheckStuck = true;
			m_vStuckCheck = m_vHead;
			m_fStuckCheckTime = CBotrixServerPlugin::GetTime() + 1.0f;
			pStuckObject = NULL;
		}
		else if ( CBotrixServerPlugin::GetTime() >= m_fStuckCheckTime )
		{
			m_bNeedCheckStuck = false;

			// Distance that bot moves since m_fStuckCheckTime.
			if ( (m_vHead - m_vStuckCheck).LengthSquared() < SQR(CBotrixMod::fMinNonStuckSpeed) )
			{
				m_bNeedCheckStuck = true;
				m_fStuckCheckTime = CBotrixServerPlugin::GetTime() + 5.0f;  // Try again in 5 secs.

				if ( !ResolveStuckMove() )
				{
					return;
				}
			}
		}

		// Calculate forward and side velocities to reach destination.
		// Vector vAccelerate(vSpeed);
		// vAccelerate -= m_vLastVelocity;
		// vAccelerate /= fDeltaTime;   // a = (v - v0)/t

		// m_vLastVelocity = vSpeed;
		Vector vNeededVelocity(m_vDestination);
		float fSpeed = 0.0f;

		// Need to stop before next move.
		if ( m_bNeedStop && (vSpeed.x == 0) && (vSpeed.y == 0) && (vSpeed.z == 0) )
		{
			m_bNeedStop = false;
		}

		if ( m_bNeedStop )
		{
			vNeededVelocity =
				CWaypoints::Get(iCurrentWaypoint).vOrigin;  // Move to current waypoint instead of next one.
			vNeededVelocity -= m_vHead;  // Abs vector.
			vNeededVelocity -= vSpeed;
			fSpeed = vNeededVelocity.Length();
		}
		else
		{
			if ( m_bNeedSprint )
			{
				fSpeed = CBotrixParameterVars::PLAYER_SPRINT_SPEED;
			}
			else if ( m_bNeedWalk )
			{
				fSpeed = CBotrixParameterVars::PLAYER_WALK_SPEED;
			}
			else
			{
				fSpeed = CBotrixParameterVars::PLAYER_RUN_SPEED;
			}

			vNeededVelocity -= m_vHead;  // Destination - head (absolute vector).

			if ( m_bStuckTryingSide )  // Currently stuck, trying to move left/right.
			{
				if ( CBotrixServerPlugin::GetTime() < m_fEndActionTime )
				{
					// If bot was stuck then move left or right accordingly.
					Vector vUp, vRight;
					VectorVectors(vNeededVelocity, vRight, vUp);
					if ( m_bStuckTryGoLeft )
					{
						vRight *= -1.0f;
					}
					vNeededVelocity = vRight;
					// BotDebug("Stuck, trying side until %.1f, now %.1f", m_fEndActionTime,
					// CBotrixServerPlugin::GetTime());
				}
				else
				{
					BotDebug("%s -> Stuck, end moving left/right.", GetName());
					m_bStuckTryingSide = false;
				}
			}

			vNeededVelocity = vNeededVelocity.Normalize();
			vNeededVelocity *= fSpeed;
			vNeededVelocity -= vSpeed;
		}

		Vector angNeeded;
		VectorAngles(vNeededVelocity, angNeeded);

		CBotrixEngineUtil::NormalizeAngle(angNeeded.y);
		CBotrixEngineUtil::GetAngleDifference(m_cCmd.viewangles, angNeeded, angNeeded);

		// CBotrixEngineUtil::DeNormalizeAngle(angNeeded.y);
		// float fYaw = (angNeeded.y * 3.14159265f) / 180.0f; // Degree to radians.
		// m_cCmd.forwardmove = fSpeed * FastCos(fYaw);
		// m_cCmd.sidemove = -fSpeed * FastSin(fYaw);

		// Vector vMove(m_cCmd.forwardmove, m_cCmd.sidemove, 0);
		// vMove += m_vHead;
		// CBotrixEngineUtil::DrawLine( m_vHead, vMove, 0.1f, 0xFF, 0xFF, 0xFF );

		// Check forward/backward move.
		if ( (-45.0f <= angNeeded.y) && (angNeeded.y <= 45.0f) )
		{
			m_cCmd.forwardmove = fSpeed;
		}
		else if ( (angNeeded.y <= -135.0f) || (angNeeded.y >= 135.0f) )
		{
			m_cCmd.forwardmove = -fSpeed;
		}

		// Check left/right move.
		if ( (45.0f <= angNeeded.y) && (angNeeded.y <= 135.0f) )
		{
			m_cCmd.sidemove = -fSpeed;
		}
		else if ( (-135.0f <= angNeeded.y) && (angNeeded.y <= -45.0f) )
		{
			m_cCmd.sidemove = fSpeed;
		}

		// BLOG_D("Forward %f, side %f", m_cCmd.forwardmove, m_cCmd.sidemove);
	}
	else  // No need to move.
	{
		m_bNeedCheckStuck = false;
		/*
		m_vLastVelocity = m_vLastOrigin;
		m_vLastVelocity -= m_vHead;
		m_vLastVelocity /= fDeltaTime;
		*/
	}

	// Check if stuck object is still near.
	if ( pStuckObject )
	{
		bool bFound = false;
		for ( int i = 0; i < m_aNearestItems[EItemTypeObject].size(); ++i )
		{
			TItemIndex iItem = m_aNearestItems[EItemTypeObject][i];
			const CItem* pObject = &CItems::GetItems(EItemTypeObject)[iItem];
			if ( pObject == pStuckObject )
			{
				bFound = true;
				break;
			}
		}
		if ( !bFound )
		{
			pStuckObject = NULL;
		}
	}

	//---------------------------------------------------------------
	if ( !m_bDontAttack )
	{
		CWeaponWithAmmo* pWeapon = CWeapons::IsValid(m_iWeapon) ? &m_aWeapons[m_iWeapon] : NULL;
		if ( m_pCurrentEnemy )
		{
			GoodAssert(m_pCurrentEnemy != this);
			if ( !m_bNeedAim )
			{
				m_bEnemyAimed = true;
				m_bStayReloading = false;  // At reload time, check if it is better to change weapon. TODO: check.

				// Aim again.
				EnemyAim();
			}

			if ( m_bEnemyAimed && !m_bEnemyOffSight )
			{
				if ( pWeapon )
				{
					if ( pWeapon->CanUse() )  // Stay shooting after first time aimed at enemy.
					{
						bool bZooming = false;
						if ( pWeapon->IsSniper() )  // Check if need to zoom or out.
						{
							bool bNeedZoom = pWeapon->ShouldZoom(m_fDistanceSqrToEnemy);
							if ( (bNeedZoom && !pWeapon->IsUsingZoom()) || (!bNeedZoom && pWeapon->IsUsingZoom()) )
							{
								WeaponToggleZoom();
								bZooming = true;
							}
						}

						if ( !bZooming )
						{
							// Hit with melee.
							if ( pWeapon->IsMelee() )
							{
								int iSec = pWeapon->Damage(CWeapon::PRIMARY) < pWeapon->Damage(CWeapon::SECONDARY);

								if ( pWeapon->IsDistanceSafe(m_fDistanceSqrToEnemy, iSec) )
								{
									WeaponShoot(iSec);
								}
								else if ( !m_bStuckBreakObject )
								{
									WeaponChoose();
								}
							}
							// Prefer secondary attack.
							else if ( pWeapon->HasAmmoInClip(CWeapon::SECONDARY) &&
									  pWeapon->IsDistanceSafe(m_fDistanceSqrToEnemy, CWeapon::SECONDARY) )
							{
								WeaponShoot(CWeapon::SECONDARY);
							}
							else if ( pWeapon->HasAmmoInClip(CWeapon::PRIMARY) &&
									  pWeapon->IsDistanceSafe(m_fDistanceSqrToEnemy, CWeapon::PRIMARY) )
							{
								WeaponShoot(CWeapon::PRIMARY);
							}
							else if ( !m_bStuckUsePhyscannon && pWeapon->IsPhysics() )
							{
								WeaponChoose();  // No more bullets, select another weapon.
							}
						}
					}
				}
				else if ( rand() % 20 )  // 19 out of 20 times shoot primary.
				{
					WeaponShoot(CWeapon::PRIMARY);
				}
				else if ( (rand() % 4) == 0 )
				{
					WeaponShoot(CWeapon::SECONDARY);
				}
			}
		}
		else  // No enemy.
		{
			m_bEnemyAim = m_bAttackDuck = false;  // Bot is not under attack now.
			if ( pWeapon )
			{
				if ( pWeapon->IsSniper() && pWeapon->IsUsingZoom() && pWeapon->CanUse() )
				{
					WeaponToggleZoom();
				}
				else if ( m_bNeedReload && !pWeapon->IsReloading() && !m_bStuckUsePhyscannon && !m_bStuckBreakObject )
				{
					WeaponChoose();
				}
			}
		}
	}

	//---------------------------------------------------------------
	// Aim finished and need start to use IN_USE button.
	if ( m_bNeedUse && !m_pCurrentEnemy && !m_bNeedAim )
	{
		if ( !m_bAlreadyUsed )
		{
			FLAG_SET(IN_USE, m_cCmd.buttons);  // For button we just press USE for one frame.
			if ( m_bUsingHealthMachine || m_bUsingArmorMachine )
			{
				if ( CBotrixServerPlugin::GetTime() >= m_fEndActionTime )
				{
					BotDebug("%s -> End using health/armor charger.", GetName());

					// Ended using machine, check if need to use again.
					int iHealthArmor = m_bUsingHealthMachine ? m_PlayerInfo.GetHealth() : m_PlayerInfo.GetArmorValue();
					if ( iHealthArmor == m_iLastHealthArmor )
					{
						m_bAlreadyUsed = true;
					}
					else
					{
						// BLOG_T("Incremented %d of %s, now %d.", iHealthArmor - m_iLastHealthArmor,
						// m_bUsingHealthMachine ? "health" : "armor", iHealthArmor);
						m_iLastHealthArmor = iHealthArmor;
						m_fEndActionTime = CBotrixServerPlugin::GetTime() + m_fTimeIntervalCheckUsingMachines;
					}
				}
			}
			else
			{
				m_bAlreadyUsed = true;
			}
		}

		if ( m_bAlreadyUsed )
		{
			m_bNeedUse = false;
			// Only apply path flags if we needed to USE, otherwise they are already applied.
			if ( m_bUsingHealthMachine || m_bUsingArmorMachine || m_bUsingButton )
			{
				m_bUsingHealthMachine = m_bUsingArmorMachine = m_bUsingButton = false;
				if ( CWaypoint::IsValid(iNextWaypoint) )
				{
					ApplyPathFlags();
					DoPathAction();
				}
			}
		}
	}

	// If bot is on ladder forwardmove and sidemove are not used, but IN_FORWARD & IN_BACK used instead.
	if ( m_bLadderMove )
	{
		FLAG_SET(IN_FORWARD, m_cCmd.buttons);  // Should be looking at next waypoint.
	}

	// Jump only once.
	if ( m_bNeedJump && (CBotrixServerPlugin::GetTime() >= m_fStartActionTime) )
	{
		FLAG_SET(IN_JUMP, m_cCmd.buttons);
		m_bNeedJump = false;
	}

	// Start holding duck for jump after m_fStartActionTime.
	if ( m_bNeedJumpDuck && (CBotrixServerPlugin::GetTime() >= m_fStartActionTime + 0.1f) )
	{
		// Stop holding duck for jump after m_fEndActionTime.
		if ( CBotrixServerPlugin::GetTime() >= m_fEndActionTime )
		{
			m_bNeedJumpDuck = false;
		}
		else
		{
			FLAG_SET(IN_DUCK, m_cCmd.buttons);
		}
	}

	// Start holding attack after m_fStartActionTime.
	if ( m_bNeedAttack )
	{
		// Set melee weapon.
		if ( (m_iWeapon != m_iMeleeWeapon) && CWeapons::IsValid(m_iMeleeWeapon) )
		{
			if ( m_aWeapons[m_iWeapon].CanChange() )
			{
				ChangeWeapon(m_iMeleeWeapon);
			}
		}
		else
		{
			// Check if some object is near.
			if ( pStuckObject && pStuckObject->IsBreakable() && !pStuckObject->IsExplosive() )
			{
				m_bResolvingStuck = m_bStuckBreakObject =
					true;  // Try to break the object, instead of just hitting blindly.
				m_bNeedAttack = false;
			}
			else if ( CBotrixServerPlugin::GetTime() < m_fEndActionTime )  // Stop attacking after m_fEndActionTime.
			{
				if ( CWeapons::IsValid(m_iWeapon) && m_aWeapons[m_iWeapon].CanUse() )
				{
					WeaponShoot();
				}
			}
			else
			{
				m_bNeedAttack = false;
				m_bNeedSetWeapon = true;
			}
		}
	}

	// Aim at object (at m_fStartActionTime), press ATTACK2 (physcannon) for a second, aim back, press ATTACK1 once.
	if ( m_bStuckUsePhyscannon )
	{
		// Still not finished throwing object.
		if ( pStuckObject && (CBotrixServerPlugin::GetTime() < m_fEndActionTime) )
		{
			if ( !m_bStuckPhyscannonEnd )
			{
				if ( m_iWeapon != m_iPhyscannon )  // Still need to switch weapon to physcannon.
				{
					if ( m_aWeapons[m_iWeapon].CanChange() )
					{
						ChangeWeapon(m_iPhyscannon);
						float fPhyscannonUseTime = m_aWeapons[m_iPhyscannon].GetEndTime();
						if ( fPhyscannonUseTime > m_fEndAimTime + 0.1f )
						{
							m_fEndAimTime = fPhyscannonUseTime - 0.1f;
						}
						else
						{
							fPhyscannonUseTime = m_fEndAimTime + 0.1f;
						}
						m_fStartActionTime = fPhyscannonUseTime;
					}
				}
				else  // Current weapon is physcannon.
				{
					if ( (CBotrixServerPlugin::GetTime() >=
						  m_fStartActionTime) )  // Aimed at object, can use physcannon now.
					{
						if ( CBotrixServerPlugin::GetTime() <
							 m_fStartActionTime + 0.5f + 0.1f * (EBotPro - m_iIntelligence + 1) )
						{
							BotDebug("%s -> Use physcannon on object.", GetName());
							WeaponShoot(CWeapon::SECONDARY);  // Attract object for half second.
						}
						else
						{
							/*if ( pObject && (pObject->CurrentPosition() == m_vDisturbingObjectPosition) )
							{
								BotDebug( "%s -> Object position didn't change, aborting.", GetName() );
								// Bot can't pick it up? TODO: Maybe bot is standing on object.
								// FLAG_SET(FObjectHeavy, ((CItem*)pObject)->iFlags);
								WeaponShoot(CWeapon::PRIMARY);
								m_bResolvingStuck =m_bStuckUsePhyscannon = m_bPathAim = false;
								m_bNeedSetWeapon = true;
							}
							else */
							if ( m_pCurrentEnemy && !m_bTest && !m_bDontAttack )  // Throw it at enemy.
							{
								BotDebug("%s -> Holding object, aim enemy.", GetName());
								m_bNeedAim = true;
								m_pCurrentEnemy->GetCenter(m_vLook);
								m_fEndAimTime = CBotrixServerPlugin::GetTime() + GetEndLookTime();
								m_fEndActionTime = m_fEndAimTime + 0.1f * (EBotPro - m_iIntelligence + 1);
							}
							else
							{
								BotDebug("%s -> Holding object, aim back.", GetName());
								// Look back while holding object.
								m_bNeedAim = true;
								m_vLook = m_vHead;
								m_vLook -= m_vDestination;
								m_vLook += m_vHead;  // back = (head - waypoint) + head.

								m_fEndAimTime = CBotrixServerPlugin::GetTime() + GetEndLookTime();
								m_fEndActionTime = m_fEndAimTime + 0.1f * (EBotPro - m_iIntelligence + 1);
							}
							m_bStuckPhyscannonEnd = true;
						}
					}
				}
			}
		}
		else  // Throw object and restore previous weapon.
		{
			// Bot should be holding object, throw it away.
			if ( m_iWeapon == m_iPhyscannon )
			{
				WeaponShoot(CWeapon::PRIMARY);
			}
			m_bResolvingStuck = m_bStuckUsePhyscannon = m_bPathAim = false;
			m_bNeedSetWeapon = true;
		}
	}

	// Start holding attack after m_fStartActionTime until break object or object moves far away.
	else if ( m_bStuckBreakObject )
	{
		if ( pStuckObject )
		{
			if ( m_bFeatureWeaponCheck && CWeapons::IsValid(m_iMeleeWeapon) && (m_iWeapon != m_iMeleeWeapon) &&
				 m_aWeapons[m_iWeapon].CanChange() &&
				 (!m_pCurrentEnemy || (m_iWeapon == m_iPhyscannon) ||
				  !m_aWeapons[m_iWeapon].CanBeUsed((GetHead() - pStuckObject->CurrentPosition()).LengthSquared())) )
			{
				ChangeWeapon(m_iMeleeWeapon);
				float fEndChange = m_aWeapons[m_iWeapon].GetEndTime();
				if ( m_fStartActionTime < fEndChange )
				{
					m_fStartActionTime = fEndChange;
				}
			}

			if ( CBotrixServerPlugin::GetTime() >= m_fStartActionTime )  // Aimed.
			{
				if ( pStuckObject && pStuckObject->IsBreakable() &&
					 !pStuckObject->IsExplosive() )  // Object still there.
				{
					if ( CWeapons::IsValid(m_iWeapon) && m_aWeapons[m_iWeapon].CanUse() )
					{
						WeaponShoot();
					}

					if ( (CBotrixServerPlugin::GetTime() >= m_fEndAimTime + 1.0f) )  // Maybe object moved.
					{
						m_bNeedAim = true;
						m_vLook = pStuckObject->CurrentPosition();
						int lookDelta = 4 - (rand() & 0x7);  // Maybe object has holes: add random from -3 to 4.
						m_vLook.x += lookDelta;
						m_vLook.y += -lookDelta;
						m_vLook.z += lookDelta;
						m_fEndAimTime = CBotrixServerPlugin::GetTime() + GetEndLookTime();
					}
				}
				else  // Object broken, restore previous weapon.
				{
					m_bStuckBreakObject = m_bPathAim = false;
					m_bNeedSetWeapon = true;
				}
			}
		}
		else
		{
			BotDebug("%s -> Object far or broken.", GetName());
			m_bResolvingStuck = m_bStuckBreakObject = false;
		}
	}

	// Duck until bot reaches next waypoint.
	if ( m_bNeedDuck || m_bAttackDuck )
	{
		FLAG_SET(IN_DUCK, m_cCmd.buttons);
	}

#ifdef BOTRIX_TODO
	// Sprint until bot reaches next waypoint. Not working.
	if ( m_bNeedSprint )
	{
		FLAG_SET(IN_SPEED, m_cCmd.buttons);
	}
#endif  // BOTRIX_TODO

	// Toggle flashlight.
	if ( m_bNeedFlashlight ^ m_bUsingFlashlight )
	{
		m_bUsingFlashlight = !m_bUsingFlashlight;
		m_cCmd.impulse = 100;
	}

	if ( m_bNeedSetWeapon )
	{
		WeaponChoose();
	}
}

//----------------------------------------------------------------------------------------------------------------
CBot_HL2DM::CBot_HL2DM(edict_t* pEdict, TBotIntelligence iIntelligence) :
	CBotrixBot(pEdict, iIntelligence, -1),
	m_aWaypoints(CWaypoints::Size()),
	m_cItemToSearch(-1, -1),
	m_cSkipWeapons(CWeapons::Size()),
	m_pChasedEnemy(NULL)
{
	m_bShootAtHead = false;
}

//----------------------------------------------------------------------------------------------------------------
void CBot_HL2DM::Activated()
{
	CBotrixBot::Activated();
}

void CBot_HL2DM::ChangeModel(TTeam iTeam)
{
	if ( iTeam == CBotrixMod::iUnassignedTeam )
	{
		iTeam = 2 + (rand() & 1);  // 0 = unassigned, 1 = spectator, 2 = rebels, 3 = combines.
	}
	else
	{
		char sCmd[32];
		PlatformLib_SNPrintF(sCmd, sizeof(sCmd), "jointeam %d", iTeam);
		g_engfuncs.pfnClientCommand(m_pEdict, sCmd);
	}

	if ( CBotrixMod::bUseModels )
	{
		const good::string* pModel = CBotrixMod::pCurrentMod->GetRandomModel(iTeam);
		if ( pModel )
		{
			BLOG_I("%s -> Using model %s", GetName(), pModel->c_str());

			g_engfuncs.pfnSetClientKeyValue(
				ENTINDEX(m_pEdict),
				g_engfuncs.pfnGetInfoKeyBuffer(m_pEdict),
				"model",
				(char*)pModel->c_str()
			);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBot_HL2DM::Respawned()
{
	CBotrixBot::Respawned();

	m_aWaypoints.reset();
	m_iFailWaypoint = EWaypointIdInvalid;

	m_iCurrentTask = EBotTaskInvalid;
	m_bNeedTaskCheck = true;
	m_bChasing = false;
}

//----------------------------------------------------------------------------------------------------------------
void CBot_HL2DM::HurtBy(int iPlayerIndex, CPlayer* pAttacker, int iHealthNow)
{
	if ( !m_bTest && !m_bDontAttack && (pAttacker != this) && IsEnemy(pAttacker) )
	{
		CheckEnemy(iPlayerIndex, pAttacker, false);
	}

	if ( iHealthNow < (CBotrixParameterVars::PLAYER_MAX_HEALTH / 2.0f) )
	{
		m_bNeedTaskCheck = true;  // Check if need search for health.
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBot_HL2DM::Think()
{
	GoodAssert(!m_bTest);
	if ( !m_bAlive )
	{
		m_cCmd.buttons = rand() & IN_ATTACK;  // Force bot to respawn by hitting randomly attack button.
		return;
	}

	bool bForceNewTask = false;

	if ( !m_bNeedMove && !m_bResolvingStuck && !m_pCurrentEnemy )
	{
		m_bMoveFailure = true;
	}

	// Check for move failure.
	if ( m_bMoveFailure )
	{
		if ( m_bUseNavigatorToMove )
		{
			if ( iCurrentWaypoint == m_iFailWaypoint )
			{
				m_iFailsCount++;
			}
			else
			{
				m_iFailWaypoint = iCurrentWaypoint;
				m_iFailsCount = 1;
			}

			if ( m_iFailsCount >= 3 )
			{
				BotDebug(
					"%s -> Failed to find a path from %d to %d 3 times, marking task as finished.",
					GetName(),
					m_iFailWaypoint,
					m_iDestinationWaypoint
				);

				TaskFinished();
				m_bNeedTaskCheck = bForceNewTask = true;
				m_iFailsCount = 0;
				m_iFailWaypoint = -1;
			}
			else if ( m_bMoveFailure )
			{
				// Recalculate the route.
				m_iDestinationWaypoint = m_iTaskDestination;
				m_bNeedMove = m_bUseNavigatorToMove = m_bDestinationChanged = true;
			}
		}
		else
		{
			m_bNeedMove = false;  // As if we got to the needed destination.
		}

		m_bMoveFailure = m_bStuck = m_bResolvingStuck = false;
	}

	// Check if needs to add new tasks.
	if ( m_bNeedTaskCheck )
	{
		m_bNeedTaskCheck = false;
		CheckNewTasks(bForceNewTask);
	}

	if ( m_pCurrentEnemy && !m_bFlee && (m_iCurrentTask != EBotTaskEngageEnemy) )
	{
		m_iCurrentTask = EBotTaskEngageEnemy;
		m_pChasedEnemy = m_pCurrentEnemy;
		m_bNeedMove = false;
	}

	if ( m_iCurrentTask == EBotTaskEngageEnemy )
	{
		CheckEngagedEnemy();
	}
}

//----------------------------------------------------------------------------------------------------------------
void CBot_HL2DM::ReceiveChat(int /*iPlayerIndex*/, CPlayer* /*pPlayer*/, bool /*bTeamOnly*/, const char* /*szText*/)
{
}

//----------------------------------------------------------------------------------------------------------------
bool CBot_HL2DM::DoWaypointAction()
{
	if ( m_bUseNavigatorToMove && (iCurrentWaypoint == m_iTaskDestination) )
	{
		m_iCurrentTask = EBotTaskInvalid;
		m_bNeedTaskCheck = true;
	}
	return CBotrixBot::DoWaypointAction();
}

//----------------------------------------------------------------------------------------------------------------
void CBot_HL2DM::CheckEngagedEnemy()
{
	GoodAssert(m_iCurrentTask == EBotTaskEngageEnemy);

	if ( m_pCurrentEnemy != m_pChasedEnemy )  // Start/stop seeing enemy or enemy change.
	{
		if ( m_pCurrentEnemy )  // Seeing new enemy.
		{
			m_pChasedEnemy = m_pCurrentEnemy;
			m_bChasing = CWeapons::IsValid(m_iWeapon) &&
				(m_aWeapons[m_iWeapon].IsMelee() || m_aWeapons[m_iWeapon].NeedsToBeCloser(m_fDistanceSqrToEnemy));
			m_bNeedMove = m_bUseNavigatorToMove = false;  // Start moving on next tick.
		}
		else
		{
			// No current enemy is visible.
			if ( m_pChasedEnemy->IsAlive() )
			{
				if ( !m_bChasing || !m_bNeedMove )  // Lost sight of enemy, chase.
				{
					if ( iCurrentWaypoint == m_pChasedEnemy->iCurrentWaypoint )
					{
						m_pCurrentEnemy = m_pChasedEnemy;
					}
					else
					{
						ChaseEnemy();
					}
				}
			}
			else  // Enemy dead, stop task.
			{
				m_bChasing = false;
				m_pChasedEnemy = NULL;
				m_iCurrentTask = EBotTaskInvalid;
				m_bNeedTaskCheck = true;
			}
		}
	}
	else if ( !m_bNeedMove || (!m_bUseNavigatorToMove && !CWaypoint::IsValid(iNextWaypoint)) )
	{
		// Bot arrived where enemy is or to next random waypoint.
		// Tip: current enemy == chased enemy.
		GoodAssert(m_pChasedEnemy);
		// Bot arrived at adyacent waypoint or is seeing enemy.
		m_bNeedMove = m_bDestinationChanged = true;
		m_bUseNavigatorToMove = m_bChasing = false;

		if ( CWeapons::IsValid(m_iWeapon) )
		{
			if ( m_aWeapons[m_iWeapon].IsMelee() )  // Rush toward enemy.
			{
				bool bNear = m_pChasedEnemy->iCurrentWaypoint == iCurrentWaypoint;

				if ( !bNear )
				{
					CWaypointPath* pPath = CWaypoints::GetPath(iCurrentWaypoint, m_pChasedEnemy->iCurrentWaypoint);
					bNear = pPath && !pPath->IsActionPath();
				}

				if ( bNear )
				{
					// We are near enemy waypoint using melee weapon, rush to enemy head.
					iNextWaypoint = EWaypointIdInvalid;
					m_vDestination = m_pCurrentEnemy->GetHead();
					m_bNeedMove = true;
				}
				else if ( !m_bChasing || (CBotrixServerPlugin::GetTime() >= m_fChaseEnemyTime) )
				{
					ChaseEnemy();
				}
				return;
			}
			else if ( CWaypoints::bValidVisibilityTable && CWaypoint::IsValid(m_pCurrentEnemy->iCurrentWaypoint) &&
					  (m_iIntelligence >= EBotNormal) )
			{
				bool bNeedComeCloser =
					m_aWeapons[m_iWeapon].IsMelee() || m_aWeapons[m_iWeapon].NeedsToBeCloser(m_fDistanceSqrToEnemy);
				if ( bNeedComeCloser ||
					 (FLAG_SOME_SET(FFightStrategyComeCloserIfFar, CBotrixBot::iDefaultFightStrategy) &&
					  m_fDistanceSqrToEnemy >= CBotrixBot::fNearDistanceSqr) )
				{
					// Try to come closer a little.
					iNextWaypoint =
						CWaypoints::GetNearestNeighbour(iCurrentWaypoint, m_pCurrentEnemy->iCurrentWaypoint, true);
					BotDebug(
						"%s -> Moving to nearest waypoint %d (current %d)",
						GetName(),
						iNextWaypoint,
						iCurrentWaypoint
					);
					return;
				}
				else if ( FLAG_SOME_SET(FFightStrategyRunAwayIfNear, CBotrixBot::iDefaultFightStrategy) &&
						  (m_fDistanceSqrToEnemy <= CBotrixBot::fNearDistanceSqr) )
				{
					// Try to run away a little.
					iNextWaypoint =
						CWaypoints::GetFurthestNeighbour(iCurrentWaypoint, m_pCurrentEnemy->iCurrentWaypoint, true);
					BotDebug(
						"%s -> Moving to farest waypoint %d (current %d)",
						GetName(),
						iNextWaypoint,
						iCurrentWaypoint
					);
					return;
				}
			}
		}

		iNextWaypoint = CWaypoints::GetRandomNeighbour(iCurrentWaypoint, m_pCurrentEnemy->iCurrentWaypoint, true);
		BotDebug(
			"%s -> Moving to random neighbour waypoint %d (current %d)",
			GetName(),
			iNextWaypoint,
			iCurrentWaypoint
		);
	}
	else if ( m_pCurrentEnemy )
	{
		if ( m_bUseNavigatorToMove && CWeapons::IsValid(m_iWeapon) &&
			 (!m_aWeapons[m_iWeapon].IsMelee() && !m_aWeapons[m_iWeapon].NeedsToBeCloser(m_fDistanceSqrToEnemy)) )
		{
			m_bNeedMove = m_bUseNavigatorToMove = false;  // Stop running and start moving randomly.
		}
		else if ( m_bChasing && (iCurrentWaypoint != m_pChasedEnemy->iCurrentWaypoint) &&
				  (CBotrixServerPlugin::GetTime() >= m_fChaseEnemyTime) )
		{
			ChaseEnemy();  // Recalculate route to enemy every 3 seconds.
		}
	}
}

#ifdef BOTRIX_OLD_TASK_CODE
//----------------------------------------------------------------------------------------------------------------
void CBot_HL2DM::CheckNewTasks(bool bForceTaskChange)
{
	TBotTask iNewTask = EBotTaskInvalid;
	bool bForce = bForceTaskChange || (m_iCurrentTask == EBotTaskInvalid);

	const CWeapon* pWeapon =
		(m_bFeatureWeaponCheck && CWeapons::IsValid(m_iBestWeapon)) ? m_aWeapons[m_iBestWeapon].GetBaseWeapon() : NULL;
	TBotIntelligence iWeaponPreference = m_iIntelligence;

	bool bNeedHealth = CBotrixMod::HasMapItems(EItemTypeHealth) &&
		(m_PlayerInfo.GetHealth() < CBotrixParameterVars::PLAYER_MAX_HEALTH);
	bool bNeedHealthBad = bNeedHealth && (m_PlayerInfo.GetHealth() < (CBotrixParameterVars::PLAYER_MAX_HEALTH / 2.0f));
	bool bAlmostDead = bNeedHealthBad && (m_PlayerInfo.GetHealth() < (CBotrixParameterVars::PLAYER_MAX_HEALTH / 5.0f));
	bool bNeedWeapon = pWeapon && CBotrixMod::HasMapItems(EItemTypeWeapon);
	bool bNeedAmmo = pWeapon && CBotrixMod::HasMapItems(EItemTypeAmmo);

	TWeaponId iWeapon = EWeaponIdInvalid;
	bool bSecondary = false;

	const CItemClass* pEntityClass = NULL;  // Weapon or ammo class to search for.

	if ( bAlmostDead )
	{
		m_bDontAttack = (m_iIntelligence <= EBotNormal);
		m_bFlee = true;
	}

	int retries = 0;

restart_find_task:  // TODO: remove gotos.
	retries++;
	if ( retries == 5 )
	{
		iNewTask = EBotTaskFindEnemy;
		pEntityClass = NULL;
		goto find_enemy;
	}

	if ( bNeedHealthBad )  // Need health pretty much.
	{
		iNewTask = EBotTaskFindHealth;
		bForce = true;
	}
	else if ( bNeedWeapon && (pWeapon->iBotPreference < iWeaponPreference) )  // Need some weapon with higher
																			  // preference.
	{
		iNewTask = EBotTaskFindWeapon;
	}
	else if ( bNeedAmmo )
	{
		// Need ammunition.
		bool bNeedAmmo0 =
			(m_aWeapons[m_iBestWeapon].ExtraBullets(0) < pWeapon->iClipSize[0]);  // Has less than 1 extra clip.
		bool bNeedAmmo1 =
			m_aWeapons[m_iBestWeapon].HasSecondary() &&  // Has secondary function, but no secondary bullets.
			!m_aWeapons[m_iBestWeapon].HasAmmoInClip(1) && !m_aWeapons[m_iBestWeapon].HasAmmoExtra(1);

		if ( bNeedAmmo0 || bNeedAmmo1 )
		{
			iNewTask = EBotTaskFindAmmo;
			// Prefer search for secondary ammo only if has extra bullets for primary.
			bSecondary = bNeedAmmo1 && (m_aWeapons[m_iBestWeapon].ExtraBullets(0) > 0);
		}
		else if ( bNeedHealth )  // Need health (but has more than 50%).
		{
			iNewTask = EBotTaskFindHealth;
		}
		else if ( CBotrixMod::HasMapItems(EItemTypeArmor) &&
				  (m_PlayerInfo.GetArmorValue() < CBotrixParameterVars::PLAYER_MAX_ARMOUR) )  // Need armor.
		{
			iNewTask = EBotTaskFindArmor;
		}
		else if ( bNeedWeapon && (pWeapon->iBotPreference < EBotPro) )  // Check if can find a better weapon.
		{
			iNewTask = EBotTaskFindWeapon;
			iWeaponPreference = pWeapon->iBotPreference + 1;
		}
		else if ( !m_aWeapons[m_iBestWeapon].HasFullAmmo(1) )  // Check if weapon needs secondary ammo.
		{
			iNewTask = EBotTaskFindAmmo;
			bSecondary = true;
		}
		else if ( !m_aWeapons[m_iBestWeapon].HasFullAmmo(0) )  // Check if weapon needs primary ammo.
		{
			iNewTask = EBotTaskFindAmmo;
			bSecondary = false;
		}
		else
		{
			iNewTask = EBotTaskFindEnemy;
		}
	}
	else
	{
		iNewTask = EBotTaskFindEnemy;
	}

	switch ( iNewTask )
	{
		case EBotTaskFindWeapon:
		{
			pEntityClass = NULL;

			// Get weapon entity class to search for. Search for better weapons that actually have.
			for ( TBotIntelligence iPreference = iWeaponPreference; iPreference < EBotIntelligenceTotal; ++iPreference )
			{
				iWeapon = CWeapons::GetRandomWeapon(iPreference, m_cSkipWeapons);
				if ( iWeapon != EWeaponIdInvalid )
				{
					pEntityClass = CWeapons::Get(iWeapon)->GetBaseWeapon()->pWeaponClass;
					break;
				}
			}

			if ( pEntityClass == NULL )
			{
				// None found, search for worst weapons.
				for ( TBotIntelligence iPreference = iWeaponPreference - 1; iPreference >= 0; --iPreference )
				{
					iWeapon = CWeapons::GetRandomWeapon(iPreference, m_cSkipWeapons);
					if ( iWeapon != EWeaponIdInvalid )
					{
						pEntityClass = CWeapons::Get(iWeapon)->GetBaseWeapon()->pWeaponClass;
						break;
					}
				}
			}

			if ( pEntityClass == NULL )
			{
				bNeedWeapon = false;
				goto restart_find_task;
			}

			break;
		}

		case EBotTaskFindAmmo:
		{
			// Get ammo entity class to search for.
			iWeapon = m_iBestWeapon;

			// Randomly search for weapon instead, as it gives same primary bullets.
			if ( !bSecondary && bNeedWeapon && CItems::ExistsOnMap(pWeapon->pWeaponClass) && (rand() & 1) )
			{
				iNewTask = EBotTaskFindWeapon;
				pEntityClass = pWeapon->pWeaponClass;
			}
			else
			{
				int iAmmoEntitiesSize = pWeapon->aAmmos[bSecondary].size();
				pEntityClass = (iAmmoEntitiesSize > 0) ? pWeapon->aAmmos[bSecondary][rand() % iAmmoEntitiesSize] : NULL;
			}

			if ( !pEntityClass || !CItems::ExistsOnMap(pEntityClass) )  // There are no such weapon/ammo on the map.
			{
				iNewTask = EBotTaskFindEnemy;  // Just find enemy.
				pEntityClass = NULL;
			}

			break;
		}

		case EBotTaskFindHealth:
		case EBotTaskFindArmor:
		{
			pEntityClass = CItems::GetRandomItemClass(EItemTypeHealth + (iNewTask - EBotTaskFindHealth));
			break;
		}
	}

	BASSERT(iNewTask != EBotTaskInvalid, return);

find_enemy:
	// Check if need task switch.
	if ( bForce || (m_iCurrentTask != iNewTask) )
	{
		m_iCurrentTask = iNewTask;
		if ( pEntityClass )  // Health, armor, weapon, ammo.
		{
			TItemType iType = EItemTypeHealth + (iNewTask - EBotTaskFindHealth);
			TItemIndex iItemToSearch = CItems::GetNearestItem(iType, GetHead(), m_aPickedItems, pEntityClass);

			if ( iItemToSearch == -1 )
			{
				if ( (m_iCurrentTask == EBotTaskFindWeapon) && (iWeapon != EWeaponIdInvalid) )
				{
					m_cSkipWeapons.set(iWeapon);
				}
				m_iCurrentTask = EBotTaskInvalid;
				goto restart_find_task;
			}
			else
			{
				m_iTaskDestination = CItems::GetItems(iType)[iItemToSearch].iWaypoint;
				m_cItemToSearch.iType = iType;
				m_cItemToSearch.iIndex = iItemToSearch;
			}
		}
		else if ( m_iCurrentTask == EBotTaskFindEnemy )
		{
			// Just go to some random waypoint.
			m_iTaskDestination = -1;
			GoodAssert(CWaypoints::Size() >= 2);
			do
			{
				m_iTaskDestination = rand() % CWaypoints::Size();
			}
			while ( m_iTaskDestination == iCurrentWaypoint );
		}

		// Check if waypoint to go to is valid.
		if ( (m_iTaskDestination == EWaypointIdInvalid) || (m_iTaskDestination == iCurrentWaypoint) )
		{
			BotDebug(
				"%s -> Task %s, invalid destination waypoint %d (current %d), recalculate task.",
				GetName(),
				CTypeToString::BotTaskToString(m_iCurrentTask).c_str(),
				m_iTaskDestination,
				iCurrentWaypoint
			);
			m_iCurrentTask = -1;
			m_bNeedTaskCheck = true;  // Check new task in next frame.
			m_bNeedMove = m_bUseNavigatorToMove = m_bDestinationChanged = false;

			// Bot searched for item at current waypoint, but item was not there. Add it to array of picked items.
			TaskFinished();
		}
		else
		{
			BotDebug(
				"%s -> New task: %s %s, waypoint %d (current %d).",
				GetName(),
				CTypeToString::BotTaskToString(m_iCurrentTask).c_str(),
				pEntityClass ? pEntityClass->sClassName.c_str() : "",
				m_iTaskDestination,
				iCurrentWaypoint
			);

			m_iDestinationWaypoint = m_iTaskDestination;
			m_bNeedMove = m_bUseNavigatorToMove = m_bDestinationChanged = true;
		}
	}
	m_cSkipWeapons.reset();
}
#else  // BOTRIX_OLD_TASK_CODE
void CBot_HL2DM::CheckNewTasks(bool bForceTaskChange)
{
	TBotTask iNewTask = EBotTaskInvalid;
	bool bForce = bForceTaskChange || (m_iCurrentTask == EBotTaskInvalid);

	const CWeapon* pWeapon =
		(m_bFeatureWeaponCheck && CWeapons::IsValid(m_iBestWeapon)) ? m_aWeapons[m_iBestWeapon].GetBaseWeapon() : NULL;
	TBotIntelligence iWeaponPreference = m_iIntelligence;

	const bool bAlmostDead = CBotrixMod::HasMapItems(EItemTypeHealth) &&
		(m_PlayerInfo.GetHealth() < (CBotrixParameterVars::PLAYER_MAX_HEALTH / 5.0f));

	bool bNeedWeapon = pWeapon && CBotrixMod::HasMapItems(EItemTypeWeapon);

	TWeaponId iWeapon = EWeaponIdInvalid;
	bool bSecondaryWeapon = false;

	const CItemClass* pEntityClass = nullptr;  // Weapon or ammo class to search for.

	if ( bAlmostDead )
	{
		m_bDontAttack = (m_iIntelligence <= EBotNormal);
		m_bFlee = true;
	}

	size_t retries = 0;
	const size_t maxretries = 5;

	while ( retries < maxretries )
	{
		retries++;
		const bool outOfRetries = retries >= maxretries;

		if ( !outOfRetries )
		{
			iNewTask = ChooseNewTask(bForce, pWeapon, iWeaponPreference, bSecondaryWeapon);
		}
		else
		{
			iNewTask = EBotTaskFindEnemy;
			pEntityClass = nullptr;
		}

		if ( !outOfRetries )
		{
			switch ( iNewTask )
			{
				case EBotTaskFindWeapon:
				{
					pEntityClass = nullptr;

					// Get weapon entity class to search for. Search for better weapons that actually have.
					for ( TBotIntelligence iPreference = iWeaponPreference; iPreference < EBotIntelligenceTotal;
						  ++iPreference )
					{
						iWeapon = CWeapons::GetRandomWeapon(iPreference, m_cSkipWeapons);
						if ( iWeapon != EWeaponIdInvalid )
						{
							pEntityClass = CWeapons::Get(iWeapon)->GetBaseWeapon()->pWeaponClass;
							break;
						}
					}

					if ( !pEntityClass )
					{
						// None found, search for worst weapons.
						for ( TBotIntelligence iPreference = iWeaponPreference - 1; iPreference >= 0; --iPreference )
						{
							iWeapon = CWeapons::GetRandomWeapon(iPreference, m_cSkipWeapons);

							if ( iWeapon != EWeaponIdInvalid )
							{
								pEntityClass = CWeapons::Get(iWeapon)->GetBaseWeapon()->pWeaponClass;
								break;
							}
						}
					}

					if ( !pEntityClass )
					{
						bNeedWeapon = false;
						continue;
					}

					break;
				}

				case EBotTaskFindAmmo:
				{
					// Get ammo entity class to search for.
					iWeapon = m_iBestWeapon;

					// Randomly search for weapon instead, as it gives same primary bullets.
					if ( !bSecondaryWeapon && bNeedWeapon && CItems::ExistsOnMap(pWeapon->pWeaponClass) &&
						 (rand() & 1) )
					{
						iNewTask = EBotTaskFindWeapon;
						pEntityClass = pWeapon->pWeaponClass;
					}
					else
					{
						int iAmmoEntitiesSize = pWeapon->aAmmos[bSecondaryWeapon ? 1 : 0].size();
						pEntityClass = (iAmmoEntitiesSize > 0)
							? pWeapon->aAmmos[bSecondaryWeapon ? 1 : 0][rand() % iAmmoEntitiesSize]
							: nullptr;
					}

					if ( !pEntityClass ||
						 !CItems::ExistsOnMap(pEntityClass) )  // There are no such weapon/ammo on the map.
					{
						iNewTask = EBotTaskFindEnemy;  // Just find enemy.
						pEntityClass = NULL;
					}

					break;
				}

				case EBotTaskFindHealth:
				case EBotTaskFindArmor:
				{
					pEntityClass = CItems::GetRandomItemClass(EItemTypeHealth + (iNewTask - EBotTaskFindHealth));
					break;
				}
			}

			BASSERT(iNewTask != EBotTaskInvalid, return);
		}

		// Check if need task switch.
		if ( bForce || (m_iCurrentTask != iNewTask) )
		{
			m_iCurrentTask = iNewTask;

			if ( pEntityClass && !outOfRetries )  // Health, armor, weapon, ammo.
			{
				TItemType iType = EItemTypeHealth + (iNewTask - EBotTaskFindHealth);
				TItemIndex iItemToSearch = CItems::GetNearestItem(iType, GetHead(), m_aPickedItems, pEntityClass);

				if ( iItemToSearch == -1 )
				{
					if ( (m_iCurrentTask == EBotTaskFindWeapon) && (iWeapon != EWeaponIdInvalid) )
					{
						m_cSkipWeapons.set(iWeapon);
					}

					m_iCurrentTask = EBotTaskInvalid;

					// Retry
					continue;
				}
				else
				{
					m_iTaskDestination = CItems::GetItems(iType)[iItemToSearch].iWaypoint;
					m_cItemToSearch.iType = iType;
					m_cItemToSearch.iIndex = iItemToSearch;
				}
			}
			else if ( m_iCurrentTask == EBotTaskFindEnemy )
			{
				// Just go to some random waypoint.
				m_iTaskDestination = -1;
				GoodAssert(CWaypoints::Size() >= 2);

				do
				{
					m_iTaskDestination = rand() % CWaypoints::Size();
				}
				while ( m_iTaskDestination == iCurrentWaypoint );
			}

			// Check if waypoint to go to is valid.
			if ( (m_iTaskDestination == EWaypointIdInvalid) || (m_iTaskDestination == iCurrentWaypoint) )
			{
				BotDebug(
					"%s -> Task %s, invalid destination waypoint %d (current %d), recalculate task.",
					GetName(),
					CTypeToString::BotTaskToString(m_iCurrentTask).c_str(),
					m_iTaskDestination,
					iCurrentWaypoint
				);

				m_iCurrentTask = EBotTaskInvalid;
				m_bNeedTaskCheck = true;  // Check new task in next frame.
				m_bNeedMove = m_bUseNavigatorToMove = m_bDestinationChanged = false;

				// Bot searched for item at current waypoint, but item was not there. Add it to array of picked items.
				TaskFinished();
			}
			else
			{
				BotDebug(
					"%s -> New task: %s <%s>, waypoint %d (current %d).",
					GetName(),
					CTypeToString::BotTaskToString(m_iCurrentTask).c_str(),
					pEntityClass ? pEntityClass->sClassName.c_str() : "null",
					m_iTaskDestination,
					iCurrentWaypoint
				);

				m_iDestinationWaypoint = m_iTaskDestination;
				m_bNeedMove = m_bUseNavigatorToMove = m_bDestinationChanged = true;
			}
		}

		// If we got here, everything was OK.
		break;
	}

	m_cSkipWeapons.reset();
}
#endif  // BOTRIX_OLD_TASK_CODE

TBotTask CBot_HL2DM::ChooseNewTask(
	bool& bForce,
	const CWeapon*& pWeapon,
	TBotIntelligence& iWeaponPreference,
	bool& bSecondaryWeapon
)
{
	TBotTask iNewTask = EBotTaskInvalid;

	const bool bNeedHealth = CBotrixMod::HasMapItems(EItemTypeHealth) &&
		(m_PlayerInfo.GetHealth() < CBotrixParameterVars::PLAYER_MAX_HEALTH);

	const bool bNeedHealthBad =
		bNeedHealth && (m_PlayerInfo.GetHealth() < (CBotrixParameterVars::PLAYER_MAX_HEALTH / 2.0f));

	const bool bNeedWeapon = pWeapon && CBotrixMod::HasMapItems(EItemTypeWeapon);
	const bool bNeedAmmo = pWeapon && CBotrixMod::HasMapItems(EItemTypeAmmo);

	if ( bNeedHealthBad )  // Need health pretty much.
	{
		iNewTask = EBotTaskFindHealth;
		bForce = true;
	}
	else if ( bNeedWeapon && (pWeapon->iBotPreference < iWeaponPreference) )  // Need some weapon with higher
																			  // preference.
	{
		iNewTask = EBotTaskFindWeapon;
	}
	else if ( bNeedAmmo )
	{
		// Need ammunition.
		bool bNeedAmmo0 =
			(m_aWeapons[m_iBestWeapon].ExtraBullets(0) < pWeapon->iClipSize[0]);  // Has less than 1 extra clip.
		bool bNeedAmmo1 =
			m_aWeapons[m_iBestWeapon].HasSecondary() &&  // Has secondary function, but no secondary bullets.
			!m_aWeapons[m_iBestWeapon].HasAmmoInClip(1) && !m_aWeapons[m_iBestWeapon].HasAmmoExtra(1);

		if ( bNeedAmmo0 || bNeedAmmo1 )
		{
			iNewTask = EBotTaskFindAmmo;

			// Prefer search for secondary ammo only if has extra bullets for primary.
			bSecondaryWeapon = bNeedAmmo1 && (m_aWeapons[m_iBestWeapon].ExtraBullets(0) > 0);
		}
		else if ( bNeedHealth )  // Need health (but has more than 50%).
		{
			iNewTask = EBotTaskFindHealth;
		}
		else if ( CBotrixMod::HasMapItems(EItemTypeArmor) &&
				  (m_PlayerInfo.GetArmorValue() < CBotrixParameterVars::PLAYER_MAX_ARMOUR) )  // Need armor.
		{
			iNewTask = EBotTaskFindArmor;
		}
		else if ( bNeedWeapon && (pWeapon->iBotPreference < EBotPro) )  // Check if can find a better weapon.
		{
			iNewTask = EBotTaskFindWeapon;
			iWeaponPreference = pWeapon->iBotPreference + 1;
		}
		else if ( !m_aWeapons[m_iBestWeapon].HasFullAmmo(1) )  // Check if weapon needs secondary ammo.
		{
			iNewTask = EBotTaskFindAmmo;
			bSecondaryWeapon = true;
		}
		else if ( !m_aWeapons[m_iBestWeapon].HasFullAmmo(0) )  // Check if weapon needs primary ammo.
		{
			iNewTask = EBotTaskFindAmmo;
			bSecondaryWeapon = false;
		}
		else
		{
			iNewTask = EBotTaskFindEnemy;
		}
	}
	else
	{
		iNewTask = EBotTaskFindEnemy;
	}

	return iNewTask;
}

//----------------------------------------------------------------------------------------------------------------
void CBot_HL2DM::TaskFinished()
{
	if ( (EBotTaskFindHealth <= m_cItemToSearch.iType) && (m_cItemToSearch.iType <= EBotTaskFindAmmo) )
	{
		const CItem& cItem = CItems::GetItems(m_cItemToSearch.iType)[m_cItemToSearch.iIndex];

		// If item is not respawnable (or just bad configuration), force to not to search for it again right away, but
		// in 1 minute.
		m_cItemToSearch.fRemoveTime = CBotrixServerPlugin::GetTime();
		m_cItemToSearch.fRemoveTime +=
			FLAG_ALL_SET_OR_0(FEntityRespawnable, cItem.iFlags) ? cItem.pItemClass->GetArgument() : 60.0f;

		m_aPickedItems.push_back(m_cItemToSearch);
	}
	else  // Bot was heading to waypoint, not item.
	{
		// m_aWaypoints.set(m_cItemToSearch.iIndex);

		m_cItemToSearch.fRemoveTime =
			CBotrixServerPlugin::GetTime() + 60.0f;  // Do not go at that waypoint again at least for 1 minute.
		m_aPickedItems.push_back(m_cItemToSearch);
	}
}
