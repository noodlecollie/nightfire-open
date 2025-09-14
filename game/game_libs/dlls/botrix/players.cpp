#include <good/string_buffer.h>
#include <good/string_utils.h>
#include <good/vector.h>
#include <cstddef>

#include "botrix/players.h"
#include "botrix/server_plugin.h"
#include "botrix/defines.h"
#include "botrix/mod.h"
#include "botrix/clients.h"
#include "botrix/waypoint.h"
#include "botrix/bot.h"
#include "botrix/type2string.h"
#include "botrix/console_vars.h"
#include "PlatformLib/String.h"
#include "client.h"
#include "engine_util.h"

namespace Botrix
{
	bool CPlayers::bAddingBot = false;
	int CPlayers::iBotsPlayersCount = 0;
	float CPlayers::fPlayerBotRatio = 0.0f;

	good::vector<CPlayerPtr> CPlayers::m_aPlayers(16);

	int CPlayers::m_iClientsCount = 0;
	int CPlayers::m_iBotsCount = 0;
	bool CPlayers::m_bCheckBotCountFinished = true;

	good::string CPlayers::m_sLastError;

#if defined(DEBUG) || defined(_DEBUG)
	bool CPlayers::m_bClientDebuggingEvents = true;
#else
	bool CPlayers::m_bClientDebuggingEvents = false;
#endif

	//----------------------------------------------------------------------------------------------------------------
	CPlayer::CPlayer(edict_t* pEdict, bool bIsBot) :
		iCurrentWaypoint(EWaypointIdInvalid),
		iNextWaypoint(EWaypointIdInvalid),
		iPrevWaypoint(EWaypointIdInvalid),
		iChatMate(-1),
		m_pEdict(pEdict),
		m_iIndex(-1),
		m_PlayerInfo(m_pEdict),
		m_fEndProtectionTime(0),
		m_bBot(bIsBot),
		m_bAlive(false),
		m_bProtected(false)
	{
	}

	CBasePlayer* CPlayer::GetPlayerClassPtr() const
	{
		CBasePlayer* ptr = dynamic_cast<CBasePlayer*>(CBaseEntity::Instance(m_pEdict));
		GoodAssert(ptr);
		return ptr;
	}

	//----------------------------------------------------------------------------------------------------------------
	void CPlayer::Activated()
	{
		BLOG_T("Player %s activated.", GetName());

		m_iIndex = CPlayers::GetIndex(m_pEdict);

		m_sName.assign(GetName(), good::string::npos, true);
		good::lower_case(m_sName);
	}

	//----------------------------------------------------------------------------------------------------------------
	void CPlayer::Respawned()
	{
		BLOG_T("Player %s respawned.", GetName());

		m_fNextDrawHullTime = 0.0f;

		CBotrixEngineUtil::EntityHead(m_pEdict, m_vHead);
		iCurrentWaypoint = CWaypoints::GetNearestWaypoint(m_vHead);

		m_bAlive = true;
		m_bProtected = true;

		// A value less than 0 means that player is forever protected.
		if ( m_fEndProtectionTime >= 0 )
		{
			float defaultEndProtectionTime = CBotrixServerPlugin::GetTime() + CMod::fSpawnProtectionTime;
			// Player can be protected for longer time by console command.
			m_fEndProtectionTime = std::max<float>(m_fEndProtectionTime, defaultEndProtectionTime);
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	void CPlayer::PreThink()
	{
		// Don't update current waypoint for dead bots.
		if ( !m_bAlive && m_bBot )
		{
			return;
		}

		// CBasePlayer::IsDead() returns true only when player became dead, but when
		// player is respawnable (but still dead) it returns false.
		if ( m_PlayerInfo.IsDead() )
		{
			m_bAlive = false;
		}

		if ( m_bAlive )
		{
			if ( m_bProtected && m_fEndProtectionTime >= 0 && CBotrixServerPlugin::GetTime() >= m_fEndProtectionTime )
			{
				m_bProtected = false;
			}
		}

		if ( CBotrixCvars::botrix_draw_player_hull.value != 0.0f &&
			 CBotrixServerPlugin::GetTime() >= m_fNextDrawHullTime )
		{
			m_fNextDrawHullTime = CBotrixServerPlugin::GetTime() + 0.1f;
			Vector vPos = m_PlayerInfo.GetAbsOrigin();
			Vector vMins = m_PlayerInfo.GetPlayerMins();
			Vector vMaxs = m_PlayerInfo.GetPlayerMaxs();
			CBotrixEngineUtil::DrawBox(vPos, vMins, vMaxs, 0.1f, 0xFF, 0xFF, 0xFF);
		}

		m_vPrevHead = m_vHead;
		CBotrixEngineUtil::EntityHead(m_pEdict, m_vHead);

		// If waypoint is not valid or we are too far, recalculate current waypoint.
		float fDist = 0.0f;

		if ( CWaypoint::IsValid(iCurrentWaypoint) )
		{
			fDist = (CWaypoints::Get(iCurrentWaypoint).vOrigin - m_vHead).LengthSquared();
		}

		if ( !CWaypoint::IsValid(iCurrentWaypoint) || fDist > SQR(CWaypoint::MAX_RANGE) )
		{
			iPrevWaypoint = iCurrentWaypoint = CWaypoints::GetNearestWaypoint(m_vHead);
			return;
		}

		// Check if we are moved too far away from current waypoint as to be near to one of its neighbour.
		TWaypointId iNewWaypoint = EWaypointIdInvalid;

		CWaypoints::WaypointNode& w = CWaypoints::GetNode(iCurrentWaypoint);
		if ( CWaypoint::IsValid(iNextWaypoint) )  // There is next waypoint in path, check if we are closer to it.
		{
			CWaypoint& n = CWaypoints::Get(iNextWaypoint);
			float fNewDist = (n.vOrigin - m_vHead).LengthSquared();

			if ( fNewDist < fDist )
			{
				iNewWaypoint = iNextWaypoint;
			}
		}
		else
		{
			for ( int i = 0; i < w.neighbours.size(); ++i )
			{
				TWaypointId id = w.neighbours[i].target;
				CWaypoint& n = CWaypoints::Get(id);
				float fNewDist = (n.vOrigin - m_vHead).LengthSquared();

				if ( fNewDist < fDist )
				{
					iNewWaypoint = id;
					fDist = fNewDist;
				}
			}
		}

		if ( CWaypoint::IsValid(iNewWaypoint) )
		{
			iPrevWaypoint = iCurrentWaypoint;
			iCurrentWaypoint = iNewWaypoint;
		}
	}

	//********************************************************************************************************************
	void CPlayers::GetNames(StringVector& aNames, bool bGetBots, bool bGetUsers)
	{
		for ( good::vector<CPlayerPtr>::const_iterator it = m_aPlayers.begin(); it != m_aPlayers.end(); ++it )
		{
			if ( *it && ((bGetBots && (*it)->IsBot()) || (bGetUsers && !(*it)->IsBot())) )
			{
				aNames.push_back((*it)->GetName());
			}
		}
	}

	void CPlayers::CheckBotsCount()
	{
		if ( !CBotrixServerPlugin::MapIsRunning() || ((fPlayerBotRatio == 0.0f) && (iBotsPlayersCount == 0)) )
		{
			return;
		}

		int iNeededCount = fPlayerBotRatio > 0.0f
			? static_cast<int>(static_cast<float>(GetClientsCount()) * fPlayerBotRatio)
			: (iBotsPlayersCount - GetClientsCount());

		if ( iNeededCount < 0 )
		{
			iNeededCount = 0;
		}
		else if ( iNeededCount + GetClientsCount() > Size() - 1 )
		{
			iNeededCount = Size() - 1 - GetClientsCount();  // Save one space for player.
		}

		if ( iNeededCount == GetBotsCount() )
		{
			m_bCheckBotCountFinished = true;
		}
		else
		{
			m_bCheckBotCountFinished = false;

			if ( iNeededCount > GetBotsCount() )
			{
				AddBot(NULL, CBot::iDefaultTeam, CBot::iDefaultClass, -1);
			}
			else
			{
				KickRandomBot();
			}
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	void CPlayers::InvalidatePlayersWaypoints()
	{
		// Invalidate current / destination waypoints for all players.
		for ( int iPlayer = 0; iPlayer < Size(); ++iPlayer )
		{
			CPlayer* pPlayer = Get(iPlayer);

			if ( pPlayer )
			{
				if ( !pPlayer->IsBot() )  // TODO: virtual function for players to invalidate waypoints.
				{
					((CBotrixClient*)pPlayer)->iDestinationWaypoint = EWaypointIdInvalid;
				}

				// Force move failure, because path can contain removed waypoint.
				pPlayer->iCurrentWaypoint = pPlayer->iNextWaypoint = pPlayer->iPrevWaypoint = EWaypointIdInvalid;
			}
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	void CPlayers::Init(int iMaxPlayers)
	{
		m_aPlayers.clear();
		m_aPlayers.resize(iMaxPlayers);
		m_bCheckBotCountFinished = false;
	}

	//----------------------------------------------------------------------------------------------------------------
	void CPlayers::Clear()
	{
		for ( int i = 0; i < CPlayers::Size(); ++i )
		{
			CPlayer* pPlayer = m_aPlayers[i].get();

			if ( pPlayer )
			{
				if ( pPlayer->IsBot() )
				{
					KickBot(pPlayer);
				}

				m_aPlayers[i].reset();
			}
		}

		m_iClientsCount = m_iBotsCount = 0;
		m_bClientDebuggingEvents = false;
	}

	//----------------------------------------------------------------------------------------------------------------
	CPlayer*
	CPlayers::AddBot(const char* szName, TTeam iTeam, TBotIntelligence iIntelligence, int argc, const char** argv)
	{
		if ( !CBotrixServerPlugin::MapIsRunning() )
		{
			m_sLastError = "Error: no map is loaded.";
			return NULL;
		}

		if ( iIntelligence < 0 )
		{
			iIntelligence = (rand() % (CBot::iMaxIntelligence - CBot::iMinIntelligence + 1)) + CBot::iMinIntelligence;
		}

		if ( !szName || !szName[0] )
		{
			const good::string& sName = CMod::GetRandomBotName(iIntelligence);
			if ( CMod::bIntelligenceInBotName )
			{
				char nameBuffer[64];
				good::string_buffer sbNameWithIntelligence(nameBuffer, sizeof(nameBuffer), false);  // Don't deallocate.
				sbNameWithIntelligence = sName;
				sbNameWithIntelligence.append(' ');
				sbNameWithIntelligence.append('(');
				sbNameWithIntelligence.append(CTypeToString::IntelligenceToString(iIntelligence));
				sbNameWithIntelligence.append(')');
				szName = sbNameWithIntelligence.c_str();
			}
			else
			{
				szName = sName.c_str();
			}
		}

		bAddingBot = true;
		CPlayer* pBot = CMod::AddBot(szName, iIntelligence, iTeam, argc, argv);
		ClientPutInServer(pBot->GetEdict());
		bAddingBot = false;

		if ( pBot )
		{
			TPlayerIndex iIndex = ENTINDEX(pBot->GetEdict()) - 1;
			GoodAssert(iIndex >= 0);
			m_aPlayers[iIndex] = CPlayerPtr(pBot);
		}
		else
		{
			m_sLastError = "CMod::AddBot() failed";
		}

		return pBot;
	}

	//----------------------------------------------------------------------------------------------------------------
	void CPlayers::KickBot(CPlayer* pPlayer)
	{
		BASSERT(pPlayer && pPlayer->IsBot());
		char szCommand[16];
		PlatformLib_SNPrintF(szCommand, sizeof(szCommand), "kick #%d\n", pPlayer->GetPlayerInfo()->GetUserID());
		SERVER_COMMAND(szCommand);
	}

	//----------------------------------------------------------------------------------------------------------------
	bool CPlayers::KickRandomBot()
	{
		return KickRandomBotOnTeam(-1);
	}

	//----------------------------------------------------------------------------------------------------------------
	bool CPlayers::KickRandomBotOnTeam(int iTeam)
	{
		if ( m_iBotsCount == 0 )
		{
			return false;
		}

		int index = rand() % m_aPlayers.size();

		// Search for used bot from index to left.
		CBot* toKick = NULL;
		int i;

		// TODO: not use 2 for.
		for ( i = index; i >= 0; --i )
		{
			CPlayer* pPlayer = m_aPlayers[i].get();
			if ( pPlayer && pPlayer->IsBot() && ((iTeam == -1) || (pPlayer->GetTeam() == iTeam)) )
			{
				toKick = (CBot*)pPlayer;
				break;
			}
		}

		// Search for used bot from index to right.
		if ( toKick == NULL )
			for ( i = index + 1; i < (int)m_aPlayers.size(); ++i )
			{
				CPlayer* pPlayer = m_aPlayers[i].get();
				if ( pPlayer && pPlayer->IsBot() && ((iTeam == -1) || (pPlayer->GetTeam() == iTeam)) )
				{
					toKick = (CBot*)pPlayer;
					break;
				}
			}

		if ( toKick )
		{
			KickBot(toKick);
		}

		return (toKick != NULL);
	}

	//----------------------------------------------------------------------------------------------------------------
	void CPlayers::PlayerConnected(edict_t* pEdict)
	{
		// IPlayerInfo* pPlayerInfo = CBotrixPlugin::pPlayerInfoManager->GetPlayerInfo(pEdict);
		// if ( pPlayerInfo && pPlayerInfo->IsConnected() ) // IsPlayer() IsFakeClient() not working
		if ( bAddingBot )
		{
			m_iBotsCount++;
		}
		else
		{
			TPlayerIndex iIdx = ENTINDEX(pEdict) - 1;
			GoodAssert(iIdx >= 0);  // Valve should not allow this assert.

			BASSERT(m_aPlayers[iIdx].get() == NULL, return);  // Should not happend.

			CBotrixClient* pPlayer = new CBotrixClient(pEdict);

			if ( !IS_DEDICATED_SERVER() && CBotrixServerPlugin::GetListenServerClient() == pEdict )
			{
				// Give listenserver client all access to bot commands.
				pPlayer->iCommandAccessFlags = FCommandAccessAll;
			}

			m_aPlayers[iIdx] = CPlayerPtr(pPlayer);
			m_iClientsCount++;

			CPlayers::CheckBotsCount();
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	void CPlayers::PlayerDisconnected(edict_t* pEdict)
	{
		int iIdx = ENTINDEX(pEdict) - 1;
		GoodAssert(iIdx >= 0);  // Valve should not allow this assert.

		if ( !m_aPlayers[iIdx].get() )
		{
			return;  // Happens when starting new map and pressing cancel button. Valve issue.
		}

		CPlayer* pPlayer = m_aPlayers[iIdx].get();
		BASSERT(pPlayer, return);

		// Notify bots that player is disconnected.
		for ( int i = 0; i < Size(); ++i )
		{
			CPlayer* pBot = m_aPlayers[i].get();
			if ( pBot && pBot->IsBot() && pBot->IsAlive() )
			{
				((CBot*)pBot)->PlayerDisconnect(iIdx, pPlayer);
			}
		}

		bool bIsBot = pPlayer->IsBot();
		m_aPlayers[iIdx].reset();

		if ( bIsBot )
		{
			m_iBotsCount--;
		}
		else
		{
			m_iClientsCount--;
			CheckForDebugging();
			CPlayers::CheckBotsCount();
		}

		GoodAssert((m_iBotsCount >= 0) && (m_iClientsCount >= 0));
	}

	//----------------------------------------------------------------------------------------------------------------
	void CPlayers::PreThink()
	{
		if ( !m_bCheckBotCountFinished )
		{
			CheckBotsCount();
		}

		for ( good::vector<CPlayerPtr>::iterator it = m_aPlayers.begin(); it != m_aPlayers.end(); ++it )
		{
			if ( it->get() )
			{
				it->get()->PreThink();
			}
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	void CPlayers::DebugEvent(const char* szFormat, ...)
	{
		static char buffer[128];
		va_list args;
		va_start(args, szFormat);
		PlatformLib_VSNPrintF(buffer, sizeof(buffer), szFormat, args);
		va_end(args);

		for ( good::vector<CPlayerPtr>::const_iterator it = m_aPlayers.begin(); it != m_aPlayers.end(); ++it )
		{
			const CPlayer* pPlayer = it->get();
			if ( pPlayer && !pPlayer->IsBot() && ((CBotrixClient*)pPlayer)->bDebuggingEvents )
			{
				BULOG_I(pPlayer->GetEdict(), "%s", buffer);
			}
		}
	}

	//----------------------------------------------------------------------------------------------------------------
	void CPlayers::CheckForDebugging()
	{
		m_bClientDebuggingEvents = false;
		for ( good::vector<CPlayerPtr>::iterator it = m_aPlayers.begin(); it != m_aPlayers.end(); ++it )
		{
			const CPlayer* pPlayer = it->get();
			if ( pPlayer && !pPlayer->IsBot() && ((CBotrixClient*)pPlayer)->bDebuggingEvents )
			{
				m_bClientDebuggingEvents = true;
				break;
			}
		}
	}
}  // namespace Botrix
