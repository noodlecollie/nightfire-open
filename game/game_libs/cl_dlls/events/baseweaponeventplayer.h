#pragma once

#include "MathLib/vec3.h"
#include "utlstring.h"
#include "rapidjson/document.h"
#include "weaponatts_baseattack.h"
#include "resources/ShellImpactSoundResources.h"

typedef struct event_args_s event_args_t;

class BaseWeaponEventPlayer
{
public:
	virtual ~BaseWeaponEventPlayer()
	{
	}

	void LoadEventScript(const CUtlString& path);
	void PlayEvent(const event_args_t* eventArgs, const WeaponAtts::WABaseAttack::AttackModeSignature* signature);

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

	Vector m_vecEntAngles;
	Vector m_vecEntOrigin;
	Vector m_vecEntVelocity;
	Vector m_vecFwd;
	Vector m_vecRight;
	Vector m_vecUp;
	Vector m_vecGunPosition;
};
