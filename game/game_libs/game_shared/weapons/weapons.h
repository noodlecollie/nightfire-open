/***
 *
 *	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
 *
 *	This product contains software technology licensed from Id
 *	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *	All Rights Reserved.
 *
 *   Use, distribution, and modification of this source code and/or resulting
 *   object code is restricted to non-commercial enhancements to products from
 *   Valve LLC.  All other use, distribution, or modification is prohibited
 *   without written permission from Valve LLC.
 *
 ****/

#pragma once

#include "effects.h"
#include "cdll_dll.h"

class CBasePlayer;
typedef struct weapon_data_s weapon_data_t;
extern int gmsgWeapPickup;

extern DLL_GLOBAL short g_sModelIndexLaser;  // holds the index for the laser beam
extern DLL_GLOBAL const char* g_pModelNameLaser;

extern DLL_GLOBAL short g_sModelIndexLaserDot;  // holds the index for the laser beam dot
extern DLL_GLOBAL short g_sModelIndexFireball;  // holds the index for the fireball
extern DLL_GLOBAL short g_sModelIndexSmoke;  // holds the index for the smoke cloud
extern DLL_GLOBAL short g_sModelIndexWExplosion;  // holds the index for the underwater explosion
extern DLL_GLOBAL short g_sModelIndexBubbles;  // holds the index for the bubbles model
extern DLL_GLOBAL short g_sModelIndexBloodDrop;  // holds the sprite index for blood drops
extern DLL_GLOBAL short g_sModelIndexBloodSpray;  // holds the sprite index for blood spray (bigger)

// the maximum amount of ammo each weapon's clip can hold
#define WEAPON_NOCLIP -1
#define WEAPON_ALLWEAPONS (~(1 << WEAPON_SUIT))

// constant items
#define ITEM_HEALTHKIT 1
#define ITEM_BATTERY 4

#define WEAPON_NONE 0

#define MAX_NORMAL_BATTERY 100

#define LOUD_GUN_VOLUME 1000
#define NORMAL_GUN_VOLUME 600
#define QUIET_GUN_VOLUME 200

#define BRIGHT_GUN_FLASH 512
#define NORMAL_GUN_FLASH 256
#define DIM_GUN_FLASH 128
#define NO_GUN_FLASH 0

#define BIG_EXPLOSION_VOLUME 2048
#define NORMAL_EXPLOSION_VOLUME 1024
#define SMALL_EXPLOSION_VOLUME 512

#define WEAPON_ACTIVITY_VOLUME 64

#define VECTOR_CONE_1DEGREES Vector(0.00873f, 0.00873f, 0.00873f)
#define VECTOR_CONE_2DEGREES Vector(0.01745f, 0.01745f, 0.01745f)
#define VECTOR_CONE_3DEGREES Vector(0.02618f, 0.02618f, 0.02618f)
#define VECTOR_CONE_4DEGREES Vector(0.03490f, 0.03490f, 0.03490f)
#define VECTOR_CONE_5DEGREES Vector(0.04362f, 0.04362f, 0.04362f)
#define VECTOR_CONE_6DEGREES Vector(0.05234f, 0.05234f, 0.05234f)
#define VECTOR_CONE_7DEGREES Vector(0.06105f, 0.06105f, 0.06105f)
#define VECTOR_CONE_8DEGREES Vector(0.06976f, 0.06976f, 0.06976f)
#define VECTOR_CONE_9DEGREES Vector(0.07846f, 0.07846f, 0.07846f)
#define VECTOR_CONE_10DEGREES Vector(0.08716f, 0.08716f, 0.08716f)
#define VECTOR_CONE_15DEGREES Vector(0.13053f, 0.13053f, 0.13053f)
#define VECTOR_CONE_20DEGREES Vector(0.17365f, 0.17365f, 0.17365f)

#define ITEM_FLAG_SELECTONEMPTY 1
#define ITEM_FLAG_NOAUTORELOAD 2
#define ITEM_FLAG_NOAUTOSWITCHEMPTY 4
#define ITEM_FLAG_LIMITINWORLD 8
#define ITEM_FLAG_EXHAUSTIBLE 16  // A player can totally exhaust their ammo supply and lose this weapon

#define WEAPON_IS_ONTARGET 0x40

// bullet types
// NOTE: At some point (once we make the original HL weapons obsolete)
// we'll want to remove the whole concept of bullet types. Damage will
// be calculated by the weapon that was fired, plus skill settings.
typedef enum
{
	BULLET_NONE = 0,
	BULLET_PLAYER_9MM,  // glock
	BULLET_PLAYER_MP5,  // mp5
	BULLET_PLAYER_357,  // python
	BULLET_PLAYER_BUCKSHOT,  // shotgun
	BULLET_PLAYER_CROWBAR,  // crowbar swipe
	BULLET_GENERIC,  // Check for ammo-specific damage instead
	BULLET_MELEE,

	BULLET_MONSTER_9MM,
	BULLET_MONSTER_MP5,
	BULLET_MONSTER_12MM
} Bullet;

typedef struct
{
	int iSlot;
	int iPosition;
	const char* pszAmmo1;  // ammo 1 type
	int iMaxAmmo1;  // max ammo 1
	const char* pszAmmo2;  // ammo 2 type
	int iMaxAmmo2;  // max ammo 2
	const char* pszName;
	int iMaxClip;
	int iId;
	int iFlags;
	int iWeight;  // this value used to determine this weapon's importance in autoselection.
} ItemInfo;

typedef struct
{
	const char* pszName;
	int iId;
} AmmoInfo;

// Items that the player has in their inventory that they can use
class CBasePlayerItem : public CBaseAnimating
{
public:
	virtual void SetObjectCollisionBox(void);

	virtual int Save(CSave& save);
	virtual int Restore(CRestore& restore);

	static TYPEDESCRIPTION m_SaveData[];

	virtual int AddToPlayer(
		CBasePlayer* pPlayer
	);  // return TRUE if the item you want the item added to the player inventory
	virtual int AddDuplicate(CBasePlayerItem*)
	{
		return FALSE;
	}  // return TRUE if you want your duplicate removed from world
	void EXPORT DestroyItem(void);
	void EXPORT DefaultTouch(CBaseEntity* pOther);  // default weapon touch
	void EXPORT FallThink(
		void
	);  // when an item is first spawned, this think is run to determine when the object has hit the ground.
	void EXPORT Materialize(void);  // make a weapon visible and tangible
	void EXPORT
	AttemptToMaterialize(void);  // the weapon desires to become visible and tangible, if the game rules allow for it
	CBaseEntity* Respawn(void);  // copy a weapon
	void FallInit(void);
	void CheckRespawn(void);
	virtual int GetItemInfo(ItemInfo*)
	{
		return 0;
	};  // returns 0 if struct not filled out
	virtual BOOL CanDeploy(void)
	{
		return TRUE;
	};
	virtual BOOL Deploy()  // returns is deploy was successful
	{
		return TRUE;
	};

	virtual BOOL CanHolster(void)
	{
		return TRUE;
	};  // can this weapon be put away right now?
	virtual void Holster(int skiplocal = 0);
	virtual void UpdateItemInfo(void)
	{
		return;
	};

	virtual void ItemPreFrame(void)
	{
		return;
	}  // called each frame by the player PreThink
	virtual void ItemPostFrame(void)
	{
		return;
	}  // called each frame by the player PostThink

	virtual void Drop(void);
	virtual void Kill(void);
	virtual void AttachToPlayer(CBasePlayer* pPlayer);
	virtual void DetachFromPlayer(CBasePlayer* pPlayer);

	virtual int PrimaryAmmoIndex()
	{
		return -1;
	};
	virtual int SecondaryAmmoIndex()
	{
		return -1;
	};

	virtual int UpdateClientData(CBasePlayer*)
	{
		return 0;
	}

	virtual CBasePlayerItem* GetWeaponPtr(void)
	{
		return NULL;
	};

	static ItemInfo ItemInfoArray[MAX_WEAPONS];
	static AmmoInfo AmmoInfoArray[MAX_AMMO_SLOTS];

	CBasePlayer* m_pPlayer;
	CBasePlayerItem* m_pNext;
	int m_iId;  // WEAPON_???

	virtual int iItemSlot(void)
	{
		return 0;
	}  // return 0 to MAX_ITEMS_SLOTS, used in hud

	int iItemPosition(void)
	{
		return ItemInfoArray[m_iId].iPosition;
	}
	const char* pszAmmo1(void)
	{
		return ItemInfoArray[m_iId].pszAmmo1;
	}
	int iMaxAmmo1(void)
	{
		return ItemInfoArray[m_iId].iMaxAmmo1;
	}
	const char* pszAmmo2(void)
	{
		return ItemInfoArray[m_iId].pszAmmo2;
	}
	int iMaxAmmo2(void)
	{
		return ItemInfoArray[m_iId].iMaxAmmo2;
	}
	const char* pszName(void)
	{
		return ItemInfoArray[m_iId].pszName;
	}
	int iMaxClip(void)
	{
		return ItemInfoArray[m_iId].iMaxClip;
	}
	int iWeight(void)
	{
		return ItemInfoArray[m_iId].iWeight;
	}
	int iFlags(void)
	{
		return ItemInfoArray[m_iId].iFlags;
	}

	// int		m_iIdPrimary;										// Unique Id for primary ammo
	// int		m_iIdSecondary;										// Unique Id for secondary ammo

	// Slight hack until we merge CBasePlayerItem and CGenericWeapon better.
	virtual const char* PickupSound() const;
};

// inventory items that
class CBasePlayerWeapon : public CBasePlayerItem
{
public:
	virtual int Save(CSave& save);
	virtual int Restore(CRestore& restore);

	static TYPEDESCRIPTION m_SaveData[];

	// generic weapon versions of CBasePlayerItem calls
	virtual int AddToPlayer(CBasePlayer* pPlayer);
	virtual int AddDuplicate(CBasePlayerItem* pItem);

	virtual int ExtractAmmo(CBasePlayerWeapon* pWeapon);  //{ return TRUE; };			// Return TRUE if you can add
														  // ammo to yourself when picked up
	virtual int ExtractClipAmmo(CBasePlayerWeapon* pWeapon);  // { return TRUE; };			// Return TRUE if you can
															  // add ammo to yourself when picked up

	virtual int AddWeapon(void)
	{
		ExtractAmmo(this);
		return TRUE;
	};  // Return TRUE if you want to add yourself to the player

	// generic "shared" ammo handlers
	BOOL AddPrimaryAmmo(int iCount, char* szName, int iMaxClip, int iMaxCarry);
	BOOL AddSecondaryAmmo(int iCount, char* szName, int iMaxCarry);

	virtual void UpdateItemInfo(void) {};  // updates HUD state

	int m_iPlayEmptySound;
	int m_fFireOnEmpty;  // True when the gun is empty and the player is still holding down the
						 // attack key(s)
	virtual BOOL PlayEmptySound(void);
	virtual void ResetEmptySound(void);

	virtual void SendWeaponAnim(int iAnim, int body = 0);

	virtual BOOL CanDeploy(void);
	virtual BOOL IsUseable(void);
	BOOL
	DefaultDeploy(const char* szViewModel, const char* szWeaponModel, int iAnim, const char* szAnimExt, int body = 0);
	int DefaultReload(int iClipSize, int iAnim, float fDelay, int body = 0);
	bool IsActiveItem() const;

	virtual void ItemPostFrame(void);  // called each frame by the player PostThink
	// called by CBasePlayerWeapons ItemPostFrame()
	virtual void PrimaryAttack(void)
	{
		return;
	}  // do "+ATTACK"
	virtual void SecondaryAttack(void)
	{
		return;
	}  // do "+ATTACK2"
	virtual void Reload(void)
	{
		return;
	}  // do "+RELOAD"
	virtual void WeaponTick()
	{
	}  // Always called at beginning of ItemPostFrame. - Solokiller
	virtual void WeaponIdle(void)
	{
		return;
	}  // called when no buttons pressed
	virtual int UpdateClientData(CBasePlayer* pPlayer);  // sends hud info to client dll, if things have changed
	virtual void RetireWeapon(void);
	virtual BOOL ShouldWeaponIdle(void)
	{
		return FALSE;
	};
	virtual void Holster(int skiplocal = 0);

	inline BOOL UseDecrement(void) const
	{
#if defined(CLIENT_WEAPONS)
		return TRUE;
#else
		return FALSE;
#endif
	};

	int PrimaryAmmoIndex();
	int SecondaryAmmoIndex();

	void PrintState(void);

	virtual CBasePlayerItem* GetWeaponPtr(void)
	{
		return (CBasePlayerItem*)this;
	};
	float GetNextAttackDelay(float delay);

	int m_fInSpecialReload;  // Are we in the middle of a reload for the shotguns
	float m_flNextPrimaryAttack;  // soonest time ItemPostFrame will call PrimaryAttack
	float m_flNextSecondaryAttack;  // soonest time ItemPostFrame will call SecondaryAttack
	float m_flTimeWeaponIdle;  // soonest time ItemPostFrame will call WeaponIdle
	float m_flEnqueuedMechanicInvocationTime;
	float m_flLastPrimaryAttack;
	float m_flLastSecondaryAttack;
	int m_iPrimaryAmmoType;  // "primary" ammo index into players m_rgAmmo[]
	int m_iSecondaryAmmoType;  // "secondary" ammo index into players m_rgAmmo[]
	int m_iClip;  // number of shots left in the primary weapon clip, -1 it not used
	int m_iClientClip;  // the last version of m_iClip sent to hud dll
	int m_iClientWeaponState;  // the last version of the weapon state sent to hud dll (is current weapon, is on target)
	int m_fInReload;  // Are we in the middle of a reload;

	int m_iDefaultAmmo;  // how much ammo you get when you pick up this weapon as placed by a level designer.

	// hle time creep vars
	float m_flPrevPrimaryAttack;
	float m_flLastFireTime;

	virtual bool ReadPredictionData(const weapon_data_t* from);
	virtual bool WritePredictionData(weapon_data_t* to);

protected:
	static BOOL CanAttack(float attack_time, float curtime, BOOL isPredicted);
};

class CBasePlayerAmmo : public CBaseEntity
{
public:
	virtual void Spawn(void);
	void EXPORT DefaultTouch(CBaseEntity* pOther);  // default weapon touch
	virtual BOOL AddAmmo(CBaseEntity*)
	{
		return TRUE;
	};

	CBaseEntity* Respawn(void);
	void EXPORT Materialize(void);
};

// Contact Grenade / Timed grenade / Satchel Charge
class CGrenade : public CBaseMonster
{
public:
	CGrenade();
	void Spawn(void);

	typedef enum
	{
		SATCHEL_DETONATE = 0,
		SATCHEL_RELEASE
	} SATCHELCODE;

	static CGrenade* ShootTimed(entvars_t* pevOwner, Vector vecStart, Vector vecVelocity, float time);
	static CGrenade* ShootContact(entvars_t* pevOwner, Vector vecStart, Vector vecVelocity);
	static CGrenade* ShootSatchelCharge(entvars_t* pevOwner, Vector vecStart, Vector vecVelocity);
	static void UseSatchelCharges(entvars_t* pevOwner, SATCHELCODE code);

	void Explode(Vector vecSrc, Vector vecAim);
	void Explode(TraceResult* pTrace, int bitsDamageType);
	void EXPORT Smoke(void);

	void EXPORT BounceTouch(CBaseEntity* pOther);
	void EXPORT SlideTouch(CBaseEntity* pOther);
	void EXPORT ExplodeTouch(CBaseEntity* pOther);
	void EXPORT DangerSoundThink(void);
	void EXPORT PreDetonate(void);
	void EXPORT Detonate(void);
	void EXPORT DetonateUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
	void EXPORT TumbleThink(void);

	virtual void BounceSound(void);
	virtual int BloodColor(void)
	{
		return DONT_BLEED;
	}
	virtual void Killed(entvars_t* pevAttacker, int iGib);

	inline unsigned char ExplodeSpriteScale() const
	{
		return m_iExplodeSpriteScale;
	}
	inline void SetExplodeSpriteScale(unsigned char val)
	{
		m_iExplodeSpriteScale = val;
	}
	inline float ExplosionRadius() const
	{
		return pev->fuser1;
	}
	inline void SetExplosionRadius(float radius)
	{
		pev->fuser1 = radius;
	}
	inline float OwnerDamageMultiplier() const
	{
		return pev->fuser2;
	}
	inline void SetOwnerDamageMultiplier(float multiplier)
	{
		pev->fuser2 = multiplier;
	}
	inline float PlayerContactDamageMultiplier() const
	{
		return pev->fuser3;
	}
	inline void SetPlayerContactDamageMultiplier(float multiplier)
	{
		pev->fuser3 = multiplier;
	}

	BOOL m_fRegisteredSound;  // whether or not this grenade has issued its DANGER sound to the world sound list yet.

private:
	unsigned char m_iExplodeSpriteScale;
};

//=========================================================
// CWeaponBox - a single entity that can store weapons
// and ammo.
//=========================================================
class CWeaponBox : public CBaseEntity
{
	void Precache(void);
	void Spawn(void);
	void Touch(CBaseEntity* pOther);
	void KeyValue(KeyValueData* pkvd);
	BOOL IsEmpty(void);
	int GiveAmmo(int iCount, const char* szName, int iMax, int* pIndex = NULL);
	void SetObjectCollisionBox(void);

public:
	void EXPORT Kill(void);
	int Save(CSave& save);
	int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];

	BOOL HasWeapon(CBasePlayerItem* pCheckItem);
	BOOL PackWeapon(CBasePlayerItem* pWeapon);
	BOOL PackAmmo(int iszName, int iCount);

	CBasePlayerItem* m_rgpPlayerItems[MAX_ITEM_TYPES];  // one slot for each

	string_t m_rgiszAmmo[MAX_AMMO_SLOTS];  // ammo names
	int m_rgAmmo[MAX_AMMO_SLOTS];  // ammo quantities

	int m_cAmmoTypes;  // how many ammo types packed into this box (if packed by a level designer)
};

#ifdef CLIENT_DLL
bool bIsMultiplayer(void);
void LoadVModel(const char* szViewModel, CBasePlayer* m_pPlayer);
#endif

extern void ClearMultiDamage(void);
extern void ApplyMultiDamage(entvars_t* pevInflictor, entvars_t* pevAttacker);
extern void AddMultiDamage(entvars_t* pevInflictor, CBaseEntity* pEntity, float flDamage, int bitsDamageType);

extern void DecalGunshot(TraceResult* pTrace, int iBulletType);
extern void SpawnBlood(Vector vecSpot, int bloodColor, float flDamage);
extern int DamageDecal(CBaseEntity* pEntity, int bitsDamageType);

typedef struct
{
	CBaseEntity* pEntity;
	float amount;
	int type;
} MULTIDAMAGE;

extern MULTIDAMAGE gMultiDamage;

#ifdef HL_CONTENT

#define WEAPON_CROWBAR 1
#define WEAPON_GLOCK 2
#define WEAPON_PYTHON 3
#define WEAPON_MP5 4
#define WEAPON_CHAINGUN 5
#define WEAPON_CROSSBOW 6
#define WEAPON_SHOTGUN 7
#define WEAPON_RPG 8
#define WEAPON_GAUSS 9
#define WEAPON_EGON 10
#define WEAPON_HORNETGUN 11
#define WEAPON_HANDGRENADE 12
#define WEAPON_TRIPMINE 13
#define WEAPON_SATCHEL 14
#define WEAPON_SNARK 15

// weapon weight factors (for auto-switching)   (-1 = noswitch)
#define CROWBAR_WEIGHT 0
#define GLOCK_WEIGHT 10
#define PYTHON_WEIGHT 15
#define MP5_WEIGHT 15
#define SHOTGUN_WEIGHT 15
#define CROSSBOW_WEIGHT 10
#define RPG_WEIGHT 20
#define GAUSS_WEIGHT 20
#define EGON_WEIGHT 20
#define HORNETGUN_WEIGHT 10
#define HANDGRENADE_WEIGHT 5
#define SNARK_WEIGHT 5
#define SATCHEL_WEIGHT -10
#define TRIPMINE_WEIGHT -10

// #define CROWBAR_MAX_CLIP		WEAPON_NOCLIP
#define GLOCK_MAX_CLIP 17
#define PYTHON_MAX_CLIP 6
#define MP5_MAX_CLIP 50
#define MP5_DEFAULT_AMMO 25
#define SHOTGUN_MAX_CLIP 8
#define CROSSBOW_MAX_CLIP 5
#define RPG_MAX_CLIP 1
#define GAUSS_MAX_CLIP WEAPON_NOCLIP
#define EGON_MAX_CLIP WEAPON_NOCLIP
#define HORNETGUN_MAX_CLIP WEAPON_NOCLIP
#define HANDGRENADE_MAX_CLIP WEAPON_NOCLIP
#define SATCHEL_MAX_CLIP WEAPON_NOCLIP
#define TRIPMINE_MAX_CLIP WEAPON_NOCLIP
#define SNARK_MAX_CLIP WEAPON_NOCLIP

// the default amount of ammo that comes with each gun when it spawns
#define GLOCK_DEFAULT_GIVE 17
#define PYTHON_DEFAULT_GIVE 6
#define MP5_DEFAULT_GIVE 25
#define MP5_DEFAULT_AMMO 25
#define MP5_M203_DEFAULT_GIVE 0
#define SHOTGUN_DEFAULT_GIVE 12
#define CROSSBOW_DEFAULT_GIVE 5
#define RPG_DEFAULT_GIVE 1
#define GAUSS_DEFAULT_GIVE 20
#define EGON_DEFAULT_GIVE 20
#define HANDGRENADE_DEFAULT_GIVE 5
#define SATCHEL_DEFAULT_GIVE 1
#define TRIPMINE_DEFAULT_GIVE 1
#define SNARK_DEFAULT_GIVE 5
#define HIVEHAND_DEFAULT_GIVE 8

// The amount of ammo given to a player by an ammo item.
#define AMMO_URANIUMBOX_GIVE 20
#define AMMO_GLOCKCLIP_GIVE GLOCK_MAX_CLIP
#define AMMO_357BOX_GIVE PYTHON_MAX_CLIP
#define AMMO_MP5CLIP_GIVE MP5_MAX_CLIP
#define AMMO_CHAINBOX_GIVE 200
#define AMMO_M203BOX_GIVE 2
#define AMMO_BUCKSHOTBOX_GIVE 12
#define AMMO_CROSSBOWCLIP_GIVE CROSSBOW_MAX_CLIP
#define AMMO_RPGCLIP_GIVE RPG_MAX_CLIP
#define AMMO_URANIUMBOX_GIVE 20
#define AMMO_SNARKBOX_GIVE 5

void DeactivateSatchels(CBasePlayer* pOwner);

enum glock_e
{
	GLOCK_IDLE1 = 0,
	GLOCK_IDLE2,
	GLOCK_IDLE3,
	GLOCK_SHOOT,
	GLOCK_SHOOT_EMPTY,
	GLOCK_RELOAD,
	GLOCK_RELOAD_NOT_EMPTY,
	GLOCK_DRAW,
	GLOCK_HOLSTER,
	GLOCK_ADD_SILENCER
};

class CGlock : public CBasePlayerWeapon
{
public:
	void Spawn(void);
	void Precache(void);
	int iItemSlot(void)
	{
		return 2;
	}
	int GetItemInfo(ItemInfo* p);
	int AddToPlayer(CBasePlayer* pPlayer);

	void PrimaryAttack(void);
	void SecondaryAttack(void);
	void GlockFire(float flSpread, float flCycleTime, BOOL fUseAutoAim);
	BOOL Deploy(void);
	void Reload(void);
	void WeaponIdle(void);

private:
	int m_iShell;

	unsigned short m_usFireGlock1;
	unsigned short m_usFireGlock2;
};

class CCrowbar : public CBasePlayerWeapon
{
public:
	void Spawn(void);
	void Precache(void);
	int iItemSlot(void)
	{
		return 1;
	}
	void EXPORT SwingAgain(void);
	void EXPORT Smack(void);
	int GetItemInfo(ItemInfo* p);
	int AddToPlayer(CBasePlayer* pPlayer);

	void PrimaryAttack(void);
	int Swing(int fFirst);
	BOOL Deploy(void);
	void Holster(int skiplocal = 0);
#ifdef CROWBAR_IDLE_ANIM
	void WeaponIdle();
#endif
	int m_iSwing;
	TraceResult m_trHit;

private:
	unsigned short m_usCrowbar;
};

enum python_e
{
	PYTHON_IDLE1 = 0,
	PYTHON_FIDGET,
	PYTHON_FIRE1,
	PYTHON_RELOAD,
	PYTHON_HOLSTER,
	PYTHON_DRAW,
	PYTHON_IDLE2,
	PYTHON_IDLE3
};

class CPython : public CBasePlayerWeapon
{
public:
	void Spawn(void);
	void Precache(void);
	int iItemSlot(void)
	{
		return 2;
	}
	int GetItemInfo(ItemInfo* p);
	int AddToPlayer(CBasePlayer* pPlayer);
	void PrimaryAttack(void);
	void SecondaryAttack(void);
	BOOL Deploy(void);
	void Holster(int skiplocal = 0);
	void Reload(void);
	void WeaponIdle(void);
	float m_flSoundDelay;

	BOOL m_fInZoom;  // don't save this.

private:
	unsigned short m_usFirePython;
};

enum mp5_e
{
	MP5_LONGIDLE = 0,
	MP5_IDLE1,
	MP5_LAUNCH,
	MP5_RELOAD,
	MP5_DEPLOY,
	MP5_FIRE1,
	MP5_FIRE2,
	MP5_FIRE3
};

class CMP5 : public CBasePlayerWeapon
{
public:
	void Spawn(void);
	void Precache(void);
	int iItemSlot(void)
	{
		return 3;
	}
	int GetItemInfo(ItemInfo* p);
	int AddToPlayer(CBasePlayer* pPlayer);

	void PrimaryAttack(void);
	void SecondaryAttack(void);
	int SecondaryAmmoIndex(void);
	BOOL Deploy(void);
	void Reload(void);
	void WeaponIdle(void);
	BOOL IsUseable();
	float m_flNextAnimTime;
	int m_iShell;

private:
	unsigned short m_usMP5;
	unsigned short m_usMP52;
};

class CCrossbow : public CBasePlayerWeapon
{
public:
	void Spawn(void);
	void Precache(void);
	int iItemSlot()
	{
		return 3;
	}
	int GetItemInfo(ItemInfo* p);

	void FireBolt(void);
	void FireSniperBolt(void);
	void PrimaryAttack(void);
	void SecondaryAttack(void);
	int AddToPlayer(CBasePlayer* pPlayer);
	BOOL Deploy();
	void Holster(int skiplocal = 0);
	void Reload(void);
	void WeaponIdle(void);

	int m_fInZoom;  // don't save this

private:
	unsigned short m_usCrossbow;
	unsigned short m_usCrossbow2;
};

enum shotgun_e
{
	SHOTGUN_IDLE = 0,
	SHOTGUN_FIRE,
	SHOTGUN_FIRE2,
	SHOTGUN_RELOAD,
	SHOTGUN_PUMP,
	SHOTGUN_START_RELOAD,
	SHOTGUN_DRAW,
	SHOTGUN_HOLSTER,
	SHOTGUN_IDLE4,
	SHOTGUN_IDLE_DEEP
};

class CShotgun : public CBasePlayerWeapon
{
public:
#ifndef CLIENT_DLL
	int Save(CSave& save);
	int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];
#endif
	void Spawn(void);
	void Precache(void);
	int iItemSlot()
	{
		return 3;
	}
	int GetItemInfo(ItemInfo* p);
	int AddToPlayer(CBasePlayer* pPlayer);

	void PrimaryAttack(void);
	void SecondaryAttack(void);
	BOOL Deploy();
	void Reload(void);
	void WeaponTick();
	void WeaponIdle(void);
	int m_fInReload;
	float m_flNextReload;
	int m_iShell;

private:
	float m_flPumpTime;
	unsigned short m_usDoubleFire;
	unsigned short m_usSingleFire;
};

class CLaserSpot : public CBaseEntity
{
	void Spawn(void);
	void Precache(void);

	int ObjectCaps(void)
	{
		return FCAP_DONT_SAVE;
	}

public:
	void Suspend(float flSuspendTime);
	void EXPORT Revive(void);

	static CLaserSpot* CreateSpot(void);
};

enum rpg_e
{
	RPG_IDLE = 0,
	RPG_FIDGET,
	RPG_RELOAD,  // to reload
	RPG_FIRE2,  // to empty
	RPG_HOLSTER1,  // loaded
	RPG_DRAW1,  // loaded
	RPG_HOLSTER2,  // unloaded
	RPG_DRAW_UL,  // unloaded
	RPG_IDLE_UL,  // unloaded idle
	RPG_FIDGET_UL  // unloaded fidget
};

class CRpg : public CBasePlayerWeapon
{
public:
#ifndef CLIENT_DLL
	int Save(CSave& save);
	int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];
#endif
	void Spawn(void);
	void Precache(void);
	void Reload(void);
	int iItemSlot(void)
	{
		return 4;
	}
	int GetItemInfo(ItemInfo* p);
	int AddToPlayer(CBasePlayer* pPlayer);

	BOOL Deploy(void);
	BOOL CanHolster(void);
	void Holster(int skiplocal = 0);

	void PrimaryAttack(void);
	void SecondaryAttack(void);
	void WeaponIdle(void);

	void UpdateSpot(void);
	BOOL ShouldWeaponIdle(void)
	{
		return TRUE;
	};

	CLaserSpot* m_pSpot;
	int m_fSpotActive;
	int m_cActiveRockets;  // how many missiles in flight from this launcher right now?

private:
	unsigned short m_usRpg;
};

class CRpgRocket : public CGrenade
{
public:
	int Save(CSave& save);
	int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];
	void Spawn(void);
	void Precache(void);
	void EXPORT FollowThink(void);
	void EXPORT IgniteThink(void);
	void EXPORT RocketTouch(CBaseEntity* pOther);
	static CRpgRocket* CreateRpgRocket(Vector vecOrigin, Vector vecAngles, CBaseEntity* pOwner, CRpg* pLauncher);

	int m_iTrail;
	float m_flIgniteTime;
	EHANDLE m_hLauncher;  // handle back to the launcher that fired me.
};

#define GAUSS_PRIMARY_CHARGE_VOLUME 256  // how loud gauss is while charging
#define GAUSS_PRIMARY_FIRE_VOLUME 450  // how loud gauss is when discharged

enum gauss_e
{
	GAUSS_IDLE = 0,
	GAUSS_IDLE2,
	GAUSS_FIDGET,
	GAUSS_SPINUP,
	GAUSS_SPIN,
	GAUSS_FIRE,
	GAUSS_FIRE2,
	GAUSS_HOLSTER,
	GAUSS_DRAW
};

class CGauss : public CBasePlayerWeapon
{
public:
#ifndef CLIENT_DLL
	int Save(CSave& save);
	int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];
#endif
	void Spawn(void);
	void Precache(void);
	int iItemSlot(void)
	{
		return 4;
	}
	int GetItemInfo(ItemInfo* p);
	int AddToPlayer(CBasePlayer* pPlayer);
	BOOL IsUseable();
	BOOL Deploy(void);
	void Holster(int skiplocal = 0);

	void PrimaryAttack(void);
	void SecondaryAttack(void);
	void WeaponIdle(void);

	void StartFire(void);
	void Fire(Vector vecOrigSrc, Vector vecDirShooting, float flDamage);
	float GetFullChargeTime(void);
	int m_iBalls;
	int m_iGlow;
	int m_iBeam;
	int m_iSoundState;  // don't save this

	// was this weapon just fired primary or secondary?
	// we need to know so we can pick the right set of effects.
	BOOL m_fPrimaryFire;

	bool ReadPredictionData(const weapon_data_t* from);
	bool WritePredictionData(weapon_data_t* to);

private:
	unsigned short m_usGaussFire;
	unsigned short m_usGaussSpin;
};

class CEgon : public CBasePlayerWeapon
{
public:
#ifndef CLIENT_DLL
	int Save(CSave& save);
	int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];
#endif
	void Spawn(void);
	void Precache(void);
	int iItemSlot(void)
	{
		return 4;
	}
	int GetItemInfo(ItemInfo* p);
	int AddToPlayer(CBasePlayer* pPlayer);

	BOOL Deploy(void);
	void Holster(int skiplocal = 0);

	void UpdateEffect(const Vector& startPoint, const Vector& endPoint, float timeBlend);

	void CreateEffect(void);
	void DestroyEffect(void);

	void EndAttack(void);
	void Attack(void);
	void PrimaryAttack(void);
	void WeaponIdle(void);

	float m_flAmmoUseTime;  // since we use < 1 point of ammo per update, we subtract ammo on a timer.

	float GetPulseInterval(void);
	float GetDischargeInterval(void);

	void Fire(const Vector& vecOrigSrc, const Vector& vecDir);

	BOOL HasAmmo(void);

	void UseAmmo(int count);

	enum EGON_FIREMODE
	{
		FIRE_NARROW,
		FIRE_WIDE
	};

	CBeam* m_pBeam;
	CBeam* m_pNoise;
	CSprite* m_pSprite;

	unsigned short m_usEgonStop;

	bool ReadPredictionData(const weapon_data_t* from);
	bool WritePredictionData(weapon_data_t* to);

private:
#ifndef CLIENT_DLL
	float m_shootTime;
#endif
	EGON_FIREMODE m_fireMode;
	float m_shakeTime;
	BOOL m_deployed;

	unsigned short m_usEgonFire;
};

class CHgun : public CBasePlayerWeapon
{
public:
#ifndef CLIENT_DLL
	int Save(CSave& save);
	int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];
#endif
	void Spawn(void);
	void Precache(void);
	int iItemSlot(void)
	{
		return 4;
	}
	int GetItemInfo(ItemInfo* p);
	int AddToPlayer(CBasePlayer* pPlayer);

	void PrimaryAttack(void);
	void SecondaryAttack(void);
	BOOL Deploy(void);
	BOOL IsUseable(void);
	void Holster(int skiplocal = 0);
	void Reload(void);
	void WeaponIdle(void);
	float m_flNextAnimTime;

	float m_flRechargeTime;

	int m_iFirePhase;  // don't save me.
private:
	unsigned short m_usHornetFire;
};

class CHandGrenade : public CBasePlayerWeapon
{
public:
	void Spawn(void);
	void Precache(void);
	int iItemSlot(void)
	{
		return 5;
	}
	int GetItemInfo(ItemInfo* p);

	void PrimaryAttack(void);
	BOOL Deploy(void);
	BOOL CanHolster(void);
	void Holster(int skiplocal = 0);
	void WeaponIdle(void);

	bool ReadPredictionData(const weapon_data_t* from);
	bool WritePredictionData(weapon_data_t* to);
};

class CSatchel : public CBasePlayerWeapon
{
public:
#ifndef CLIENT_DLL
	int Save(CSave& save);
	int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];
#endif
	void Spawn(void);
	void Precache(void);
	int iItemSlot(void)
	{
		return 5;
	}
	int GetItemInfo(ItemInfo* p);
	int AddToPlayer(CBasePlayer* pPlayer);
	void PrimaryAttack(void);
	void SecondaryAttack(void);
	int AddDuplicate(CBasePlayerItem* pOriginal);
	BOOL CanDeploy(void);
	BOOL Deploy(void);
	BOOL IsUseable(void);

	void Holster(int skiplocal = 0);
	void WeaponIdle(void);
	void Throw(void);

	bool ReadPredictionData(const weapon_data_t* from);
	bool WritePredictionData(weapon_data_t* to);
};

class CTripmine : public CBasePlayerWeapon
{
public:
	void Spawn(void);
	void Precache(void);
	int iItemSlot(void)
	{
		return 5;
	}
	int GetItemInfo(ItemInfo* p);
	void SetObjectCollisionBox(void)
	{
		//!!!BUGBUG - fix the model!
		(Vector(pev->origin) + Vector(-16, -16, -5)).CopyToArray(pev->absmin);
		(Vector(pev->origin) + Vector(16, 16, 28)).CopyToArray(pev->absmax);
	}

	void PrimaryAttack(void);
	BOOL Deploy(void);
	void Holster(int skiplocal = 0);
	void WeaponIdle(void);

private:
	unsigned short m_usTripFire;
};

class CSqueak : public CBasePlayerWeapon
{
public:
	void Spawn(void);
	void Precache(void);
	int iItemSlot(void)
	{
		return 5;
	}
	int GetItemInfo(ItemInfo* p);

	void PrimaryAttack(void);
	void SecondaryAttack(void);
	BOOL Deploy(void);
	void Holster(int skiplocal = 0);
	void WeaponIdle(void);
	int m_fJustThrown;

private:
	unsigned short m_usSnarkFire;
};

#endif  // HL_CONTENT
