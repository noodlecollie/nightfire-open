
#include "botrix/item.h"
#include "botrix/defines.h"
#include "botrix/type2string.h"
#include "botrix/server_plugin.h"
#include "botrix/botrixmod.h"
#include "botrix/clients.h"
#include "botrix/waypoint.h"
#include "botrix/parameter_vars.h"
#include "standard_includes.h"
#include "EnginePublicAPI/engine_limits.h"
#include "PlatformLib/String.h"
#include "weapons.h"

#ifndef BOTRIX_SOURCE_ENGINE_2006
#define BOTRIX_ENTITY_USE_KEY_VALUE
#endif

typedef struct
{
	unsigned short iItemType;
	unsigned short iItemIndex;
} fast_edict_index_t;

fast_edict_index_t m_aEdictsIndexes[MAX_EDICTS];

//================================================================================================================
// inline int GetEntityFlags( CBotrixServerEntity* pServerEntity )
//{
//    // ObjectCaps(), GetMaxHealth(), IsAlive().
//    CBaseEntity* pEntity = pServerEntity->GetBaseEntity();
//    //return pEntity->GetEFlags(); // GetEFlags();
//    int iFlags1 = pEntity->GetEffects();
//    int iFlags2 = pEntity->GetEFlags();
//    int iFlags3 = pEntity->GetFlags();
//    //int iFlags4 = pEntity->GetSolidFlags();
//    int iFlags5 = pEntity->GetSpawnFlags();
//
// #ifdef BOTRIX_ENTITY_USE_KEY_VALUE
//    pEntity->GetKeyValue( "ignite", szValueBuffer, 16 );
//    int flags = atoi( szValueBuffer );
//    return iFlags5;
// #else
//    return pEntity->GetEffects();
// #endif
//}

inline int GetEntityEffects(const CBotrixServerEntity& serverEntity)
{
	return serverEntity.GetEdict()->v.effects;
}

inline int GetEntityHealth(const CBotrixServerEntity& serverEntity)
{
	return static_cast<int>(serverEntity.GetEdict()->v.health);
}

inline bool IsEntityOnMap(const CBotrixServerEntity& serverEntity)
{
	return FLAG_CLEARED(EF_NODRAW, GetEntityEffects(serverEntity));
}

inline bool IsEntityTaken(const CBotrixServerEntity& serverEntity)
{
	CBasePlayerItem* item = serverEntity.GetBasePlayerItem();
	return item && item->m_pPlayer;
}

inline bool IsEntityBreakable(const CBotrixServerEntity& serverEntity)
{
	return GetEntityHealth(serverEntity) != 0;
}

// inline bool CanPickupEntity( CBotrixServerEntity* pServerEntity )
//{
//     CBaseEntity* pEntity = pServerEntity->GetBaseEntity();
//     if ( pEntity == NULL )
//         return false;
//
//     // Not working:
//     IPhysicsObject *pObject = (IPhysicsObject *)pEntity;
//     if ( pObject )
//         return pObject->IsMoveable();
//
//     // Not working:
//     int iEFlags = pEntity->GetEFlags();
//     int iFlags3 = pEntity->GetFlags();
//     int iFlags4 = pEntity->GetSolidFlags();
//     int iSpawnFlags = pEntity->GetSpawnFlags();
//     return !FLAG_SOME_SET( EFL_NO_PHYSCANNON_INTERACTION, iEFlags ) && ( FLAG_SOME_SET( SF_PHYSPROP_ALWAYS_PICK_UP,
//     iSpawnFlags ) ||
//                                                                          !FLAG_SOME_SET( SF_PHYSPROP_PREVENT_PICKUP,
//                                                                          iSpawnFlags ) );
// }

bool CItem::IsFree() const
{
	return !pEdict || pEdict->free;
}

//================================================================================================================
bool CItem::IsOnMap() const
{
	return IsEntityOnMap(CBotrixServerEntity(pEdict));
}

//----------------------------------------------------------------------------------------------------------------
bool CItem::IsBreakable() const
{
	return IsEntityBreakable(CBotrixServerEntity(pEdict));
}

//----------------------------------------------------------------------------------------------------------------
bool CItem::IsTaken() const
{
	return IsEntityTaken(CBotrixServerEntity(pEdict));
}

//================================================================================================================
good::vector<CItem> CItems::m_aItems[EItemTypeKnownTotal];  // Array of items.
good::list<CItemClass> CItems::m_aItemClasses[EItemTypeKnownTotal];  // Array of item classes.
TItemIndex CItems::m_iFreeIndex[EItemTypeKnownTotal];  // First free weapon index.
int CItems::m_iFreeEntityCount[EItemTypeKnownTotal];  // Count of unused entities.

good::vector<edict_t*> CItems::m_aOthers(256);  // Array of other entities.

#ifndef BOTRIX_SOURCE_ENGINE_2006
good::vector<edict_t*> CItems::m_aNewEntities(16);
#endif

good::vector<good::pair<good::string, TItemFlags>> CItems::m_aObjectFlagsForModels(4);
good::vector<TItemIndex> CItems::m_aObjectFlags;

good::bitset CItems::m_aUsedItems(MAX_EDICTS);
int CItems::m_iCurrentEntity;
int CItems::m_iMaxEntityIndex = MAX_EDICTS;
bool CItems::m_bMapLoaded = false;

//----------------------------------------------------------------------------------------------------------------
void CItems::PrintClasses()
{
	for ( TItemType iEntityType = 0; iEntityType < EItemTypeKnownTotal; ++iEntityType )
	{
		BLOG_D("Item type %s:", CTypeToString::EntityTypeToString(iEntityType).c_str());
		const good::list<CItemClass>& aItemClasses = m_aItemClasses[iEntityType];

		for ( good::list<CItemClass>::const_iterator it = aItemClasses.begin(); it != aItemClasses.end(); ++it )
			BLOG_D("  Item class %s", it->sClassName.c_str());
	}
}

TItemType CItems::GetItemFromId(TItemId iId, TItemIndex* pIndex)
{
	GoodAssert(0 <= iId && iId < MAX_EDICTS);
	const fast_edict_index_t& pLookup = m_aEdictsIndexes[iId];
	if ( pIndex )
		*pIndex = pLookup.iItemIndex;
	return pLookup.iItemType;
}

TItemIndex CItems::GetNearestItem(
	TItemType iEntityType,
	const Vector& vOrigin,
	const good::vector<CPickedItem>& aSkip,
	const CItemClass* pClass
)
{
	good::vector<CItem>& aItems = m_aItems[iEntityType];

	TItemIndex iResult = -1;
	float fSqrDistResult = 0.0f;

	for ( int i = 0; i < aItems.size(); ++i )
	{
		CItem& cItem = aItems[i];

		if ( (cItem.pEdict == NULL) || cItem.pEdict->free || !CWaypoint::IsValid(cItem.iWaypoint) ||
			 (pClass && (pClass != cItem.pItemClass)) )  // If item is already added, we have engine name.
		{
			continue;
		}

		CPickedItem cPickedItem(iEntityType, i);

		if ( find(aSkip.begin(), aSkip.end(), cPickedItem) != aSkip.end() )  // Skip this item.
		{
			continue;
		}

		float fSqrDist = (cItem.CurrentPosition() - vOrigin).LengthSquared();

		if ( (iResult == -1) || (fSqrDist < fSqrDistResult) )
		{
			iResult = i;
			fSqrDistResult = fSqrDist;
		}
	}

	return iResult;
}

#ifndef BOTRIX_SOURCE_ENGINE_2006

//----------------------------------------------------------------------------------------------------------------
void CItems::Allocated(edict_t* pEdict)
{
	if ( m_bMapLoaded )
	{
		m_aNewEntities.push_back(pEdict);
	}
}

//----------------------------------------------------------------------------------------------------------------
void CItems::Freed(const edict_t* pEdict)
{
	if ( !m_bMapLoaded )
	{
		return;
	}

	int iIndex = ENTINDEX(pEdict);
	GoodAssert(iIndex > 0);  // Valve should not allow this assert.

	m_aEdictsIndexes[iIndex].iItemType = EItemTypeOther;

	// Check only server entities.
	if ( !m_aUsedItems.test(iIndex) )
	{
		return;
	}

	m_aUsedItems.reset(iIndex);
	good::vector<CItem>& aWeapons = m_aItems[EItemTypeWeapon];

	for ( TItemIndex i = 0; i < (int)aWeapons.size(); ++i )
	{
		if ( aWeapons[i].pEdict == pEdict )
		{
			aWeapons[i].pEdict = NULL;
			m_iFreeIndex[EItemTypeWeapon] = i;
			return;
		}
	}
	// BASSERT(false); // Only weapons are allocated/deallocated while map is running.
}
#endif  // BOTRIX_SOURCE_ENGINE_2006

//----------------------------------------------------------------------------------------------------------------
void CItems::MapUnloaded(bool bClearObjectFlags)
{
	for ( TItemType iEntityType = 0; iEntityType < EItemTypeKnownTotal; ++iEntityType )
	{
		good::list<CItemClass>& aClasses = m_aItemClasses[iEntityType];
		good::vector<CItem>& aItems = m_aItems[iEntityType];

		aItems.clear();
		m_iFreeIndex[iEntityType] = -1;  // Invalidate free entity index.

		for ( good::list<CItemClass>::iterator it = aClasses.begin(); it != aClasses.end(); ++it )
		{
			it->szEngineName = NULL;  // Invalidate class name, because it was loaded in previous map.
			aItems.reserve(64);  // At least.
		}
	}

#ifndef BOTRIX_SOURCE_ENGINE_2006
	m_aNewEntities.clear();
#endif

	m_aOthers.clear();
	if ( bClearObjectFlags )
	{
		m_aObjectFlags.clear();
	}

	m_aUsedItems.reset();
	m_bMapLoaded = false;
}

//----------------------------------------------------------------------------------------------------------------
void CItems::MapLoaded(bool bLog)
{
	// 0 is world, 1..max players are players. Other entities are from indexes above max players.
	m_iCurrentEntity = CPlayers::Size() + 1;
	m_iMaxEntityIndex = CPlayers::Size();

	for ( int i = m_iCurrentEntity; i < MAX_EDICTS; ++i )
	{
		fast_edict_index_t t = {EItemTypeOther, 0};
		m_aEdictsIndexes[i] = t;

		edict_t* pEdict = INDEXENT(i);

		if ( !pEdict || pEdict->free )
		{
			continue;
		}

		// Check items and objects.
		CheckNewEntity(pEdict, bLog);
	}

	m_bMapLoaded = true;
}

//----------------------------------------------------------------------------------------------------------------
void CItems::Update()
{
	// Source engine 2007 uses IServerPluginCallbacks::OnEdictAllocated instead of checking all array of edicts.

	// Update weapons we have in items array.
	good::vector<CItem>& aWeapons = m_aItems[EItemTypeWeapon];
	for ( TItemIndex i = 0; i < aWeapons.size(); ++i )
	{
		CItem& cEntity = aWeapons[i];
		edict_t* pEdict = cEntity.pEdict;
		if ( pEdict == NULL )
		{
			m_iFreeIndex[EItemTypeWeapon] = i;
			continue;
		}

		CBotrixServerEntity serverEntity(pEdict);

		if ( pEdict->free )
		{
			cEntity.pEdict = NULL;
			m_iFreeIndex[EItemTypeWeapon] = i;
			m_aUsedItems.reset(ENTINDEX(pEdict));
		}
		else if ( IsEntityTaken(serverEntity) )  // Weapon still belongs to some player.
		{
			FLAG_SET(FTaken, cEntity.iFlags);
		}
		else if ( FLAG_ALL_SET_OR_0(FTaken, cEntity.iFlags) && IsEntityOnMap(serverEntity) )
		{
			FLAG_CLEAR(FTaken, cEntity.iFlags);
			cEntity.vOrigin = cEntity.CurrentPosition();
			cEntity.iWaypoint = CWaypoints::GetNearestWaypoint(cEntity.vOrigin, NULL, true, CItem::iMaxDistToWaypoint);
		}
	}

	// Update weapon entities on map.
	int entsToCheck = m_iCheckEntitiesPerFrame;
	TItemIndex i = m_iCurrentEntity;

	for ( ; i < MAX_EDICTS && entsToCheck > 0; ++i )
	{
		if ( m_aUsedItems.test(i) )
		{
			continue;
		}

		edict_t* pEdict = INDEXENT(i);

		if ( !pEdict || pEdict->free )
		{
			continue;
		}

		--entsToCheck;

		// Check only server entities.
		CBotrixServerEntity serverEntity(pEdict);

		// Check only for new weapons, because new weapon instance is created when weapon is picked up.
		CItemClass* pWeaponClass;
		TItemType iEntityType =
			GetEntityType(STRING(pEdict->v.classname), pWeaponClass, EItemTypeWeapon, EItemTypeWeapon + 1);

		if ( iEntityType == EItemTypeWeapon )
		{
			AddItem(EItemTypeWeapon, pEdict, pWeaponClass, serverEntity);
		}
	}

	m_iCurrentEntity = (i >= MAX_EDICTS) ? CPlayers::Size() + 1 : i;
}

//----------------------------------------------------------------------------------------------------------------
good::vector<TItemId>::iterator LocateObjectFlags(good::vector<TItemId>& aFlags, TItemId iObject)
{
	for ( good::vector<TItemId>::iterator it = aFlags.begin(); it != aFlags.end(); it += 2 )
		if ( *it == iObject )
			return it;
	return aFlags.end();
}

bool CItems::GetObjectFlags(TItemId iObject, TItemFlags& iFlags)
{
	good::vector<TItemId>::const_iterator it = LocateObjectFlags(m_aObjectFlags, iObject);
	if ( it == m_aObjectFlags.end() )
		return false;
	iFlags = *(++it);
	return true;
}

bool CItems::SetObjectFlags(TItemId iObject, TItemFlags iFlags)
{
	GoodAssert(0 <= iObject && iObject < MAX_EDICTS);
	if ( m_aEdictsIndexes[iObject].iItemType != EItemTypeObject )
		return false;

	int iIndex = m_aEdictsIndexes[iObject].iItemIndex;
	m_aItems[EItemTypeObject][iIndex].iFlags = iFlags;

	good::vector<TItemId>::iterator it = LocateObjectFlags(m_aObjectFlags, iObject);
	if ( it == m_aObjectFlags.end() )
	{
		m_aObjectFlags.push_back(iObject);
		m_aObjectFlags.push_back(iFlags);
	}
	else
		*(++it) = iFlags;

	return true;
}

//----------------------------------------------------------------------------------------------------------------
TItemType CItems::GetEntityType(const char* szClassName, CItemClass*& pEntityClass, TItemType iFrom, TItemType iTo)
{
	GoodAssert(iTo < EItemTypeAll);
	for ( TItemType iEntityType = iFrom; iEntityType < iTo; ++iEntityType )
	{
		const good::list<CItemClass>& aItemClasses = m_aItemClasses[iEntityType];

		for ( good::list<CItemClass>::iterator it = aItemClasses.begin(); it != aItemClasses.end(); ++it )
		{
			CItemClass& cEntityClass = *it;
			GoodAssert(cEntityClass.sClassName.size() > 0);

			if ( cEntityClass.szEngineName &&
				 (cEntityClass.szEngineName == szClassName) )  // Compare by pointer, faster.
			{
				pEntityClass = &cEntityClass;
				return iEntityType;
			}
			else if ( cEntityClass.sClassName == szClassName )  // Full string content comparison.
			{
				cEntityClass.szEngineName = szClassName;  // Save engine string.
				pEntityClass = &cEntityClass;
				return iEntityType;
			}
		}
	}

	return EItemTypeOther;
}

//----------------------------------------------------------------------------------------------------------------
void CItems::CheckNewEntity(struct edict_s* pEdict, bool bLog)
{
	const char* szClassName = STRING(pEdict->v.classname);

	if ( szClassName == NULL || szClassName[0] == 0 )
	{
		return;
	}

	int iEntIndex = ENTINDEX(pEdict);

	if ( bLog )
	{
		BLOG_T("New entity: %s, index %d.", szClassName ? szClassName : "", iEntIndex);
	}

	m_iMaxEntityIndex = MAX2(iEntIndex, m_iMaxEntityIndex);

	// Check only server entities.
	CBotrixServerEntity serverEntity(pEdict);

	CItemClass* pItemClass;
	TItemType iEntityType = GetEntityType(szClassName, pItemClass, 0, EItemTypeKnownTotal);

	if ( iEntityType == EItemTypeOther )
	{
		fast_edict_index_t t = {EItemTypeOther, (unsigned short)m_aOthers.size()};
		m_aEdictsIndexes[iEntIndex] = t;
		m_aOthers.push_back(pEdict);
	}
	else if ( iEntityType == EItemTypeObject )
	{
		AddObject(pEdict, pItemClass, serverEntity);
	}
	else
	{
		TItemIndex iIndex = AddItem(iEntityType, pEdict, pItemClass, serverEntity);
		BASSERT(iIndex >= 0, return);

		CItem& cItem = m_aItems[iEntityType][iIndex];

		if ( bLog )
		{
			BLOG_D(
				"New item: %s %d (%s), waypoint %d (%s).",
				CTypeToString::EntityTypeToString(iEntityType).c_str(),
				iIndex,
				STRING(pEdict->v.classname),
				cItem.iWaypoint,
				CWaypoints::IsValid(cItem.iWaypoint)
					? CTypeToString::WaypointFlagsToString(CWaypoints::Get(cItem.iWaypoint).iFlags).c_str()
					: ""
			);
		}

		if ( !m_bMapLoaded && FLAG_CLEARED(FTaken, cItem.iFlags) )  // Item should have waypoint near.
		{
			if ( !CWaypoint::IsValid(cItem.iWaypoint) )
			{
				const Vector& vOrigin = cItem.CurrentPosition();

				if ( bLog )
				{
					BLOG_W(
						"Warning: entity %s %d (%.0f %.0f %.0f) doesn't have waypoint close.",
						STRING(pEdict->v.classname),
						iIndex + 1,
						vOrigin.x,
						vOrigin.y,
						vOrigin.z
					);
				}

				TWaypointId iWaypoint = CWaypoints::GetNearestWaypoint(cItem.vOrigin);

				if ( CWaypoint::IsValid(iWaypoint) && bLog )
				{
					BLOG_W("  Nearest waypoint %d.", iWaypoint);
				}
			}
			else if ( bLog && iEntityType == EItemTypeDoor &&
					  !CWaypoint::IsValid((TWaypointId)((intptr_t)cItem.pArguments)) )
			{
				BLOG_W("Door %d doesn't have 2 waypoints near.", iIndex);
			}
		}

#ifdef BOTRIX_SOURCE_ENGINE_2006
		// Weapon entities are allocated / deallocated when respawned / owner killed.
		if ( iEntityType != EItemTypeWeapon )
#endif
		{
			int entIndex = ENTINDEX(pEdict);
			GoodAssert(entIndex > 0);  // Valve should not allow this assert.
			m_aUsedItems.set(entIndex);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
TItemIndex CItems::NewEntityIndex(int iEntityType)
{
	good::vector<CItem>& aItems = m_aItems[iEntityType];

	if ( m_bMapLoaded )  // Check if there are free space in items array.
	{
		if ( m_iFreeIndex[iEntityType] != -1 )
		{
			int iIndex = m_iFreeIndex[iEntityType];
			m_iFreeIndex[iEntityType] = -1;
			return iIndex;
		}

		for ( int i = 0; i < aItems.size(); ++i )  // TODO: add free count.
			if ( aItems[i].pEdict == NULL )
				return i;
	}

	return aItems.size();
}

//----------------------------------------------------------------------------------------------------------------
void CItems::AutoWaypointPathFlagsForEntity(TItemType iEntityType, TItemIndex iIndex, CItem& cEntity)
{
	TWaypointId iWaypoint = cEntity.iWaypoint;
	if ( (iEntityType == EItemTypeButton) && (iWaypoint != EWaypointIdInvalid) )
	{
		// Set waypoint argument to button.
		CWaypoint& cWaypoint = CWaypoints::Get(iWaypoint);
		FLAG_SET(FWaypointButton, cWaypoint.iFlags);
		CWaypoint::SetButton(iIndex, cWaypoint.iArgument);
	}

	// Check 2nd nearest waypoint for door.
	if ( iEntityType == EItemTypeDoor )
	{
		if ( iWaypoint != EWaypointIdInvalid )
		{
			good::bitset cOmitWaypoints(CWaypoints::Size());
			cOmitWaypoints.set(iWaypoint);
			iWaypoint =
				CWaypoints::GetNearestWaypoint(cEntity.vOrigin, &cOmitWaypoints, true, CItem::iMaxDistToWaypoint);
		}
		cEntity.pArguments = (void*)(intptr_t)iWaypoint;

		// Set door for paths between these two waypoints.
		if ( iWaypoint != EWaypointIdInvalid )
		{
			CWaypointPath* pPath = CWaypoints::GetPath(iWaypoint, cEntity.iWaypoint);  // From -> To.
			if ( pPath )
			{
				FLAG_SET(FPathDoor, pPath->iFlags);
				pPath->SetDoorNumber(iIndex);
			}

			pPath = CWaypoints::GetPath(cEntity.iWaypoint, iWaypoint);  // To -> From.
			if ( pPath )
			{
				FLAG_SET(FPathDoor, pPath->iFlags);
				pPath->SetDoorNumber(iIndex);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
TItemIndex CItems::AddItem(
	TItemType iEntityType,
	struct edict_s* pEdict,
	CItemClass* pItemClass,
	const CBotrixServerEntity& serverEntity
)
{
	GoodAssert((0 <= iEntityType) && (iEntityType < EItemTypeAll));

	const Vector vItemOrigin = serverEntity.GetOrigin();
	float fPickupDistanceSqr = pItemClass->fPickupDistanceSqr;

	if ( fPickupDistanceSqr < 1.0f )
	{
		// Calculate object radius.
		fPickupDistanceSqr = serverEntity.GetCollisionRadius();

		if ( iEntityType < EItemTypeCanPickTotal )
		{
			fPickupDistanceSqr += CBotrixParameterVars::GetPlayerRadiusInt() + CBotrixMod::iItemPickUpDistance;
			fPickupDistanceSqr *= fPickupDistanceSqr * 2;
		}
		else
		{
			const float toSquare = fPickupDistanceSqr + CBotrixParameterVars::GetPlayerRadiusInt();
			fPickupDistanceSqr = toSquare * toSquare;
		}

		pItemClass->fPickupDistanceSqr = fPickupDistanceSqr;
	}

	int iFlags = pItemClass->iFlags;
	TWaypointId iWaypoint = -1;

	if ( !IsEntityOnMap(serverEntity) || IsEntityTaken(serverEntity) )
	{
		FLAG_SET(FTaken, iFlags);
	}
	else
	{
		iWaypoint = CWaypoints::GetNearestWaypoint(vItemOrigin, NULL, true, CItem::iMaxDistToWaypoint);
	}

	TItemIndex iIndex = NewEntityIndex(iEntityType);
	fast_edict_index_t t = {(unsigned short)iEntityType, (unsigned short)iIndex};
	int entIndex = ENTINDEX(pEdict);
	m_aEdictsIndexes[entIndex] = t;

	CItem cNewEntity(pEdict, iFlags, fPickupDistanceSqr, pItemClass, vItemOrigin, iWaypoint);
	AutoWaypointPathFlagsForEntity(iEntityType, iIndex, cNewEntity);

	good::vector<CItem>& aItems = m_aItems[iEntityType];

	if ( iIndex < aItems.size() )
	{
		aItems[iIndex] = cNewEntity;
	}
	else
	{
		aItems.push_back(cNewEntity);
	}

	return iIndex;
}

//----------------------------------------------------------------------------------------------------------------
void CItems::AddObject(struct edict_s* pEdict, const CItemClass* pObjectClass, const CBotrixServerEntity& serverEntity)
{
	// Calculate object radius.
	// TODO: why not use object class to get distance?
	float radiusSquared = serverEntity.GetCollisionRadius();
	radiusSquared += CBotrixParameterVars::GetPlayerRadiusInt();
	radiusSquared *= radiusSquared;

	TItemIndex iIndex = NewEntityIndex(EItemTypeObject);
	fast_edict_index_t t = {EItemTypeObject, (unsigned short)iIndex};
	int edictIndex = ENTINDEX(pEdict);
	m_aEdictsIndexes[edictIndex] = t;

	TItemFlags iFlags = pObjectClass->iFlags;

	// Get object flags (saved in waypoints).
	bool bHasFlags = GetObjectFlags(edictIndex, iFlags);

	if ( !bHasFlags )
	{
		// Get flags from model name.
		if ( m_aObjectFlagsForModels.size() )
		{
			good::string sModel = STRING(pEdict->v.model);

			for ( int i = 0; i < m_aObjectFlagsForModels.size(); ++i )
			{
				if ( sModel.find(m_aObjectFlagsForModels[i].first) != good::string::npos )
				{
					FLAG_SET(m_aObjectFlagsForModels[i].second, iFlags);
					break;
				}
			}
		}
	}

	CItem cObject(pEdict, iFlags, radiusSquared, pObjectClass, serverEntity.GetOrigin(), -1);
	good::vector<CItem>& aItems = m_aItems[EItemTypeObject];

	if ( iIndex < aItems.size() )
	{
		aItems[iIndex] = cObject;
	}
	else
	{
		aItems.push_back(cObject);
	}
}

//----------------------------------------------------------------------------------------------------------------
int CItems::IsDoorClosed(TItemIndex iDoor)
{
	BASSERT(0 <= iDoor && iDoor < m_aItems[EItemTypeDoor].size(), return false);
	const CItem& cDoor = m_aItems[EItemTypeDoor][iDoor];
	TWaypointId w1 = cDoor.iWaypoint, w2 = (TWaypointId)(intptr_t)cDoor.pArguments;

	if ( !CWaypoints::IsValid(w1) || !CWaypoints::IsValid(w2) )  // Door should have two waypoints from each side.
	{
		return -1;
	}

	const Vector& v1 = CWaypoints::Get(w1).vOrigin;
	const Vector& v2 = CWaypoints::Get(w2).vOrigin;
	return CBotrixEngineUtil::RayHitsEntity(cDoor.pEdict, v1, v2) ? 1 : 0;
}

//----------------------------------------------------------------------------------------------------------------
void CItems::Draw(CBotrixClient* pClient)
{
	static float fNextDrawTime = 0.0f;
	static unsigned char pvs[(MAX_MAP_LEAFS + 7) / 8];

	char printBuffer[256];

	if ( (pClient->iItemDrawFlags == FItemDontDraw) || (pClient->iItemTypeFlags == 0) ||
		 (CBotrixServerPlugin::GetTime() < fNextDrawTime) )
	{
		return;
	}

	fNextDrawTime = CBotrixServerPlugin::GetTime() + 1.0f;

	const byte* pvsForPoint = g_engfuncs.pfnGetPvsForPoint(pClient->GetHead());
	memcpy(pvs, pvsForPoint, sizeof(pvs));

	Vector vHead = pClient->GetHead();
	CBotrixEngineUtil::SetPVSForVector(vHead);

	for ( TItemType iEntityType = 0; iEntityType < EItemTypeAll; ++iEntityType )
	{
		if ( !FLAG_SOME_SET(1 << iEntityType, pClient->iItemTypeFlags) )  // Don't draw items of disabled item type.
		{
			continue;
		}

		int iSize = (iEntityType == EItemTypeOther) ? m_aOthers.size() : m_aItems[iEntityType].size();
		for ( int i = 0; i < iSize; ++i )
		{
			edict_t* pEdict = (iEntityType == EItemTypeOther) ? m_aOthers[i] : m_aItems[iEntityType][i].pEdict;

			if ( (pEdict == NULL) || pEdict->free )
			{
				continue;
			}

			CBotrixServerEntity serverEntity(pEdict);

			if ( IsEntityTaken(serverEntity) )
			{
				continue;
			}

			const Vector vOrigin = serverEntity.GetOrigin();

			if ( CBotrixEngineUtil::IsVisiblePVS(vOrigin) &&
				 CBotrixEngineUtil::IsVisible(vHead, vOrigin, EVisibilityWorld, false) )
			{
				const CItem* pEntity = (iEntityType == EItemTypeOther) ? NULL : &m_aItems[iEntityType][i];

				if ( FLAG_SOME_SET(FItemDrawStats, pClient->iItemDrawFlags) )
				{
					// Draw entity class name name with index.
					PlatformLib_SNPrintF(
						printBuffer,
						sizeof(printBuffer),
						"%s %d (id %d)",
						CTypeToString::EntityTypeToString(iEntityType).c_str(),
						i,
						ENTINDEX(pEdict)
					);

					int pos = 0;
					CBotrixEngineUtil::DrawTextAtLocation(vOrigin, pos++, 1.0f, 0xFF, 0xFF, 0xFF, printBuffer);
					CBotrixEngineUtil::DrawTextAtLocation(
						vOrigin,
						pos++,
						1.0f,
						0xFF,
						0xFF,
						0xFF,
						STRING(pEdict->v.classname)
					);

					if ( iEntityType == EItemTypeDoor )
					{
						int iClosed = IsDoorClosed(i);
						CBotrixEngineUtil::DrawTextAtLocation(
							vOrigin,
							pos++,
							1.0f,
							0xFF,
							0xFF,
							0xFF,
							iClosed == -1      ? "no near waypoints"
								: iClosed == 0 ? "opened"
											   : "closed"
						);
					}
					else if ( iEntityType == EItemTypeObject )
					{
						PlatformLib_SNPrintF(
							printBuffer,
							sizeof(printBuffer),
							"%s %d",
							CTypeToString::EntityTypeToString(iEntityType).c_str(),
							i
						);

						CBotrixEngineUtil::DrawTextAtLocation(
							vOrigin,
							pos++,
							1.0f,
							0xFF,
							0xFF,
							0xFF,
							IsEntityOnMap(serverEntity) ? "alive" : "dead"
						);

						// CBotrixEngineUtil::DrawText( vOrigin, pos++, 1.0f, 0xFF, 0xFF, 0xFF,
						// IsEntityTaken(pServerEntity) ? "taken" : "not taken" ); // Taken not shown.
						// CBotrixEngineUtil::DrawText( vOrigin, pos++, 1.0f, 0xFF, 0xFF, 0xFF,
						// CanPickupEntity(pServerEntity) ? "moveable" : "static" ); // Taken not shown.

						CBotrixEngineUtil::DrawTextAtLocation(
							vOrigin,
							pos++,
							1.0f,
							0xFF,
							0xFF,
							0xFF,
							IsEntityBreakable(serverEntity) ? "breakable" : "non breakable"
						);

						const char* sFlags = CTypeToString::EntityClassFlagsToString(pEntity->iFlags, false).c_str();

						if ( sFlags[0] != 0 )
						{
							CBotrixEngineUtil::DrawTextAtLocation(vOrigin, pos++, 1.0f, 0xFF, 0xFF, 0xFF, sFlags);
						}
					}

					if ( iEntityType >= EItemTypeObject )  // Draw object model.
					{
						CBotrixEngineUtil::DrawTextAtLocation(
							vOrigin,
							pos++,
							1.0f,
							0xFF,
							0xFF,
							0xFF,
							STRING(pEdict->v.model)
						);
					}
				}

				// Draw box around item.
				if ( FLAG_SOME_SET(FItemDrawBoundBox, pClient->iItemDrawFlags) )
				{
					CBotrixEngineUtil::DrawBox(
						vOrigin,
						serverEntity.GetMins(),
						serverEntity.GetMaxs(),
						1.0f,
						0xFF,
						0xFF,
						0xFF
					);
				}

				if ( FLAG_SOME_SET(FItemDrawWaypoint, pClient->iItemDrawFlags) && (iEntityType < EItemTypeObject) )
				{
					// Draw nearest waypoint from item, yellow.
					if ( CWaypoint::IsValid(pEntity->iWaypoint) )
					{
						CBotrixEngineUtil::DrawLine(
							CWaypoints::Get(pEntity->iWaypoint).vOrigin,
							vOrigin,
							1.0f,
							0xFF,
							0xFF,
							0
						);
					}

					// Draw second waypoint for door, yellow.
					if ( (iEntityType == EItemTypeDoor) &&
						 CWaypoint::IsValid((TWaypointId)(intptr_t)pEntity->pArguments) )
					{
						CBotrixEngineUtil::DrawLine(
							CWaypoints::Get((TWaypointId)(intptr_t)pEntity->pArguments).vOrigin,
							vOrigin,
							1.0f,
							0xFF,
							0xFF,
							0
						);
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------
// void CItems::WaypointDeleted( TWaypointId id )
//{
//    for ( int iEntityType = 0; iEntityType < EItemTypeTotal; ++iEntityType )
//    {
//        good::vector<CItem>& aItems = m_aItems[iEntityType];
//
//        for ( int i = 0; i < aItems.size(); ++i )
//        {
//            CItem& cItem = aItems[i];
//            if ( cItem.iWaypoint == id )
//                cItem.iWaypoint = CWaypoints::GetNearestWaypoint( cItem.vOrigin );
//            else if ( cItem.iWaypoint > id )
//                --cItem.iWaypoint;
//
//            // TODO: update doors.
//            //if ( iEntityType == EItemTypeDoor )
//        }
//    }
//}
