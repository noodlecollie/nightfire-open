#pragma once

#include "utlstring.h"
#include "rapidjson/document.h"
#include "weaponatts_baseattack.h"
#include "resources/ShellImpactSoundResources.h"

typedef struct event_args_s event_args_t;
class CGenericWeaponAtts_HitscanFireMechanic;

class BaseWeaponEventPlayer
{
public:
	virtual ~BaseWeaponEventPlayer() {}

	void LoadEventScript(const CUtlString& path);
	void PlayEvent(const event_args_t* eventArgs,
				   const WeaponAtts::WABaseAttack::AttackModeSignature* signature);

protected:
	virtual void EventStart() = 0;
	virtual bool Initialise();
	virtual void ParseEventScript(const rapidjson::Document& document);

	void AnimateViewModel();
	void EjectShellFromViewModel(int shellIndex, ShellType shellType);
	void PlayFireSound();

	const event_args_t* m_pEventArgs = nullptr;
	const WeaponAtts::WABaseAttack::AttackModeSignature* m_pSignature = nullptr;
	const WeaponAtts::WABaseAttack* m_pAttackMode = nullptr;

	int m_iEntIndex = -1;
	bool m_bWeaponIsEmpty = false;

	vec3_t m_vecEntAngles;
	vec3_t m_vecEntOrigin;
	vec3_t m_vecEntVelocity;
	vec3_t m_vecFwd;
	vec3_t m_vecRight;
	vec3_t m_vecUp;
	vec3_t m_vecGunPosition;
};
