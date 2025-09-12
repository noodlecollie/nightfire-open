#pragma once

#include "botrix/types.h"
#include "botrix/defines.h"
#include "botrix/players.h"
#include "botrix/botrixmod.h"
#include "botrix/waypoint.h"
#include "botrix/item.h"
#include "botrix/weapon.h"
#include "botrix/movecmd.h"
#include "botrix/waypoint_navigator.h"
#include "botrix/parameter_vars.h"

struct edict_s;

#define BotMessage(...) \
	GOOD_SCOPE_START if ( m_bDebugging ) BLOG_I(__VA_ARGS__); \
	GOOD_SCOPE_END
#define BotDebug(...) \
	GOOD_SCOPE_START if ( m_bDebugging ) BLOG_D(__VA_ARGS__); \
	GOOD_SCOPE_END
#define BotTrace(...) \
	GOOD_SCOPE_START if ( m_bDebugging ) BLOG_T(__VA_ARGS__); \
	GOOD_SCOPE_END

class CBotChat;  // Forward declaration.

//****************************************************************************************************************
/// Abstract class representing a bot.
/** This class allows to bot ability to move and use waypoints, but not to shoot. Shooting and setting
 * objectives/waypoint-destination is a child class responsability (which should derive from CBotrixBot class). */
//****************************************************************************************************************
class CBotrixBot : public CPlayer
{
public:  // Members.
	static bool bAssumeUnknownWeaponManual;  ///< Assume that unknown weapon are melees.
	static TBotIntelligence iMinIntelligence;  ///< Minimum default bot intelligence.
	static TBotIntelligence iMaxIntelligence;  ///< Maximum default bot intelligence.
	static TTeam iDefaultTeam;  ///< Default team when adding bots without specify team.
	static TClass iDefaultClass;  ///< Default class when adding bots without specify class.
	static int iChangeClassRound;  ///< For bot to change class every x rounds.
	static TFightStrategyFlags iDefaultFightStrategy;  ///< Flags of default fighting strategy.

	static float fNearDistanceSqr;  ///< Distance to consider to be near enemy.
	static float fFarDistanceSqr;  ///< Distance to consider to be far away from enemy.
	static float fInvalidWaypointSuicideTime;  ///< Max time to stay away from waypoints.

public:  // Methods.
	/// Constructor.
	CBotrixBot(struct edict_s* pEdict, TBotIntelligence iIntelligence, TClass iClass);

	/// Destructor.
	virtual ~CBotrixBot()
	{
	}

	/// Get bot's intelligence.
	TBotIntelligence GetIntelligence()
	{
		return m_iIntelligence;
	}

	/// Return true if @p iPlayer is ally for bot.
	bool IsAlly(TPlayerIndex iPlayer)
	{
		return m_aAllies.test(iPlayer);
	}

	/// Return true if bot is paused.
	bool IsAttacking()
	{
		return m_bCommandAttack;
	}

	/// Return true if bot is printing debug messages.
	bool IsDebugging()
	{
		return m_bDebugging;
	}

	/// Return true if bot is paused.
	bool IsPaused()
	{
		return m_bCommandPaused;
	}

	/// Return true if bot is paused.
	bool IsStopped()
	{
		return m_bCommandStopped;
	}

	/// Set ally/enemy for bot.
	void SetAlly(TPlayerIndex iPlayer, bool bAlly)
	{
		m_aAllies.set(iPlayer, bAlly);
		if ( bAlly && (m_pCurrentEnemy == CPlayers::Get(iPlayer)) )
		{
			EraseCurrentEnemy();
		}
	}

	/// Start/stop bot attack.
	void SetAttack(bool bAttack)
	{
		m_bCommandAttack = bAttack;
	}

	/// Show or hide bot messages.
	void SetDebugging(bool bOn)
	{
		m_bDebugging = bOn;
	}

	/// Pause/resume bot.
	void SetPaused(bool bPause)
	{
		m_bCommandPaused = bPause;
	}

	/// Prevent bot from moving.
	void SetStopped(bool bStop)
	{
		m_bCommandStopped = bStop;
	}

	/// Emulate console command for bot.
	void ConsoleCommand(const char* szCommand);

	/// Use say command.
	void Say(bool bTeamOnly, const char* szFormat, ...);

	/// Bot is created for testing path between two waypoints. Will be kicked if fails, killed, or reaches destination.
	virtual void TestWaypoints(TWaypointId iFrom, TWaypointId iTo);

	/// Add weapon to bot.
	virtual void AddWeapon(TWeaponId iWeaponId);

	/// Remove all weapons from bot.
	virtual void WeaponsRemove()
	{
		GetPlayerClassPtr()->RemoveAllItems(false);
		m_aWeapons.clear();
		m_iWeapon = m_iBestWeapon = m_iMeleeWeapon = m_iPhyscannon = EWeaponIdInvalid;
	}

	//------------------------------------------------------------------------------------------------------------
	// Next functions are mod dependent. You need to implement those in order to make bot moving around.
	//------------------------------------------------------------------------------------------------------------
	/// Called when player becomes active, before first respawn. Sets players model and team.
	virtual void Activated();

	/// Respawned on map. Reset all variables, waypoint navigator.
	virtual void Respawned();

	/// Called when player's team changed.
	virtual void ChangeTeam(TTeam iTeam) = 0;

	/// Called after respawn to configure weapons.
	virtual void ConfigureRespawnWeapons();

	/// Called when player becomes dead. Will kick it if bot was created for testing purposes.
	virtual void Dead();

	/// Called when player got disconnected / kicked.
	virtual void PlayerDisconnect(int iPlayerIndex, CPlayer* pPlayer);

	/// Called when this bot just killed an enemy.
	virtual void KilledEnemy(int /*iPlayerIndex*/, CPlayer* pPlayer)
	{
		if ( pPlayer == this )
			BotMessage("%s -> Suicide.", GetName());
		else
			BotDebug("%s -> Killed %s.", GetName(), pPlayer->GetName());
		if ( pPlayer == m_pCurrentEnemy )
			EraseCurrentEnemy();
	}

	/// Called when enemy just shot this bot.
	virtual void HurtBy(int iPlayerIndex, CPlayer* pPlayer, int iHealthNow) = 0;

private:
	// Called every frame to evaluate next move. Note that this method is private, use Move() method in subclasses.
	virtual void PreThink();

protected:  // Mod dependend protected functions.
	//------------------------------------------------------------------------------------------------------------
	// Next functions are mod dependent.
	//------------------------------------------------------------------------------------------------------------
	// Called each frame. Set move and look variables. You can also set shooting/crouching/jumping buttons in
	// m_cCmd.buttons.
	virtual void Think() = 0;

	// This function get's called when next waypoint in path becomes closer than current one. By default sets new
	// look forward to next waypoint in path (after current one).
	virtual void CurrentWaypointJustChanged();

	// Check waypoint flags and perform waypoint action. Called when arrives to next waypoint. By default this
	// will check if waypoint has health/armor machine flags and if bot needs to use it. If so, then this function
	// returns true to not to call ApplyPathFlags() / DoPathAction(), while performing USE action (for machine),
	// and ApplyPathFlags() / DoPathAction() will be called after using machine.
	virtual bool DoWaypointAction();

	// Check waypoint path flags and set bot flags accordingly. Called when arrives to next waypoint.
	// Note that you need to set action variables in DoPathAction(). This function is just to
	// figure out if need to crouch/sprint/etc and set new aim/destination to next waypoint in path.
	virtual void ApplyPathFlags();

	// Called when started to touch waypoint. You can use iCurrentWaypoint and iNextWaypoint to get path flags.
	// Will set all needed action variables to jump / jump with duck / break.
	virtual void DoPathAction();

	// Return true if given player is enemy. Really it is mod's dependant.
	inline bool IsEnemy(CPlayer* pPlayer) const
	{
		int team = pPlayer->GetTeam();
		int index = pPlayer->GetIndex();
		return (!pPlayer->IsProtected()) &&
			(CBotrixMod::iSpawnProtectionHealth == 0 || pPlayer->GetHealth() < CBotrixMod::iSpawnProtectionHealth) &&
			(team != CBotrixMod::iSpectatorTeam) &&
			(team != GetTeam() || team == CBotrixMod::iUnassignedTeam) &&  // Different teams or deathmatch.
			(index >= 0) && !m_aAllies.test(index);
	}

	// Enemy is dead or got disconnected.
	virtual void EraseCurrentEnemy()
	{
		CurrentEnemyNotVisible();
	}

	// Bot just picked up given item.
	virtual void PickItem(const CItem& cItem, TItemType iEntityType, TItemIndex iIndex);

protected:  // Methods.
#ifdef BOTRIX_SEND_BOT_CHAT
			/// Say something in chat.
	void Speak(bool bTeamSay);
#endif  // BOTRIX_SEND_BOT_CHAT

	// Return true if entity is inside bot's view cone.
	bool IsVisible(CPlayer* pPlayer, bool bViewCone) const;

	// Move failure is produced when current waypoint is invalid or when using navigator,
	// there is no path from current to next waypoint.
	bool CheckMoveFailure()
	{
		if ( !m_bMoveFailure )
		{
			m_bMoveFailure =  // TODO: check here waypoint invalid: !CWaypoint::IsValid(iCurrentWaypoint) ||
				(m_bNeedMove && m_bUseNavigatorToMove && m_cNavigator.SearchEnded() && !m_bDestinationChanged &&
				 (!CWaypoint::IsValid(iNextWaypoint) ||
				  ((iCurrentWaypoint != iNextWaypoint) && !CWaypoints::HasPath(iCurrentWaypoint, iNextWaypoint))));
		}
		return m_bMoveFailure;
	}

	// Move between current and next waypoints. Return true if next waypoint is reached.
	bool MoveBetweenWaypoints();

	// Get time to end aiming to sinchronize angles. Depends on how much 'mouse' distance there are and bot's
	// intelligence.
	float GetEndLookTime();

	// Check current's bot weapon.
	void WeaponCheckCurrent(bool bAddToBotWeapons);

	// Scan bot's weapon.
	void WeaponsScan();

	// Update bot's weapon.
	void UpdateWeapon();

	// Update nearest objects, players, items and weapons.
	void UpdateWorld();

	inline void CurrentEnemyNotVisible()
	{
		m_bEnemyAim = m_bEnemyOffSight = m_bAttackDuck = false;
		m_pCurrentEnemy = NULL;
	}
	// Check if this enemy can be seen / should be attacked.
	void CheckEnemy(int iPlayerIndex, CPlayer* pPlayer, bool bCheckVisibility);

	// Check if need to duck to attack.
	void CheckAttackDuck(CPlayer* pPlayer);

	// Try to follow enemy.
	bool FollowEnemy(CPlayer* pEnemy)
	{
		if ( !pEnemy->IsAlive() || !CWaypoint::IsValid(pEnemy->iCurrentWaypoint) )
		{
			return false;
		}

		BotDebug("%s -> Follow enemy %s.", GetName(), pEnemy->GetName());
		m_iDestinationWaypoint = pEnemy->iCurrentWaypoint;
		m_bUseNavigatorToMove = m_bNeedMove = m_bDestinationChanged = true;
		return true;
	}

	// Aim at enemy.
	void EnemyAim();

	// Get current weapon id.
	TWeaponId WeaponSearch(const char* szWeapon)
	{
		for ( TWeaponId i = 0; i < m_aWeapons.size(); ++i )
		{
			if ( m_aWeapons[i].GetName() == szWeapon )
			{
				return i;
			}
		}

		return EWeaponIdInvalid;
	}

	// Choose best attack weapon from available weapons.
	void WeaponChoose();

	// Set active weapon. Mod dependent. Instant, doensn't take in account parameters from config.ini
	virtual bool WeaponSet(const good::string& sWeapon);

	// Try to swith to other weapon.
	void ChangeWeapon(TWeaponId iIndex);

	// Shoot current weapon.
	void WeaponShoot(int iSecondary = CWeapon::PRIMARY);

	// Toggle zoom.
	void WeaponToggleZoom()
	{
		CWeaponWithAmmo& cWeapon = m_aWeapons[m_iWeapon];
		BASSERT(cWeapon.IsSniper() && cWeapon.CanUse(), return);
		cWeapon.ToggleZoom();
		BotDebug(
			"%s -> Zoom %s: %s.",
			GetName(),
			m_aWeapons[m_iWeapon].GetName().c_str(),
			cWeapon.IsUsingZoom() ? "true" : "false"
		);
		FLAG_SET(IN_ATTACK2, m_cCmd.buttons);
	}

	// Reload current weapon.
	void WeaponReload()
	{
		BotDebug("%s -> Reload %s.", GetName(), m_aWeapons[m_iWeapon].GetName().c_str());
		m_aWeapons[m_iWeapon].Reload(0);
		FLAG_SET(IN_RELOAD, m_cCmd.buttons);
	}

	// Check if bot needs to change side look.
	void CheckSideLook(bool bIsMoving, bool bForceCheck);

	// While stucked, return true if move destination have been calculated, i.e. bot can continue moving to destination,
	// false if bot needs to restart thinking (in order to perform 'touch' on stucked waypoint for example).
	bool ResolveStuckMove();

	// Setup path search / continues searching / returns true if search finishes or bot arrived to next waypoint in
	// path.
	bool NavigatorMove();

	// If not using navigator, returns true if bot arrived to m_vDestination.
	bool NormalMove();

	// If m_bNeedMove then moves toward m_vDestination. If m_bNeedAim then smoothly aims to m_vLook until m_fEndAimTime.
	void PerformMove(TWaypointId iPreviousWaypoint, const Vector& vPrevOrigin);

protected:  // Members.
	static float m_fTimeIntervalCheckUsingMachines;  // After this interval will check if health/armor is incrementing
													 // (when using health/armor machine).
	static int m_iCheckEntitiesPerFrame;  // How much near items check per frame (to know if bot is stucked with objects
										  // or picked up item).

	BotrixMoveCmd m_cCmd;  // Bot's command (virtual keyboard, used to move bot around and fire weapons).

	TBotIntelligence m_iIntelligence;  // Bot's intelligence.
	TClass m_iClass;  // Bot's class.
	int m_iClassChange;  // Will change it's class when reach 0.

	float m_fPrevThinkTime;  // Previous think time (used to get time difference between this and previous frame).

	Vector m_vDestination;  // Vector, where bot tries to move to.
	// Vector m_vLastVelocity;                                       // Vector of velocity in previous frame.
	Vector m_vLook;  // Point where bot tries to aim to.

	Vector m_vForward;  // Bot will look at this forward vector most time it is moving.
						// It is set to next waypoint origin in path when bot becomes closer to destination waypoint.
	TWaypointId
		m_iDestinationWaypoint;  // Set this to waypoint you want to move to, also m_bDestinationChanged to true.
	TWaypointId m_iAfterNextWaypoint;  // This is a next waypoint in path after iNextWaypoint. Used to look at, when
									   // iNextWaypoint is close.

	float m_fEndAimTime;  // Time bot should end "moving mouse" to aim at m_vLook.
	float m_fStartActionTime;  // Time when bot jumps / breaks / uses (check m_bNeedJump, m_bNeedAttack, m_bNeedUse).
	float m_fEndActionTime;  // Time to release duck button, which was pressed when bot jumps.

	float m_fStuckCheckTime;  // Time when bot got stucked.
	Vector m_vStuckCheck;  // Position of player at m_fStuckCheckTime.
	Vector m_vDisturbingObjectPosition;  // Position of disturbing object in previous frame.

	int m_iLastHealthArmor;  // Amount of health/armor bot had at m_fStartActionTime.

	unsigned char r, g, b;  // Bot's path color.

	CWaypointNavigator m_cNavigator;  // Waypoint navigator.
	good::vector<TAreaId> m_aAvoidAreas;  // Array of areas waypoint navigator must avoid.

	// Nearest items from m_aNearItems that are checked every frame (to know if bot picked them up).
	good::vector<TItemIndex> m_aNearestItems[EItemTypeCollisionTotal];
	// Items in close range.
	good::vector<TItemIndex> m_aNearItems[EItemTypeCollisionTotal];
	int m_iNextNearItem[EItemTypeCollisionTotal];  // Next item to check if close (index in array CItems::GetItems()).
	const CItem* pStuckObject;  // Object that player is stucked with.

	good::bitset m_aNearPlayers;  // Bitset of players near (to know if bot can stuck with them).
	good::bitset m_aSeenEnemies;  // Bitset of enemies that bot can see right now.
	good::bitset m_aEnemies;  // Bitset of enemies that bot can't see right now, but it knows they are there.
	good::bitset m_aAllies;  // Allies for bot.
	int m_iNextCheckPlayer;  // Next player to check if close.

	CPlayer* m_pCurrentEnemy;  // Current enemy.
	float m_fDistanceSqrToEnemy;  // If m_pCurrentEnemy is not NULL, squared distance to it.
	float m_fTimeToEraseEnemy;  // Time to mark enemy no visible after not seeing it.

	good::vector<CPickedItem>::size_type m_iCurrentPickedItem;  // Index in m_aPickedItems to check next frame.
	good::vector<CPickedItem> m_aPickedItems;  // Picked items (like health or weapon), for bot to know which items are
											   // available to pick on map.

	good::vector<CWeaponWithAmmo> m_aWeapons;  // Weapons that bot actually has.
	TWeaponId m_iWeapon, m_iBestWeapon;  // Current / previous / best weapon.
	TWeaponId m_iPhyscannon, m_iMeleeWeapon;  // Index of gravity gun / melee gun, -1 if bot doesn't have it.

	float m_fNextDrawNearObjectsTime;  // Next time to draw near objects.
	float m_fInvalidWaypointEnd;  // Time when suicide because far away from waypoints.

	good::pair<int, int> m_cAttackDuckRangeSqr;  // Will duck if attacking & m_bFeatureAttackDuckEnabled & in range.

protected:  // Bot flags.
	bool m_bTest;  // Bot was created only for testing purposes, it will be eliminated after reaching needed
				   // waypoint.
	bool m_bCommandAttack;  // Bot can't attack by console command.
	bool m_bCommandPaused;  // Bot is paused by console command.
	bool m_bCommandStopped;  // Bot is stopped (can't move) by console command.
	bool m_bDebugging;  // Currently debugging this bot.
	bool m_bInvalidWaypointStart;  // Start counting time to suicide.

	bool m_bAimChanged;  // True if need to change bot's angles (if false m_angLook has been calculated).
	bool m_bNeedAim;  // True if need to change bot's angles. Will be set to false when finished aiming.
	bool m_bUseSideLook;  // If true then look at random: forward, left, right or back.
	bool m_bPathAim;  // Used when preparing to jump, use, sprint, etc.
	bool m_bEnemyAim;  // Currently aim at enemy. Has more priority than m_bPathAim.
	bool m_bLockAim;  // Don't change bot's angles while aim is locked. To be used by mod, more priority than
					  // m_bEnemyAim.

	bool m_bDestinationChanged;  // Set this to true when you change destination to go to.
	bool m_bNeedMove;  // True if need to move. Will be set to false if reached m_vDestination.
	bool m_bLastNeedMove;  // m_bNeedMove value of previous frame.
	bool m_bLockMove;  // Force not to move.

	bool m_bUseNavigatorToMove;  // If true then use waypoint navigator to get to m_vDestination, else just move
								 // there in line.
	bool m_bLockNavigatorMove;  // Don't move while this boolean is true. Used when preparing to jump/sprint/etc to
								// next waypoint.
	bool m_bMoveFailure;  // Set to true when bot can't reach m_vDestination through navigator. Make sure it is
						  // false.

	bool m_bStuck;  // true, if m_bNeedMove is set, but couldn't move for 1 second.
	bool m_bResolvingStuck;  // true, if stucked, but performing some action to unstuck.
	bool m_bNeedCheckStuck;  // If true then check if stucked at m_fStuckCheckTime.

	bool m_bStuckBreakObject;  // If true then will try break m_aNearestItems.
	bool m_bStuckUsePhyscannon;  // If true then will try move m_aNearestItems with gravity gun.
	bool m_bStuckPhyscannonEnd;  // Becomes true when bot is holding object and aimed back/at enemy.
	bool m_bStuckTryingSide;  // If true then bot is stucked, and going left or right for half second according to
							  // m_bStuckTryGoLeft.
	bool m_bStuckTryGoLeft;  // If true go left when stucked, else go right.
	bool m_bStuckGotoCurrent;  // When stucked will try go to current waypoint (for 'touching' and so performing
							   // action). If false, then will try to do left or right move.
	bool m_bStuckGotoPrevious;  //
	bool m_bRepeatWaypointAction;  // Set when stucked, and repeats go to current waypoint and touch it.

	bool m_bLadderMove;  // Will be set to true, when current waypoint path has ladder flag.

	bool m_bNeedStop;  // Need to stop when reach next waypoint.
	bool m_bNeedDuck;  // Will crouch until reaches next waypoint or until action time end (if used with jump).
	bool m_bNeedWalk;  // Will walk on straight line.
	bool m_bNeedSprint;  // Will sprint until reaches next waypoint. Used for long jumps.

	bool m_bNeedReload;  // Need to reload some weapons.

	bool m_bNeedFlashlight;  // Need to use flashlight.
	bool m_bUsingFlashlight;  // True if currently using flashlight.

	bool m_bNeedUse;  // Will starts using USE button at m_fStartActionTime until m_fEndActionTime.
	bool m_bAlreadyUsed;  // This var will be set when bot ends performing USE action. This will allow bot to not to
						  // do USE action again.
	bool m_bUsingHealthMachine;  // if m_bNeedUse and this variable is true then we are at health charger.
	bool m_bUsingArmorMachine;  // if m_bNeedUse and this variable is true then we are at armor charger.
	bool m_bUsingButton;  // if m_bNeedUse and this variable is true then we are at some button.

	bool m_bNeedAttack;  // Will press attack button at m_fStartActionTime until m_fEndActionTime.
	bool m_bNeedAttack2;  // Will press attack button 2 at m_fStartActionTime until m_fEndActionTime.
	bool m_bNeedJump;  // Will jump once at m_fStartActionTime.
	bool m_bNeedJumpDuck;  // Will start ducking at m_fStartActionTime and hold duck until m_fEndActionTime.

	bool m_bDontBreakObjects;  // Set to true to not to break nearby objects.
	bool m_bDontThrowObjects;  // Set to true to not to throw nearby objects.
	bool m_bDontAttack;  // Set to true to ignore enemy (for example if defusing the bomb).

	bool m_bFlee;  // True, if currenly running away from enemy.
	bool m_bAttackDuck;  // True if need to duck, while attacking enemy.
	bool m_bNeedSetWeapon;  // Need to set best weapon.
	bool m_bEnemyAimed;  // If true, then enemy is already aimed.
	bool m_bEnemyOffSight;  // Enemy is not visible.
	bool m_bStayReloading;  // If true, then don't change weapon at reload time, but reload current weapon instead.
	bool m_bShootAtHead;  // If true, then will shoot at head instead of body.

	// Features.
	bool m_bFeatureAttackDuckEnabled;  // Duck while attacking. Will duck if in attack duck range.
	bool m_bFeatureWeaponCheck;  // Check or not weapons.

	bool m_bSaidNoWaypoints;  // Say that there are no waypoints only once.
};

//****************************************************************************************************************
/// Class representing a bot for Half Life 2 Deathmatch.
// sv_hl2mp_weapon_respawn_time sv_hl2mp_item_respawn_time
//****************************************************************************************************************
class CBot_HL2DM : public CBotrixBot
{
public:
	/// Constructor.
	CBot_HL2DM(edict_t* pEdict, TBotIntelligence iIntelligence);

	/// Looks like change model is the only way to change the team in HL2DM.
	void ChangeModel(TTeam iTeam);

	//------------------------------------------------------------------------------------------------------------
	// Next functions are mod dependent.
	//------------------------------------------------------------------------------------------------------------
	/// Called when player becomes active, before first respawn. Sets players model and team.
	virtual void Activated();

	/// Called each time bot is respawned.
	virtual void Respawned();

	/// Called when player's team changed.
	virtual void ChangeTeam(TTeam /*iTeam*/)
	{
	}

	// Enemy is dead or got disconnected.
	virtual void EraseCurrentEnemy()
	{
		CBotrixBot::EraseCurrentEnemy();
		m_bChasing = false;
		m_pChasedEnemy = NULL;
		m_iCurrentTask = EBotTaskInvalid;
		m_bNeedTaskCheck = true;
	}

	/// Called when enemy just shot this bot.
	virtual void HurtBy(int iPlayerIndex, CPlayer* pAttacker, int iHealthNow);

	/// Set move and aim variables. You can also set ShootWeaponing/crouching/jumping buttons in m_cCmd.buttons.
	virtual void Think();

	/// Called when chat arrives from other player.
	virtual void ReceiveChat(int iPlayerIndex, CPlayer* pPlayer, bool bTeamOnly, const char* szText);

protected:
	/// Inherited from CBot. Will check if arrived at m_iTaskDestination and invalidates current task.
	virtual bool DoWaypointAction();

	/// Bot just picked up given item.
	virtual void PickItem(const CItem& cItem, TItemType iEntityType, TItemIndex iIndex)
	{
		CBotrixBot::PickItem(cItem, iEntityType, iIndex);
		if ( m_bFlee && (iEntityType == EItemTypeHealth) )
		{
			m_bFlee = (m_PlayerInfo.GetHealth() < (CBotrixParameterVars::PLAYER_MAX_HEALTH / 8.0f));
			m_bDontAttack = m_bFlee;
		}
	}

	/// Chase enemy.
	void ChaseEnemy()
	{
		m_bChasing = FollowEnemy(m_pChasedEnemy);

		if ( m_bChasing )
		{
			m_fChaseEnemyTime = CBotrixServerPlugin::GetTime() + (EBotPro - m_iIntelligence) + 0.5f;
		}
		else  // Dead or flying?
		{
			m_pChasedEnemy = NULL;
			m_iCurrentTask = EBotTaskInvalid;
			m_bNeedTaskCheck = true;
		}
	}

	// Check new from enemy.
	void CheckEngagedEnemy();

	/// Check if new tasks are needed.
	void CheckNewTasks(bool bForceTaskChange);

	/// Mark task as finished.
	void TaskFinished();

protected:  // Members.
	good::bitset m_aWaypoints;  ///< Waypoints, that bot can't use.

	TBotTask m_iCurrentTask;  ///< Current task.
	TWaypointId m_iTaskDestination;  ///< Waypoint for task destination.
	CPickedItem m_cItemToSearch;  ///< Item, we are searching right now. If iType is -1 then iIndex
								  ///< is waypoint (found no items, so heading to waypoint of that type).

	TWaypointId m_iFailWaypoint;  ///< Waypoint id where bot stucks.
	int m_iFailsCount;  ///< Times bot stucks at m_iFailWaypoint (at 3 times will change destination).

	good::bitset m_cSkipWeapons;  ///< Weapons to skip searching.

	CPlayer* m_pChasedEnemy;  ///< Enemy we are following.
	float m_fChaseEnemyTime;  ///< To recalculate path to enemy every 3 seconds.

protected:  // Flags.
	bool m_bNeedTaskCheck;  ///< True if there is need to check for new tasks.
	bool m_bChasing;  ///< Currently chasing enemy out of sight.

private:
	TBotTask
	ChooseNewTask(bool& bForce, const CWeapon*& pWeapon, TBotIntelligence& iWeaponPreference, bool& bSecondaryWeapon);
};
