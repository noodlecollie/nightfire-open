#pragma once

#include "baseweaponeventplayer.h"
#include "weaponatts_hitscanattack.h"

class HitscanWeaponEventPlayer : public BaseWeaponEventPlayer
{
public:
	virtual ~HitscanWeaponEventPlayer()
	{
	}

protected:
	virtual void EventStart() override;
	virtual bool Initialise() override;
	virtual void ParseEventScript(const rapidjson::Document& document) override;

private:
	void CreateBulletTracers();

	const WeaponAtts::WAHitscanAttack* m_pHitscanAttack = nullptr;
	int m_iShellModelIndex = -1;
	int m_iRandomSeed = 0;
	float m_flSpreadInterp = 0.0f;
	int m_iTracerStride = 1;
	uint32_t m_iShotsFired = 0;
};
