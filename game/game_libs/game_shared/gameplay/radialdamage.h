#pragma once

#include "MathLib/vec3.h"
#include "EnginePublicAPI/eiface.h"

typedef struct entvars_s entvars_t;
class CBaseEntity;

class CRadialDamageInflictor
{
public:
	CRadialDamageInflictor();

	Vector Origin() const;
	void SetOrigin(const Vector& origin);

	entvars_t* Inflictor() const;
	void SetInflictor(entvars_t* inflictor);

	entvars_t* Attacker() const;
	void SetAttacker(entvars_t* attacker);

	float Damage() const;
	void SetDamage(float damage);

	float Radius() const;
	void SetRadius(float radius);

	int IgnoreClass() const;
	void SetIgnoreClass(int ignoreClass);

	int DamageTypeBits() const;
	void SetDamageTypeBits(int bits);

	float AttackerDamageMultiplier() const;
	void SetAttackerDamageMultiplier(float multiplier);

	// Damage is only applied server-side. On the client,
	// this compiles to a stub.
	void ApplyDamage();

private:
#ifndef CLIENT_DLL
	void DamageEntityFromSphere(CBaseEntity& target);
	void ApplyDamageToEntity(CBaseEntity& target);
	float GetAdjustedDamage(CBaseEntity& target, const Vector& endPos) const;
#endif

	// Attributes:
	Vector m_vecOrigin;
	entvars_t* m_pInflictor = NULL;
	entvars_t* m_pAttacker = NULL;
	float m_flDamage = 0.0f;
	float m_flRadius = 0.0f;
	int m_iIgnoreClass = 0;
	int m_bitsDamageType = 0;
	float m_flAttackerDamageMultiplier = 1.0f;

	// Working variables when applying damage:
	float m_flWorkingFalloff = 0.0f;
	entvars_t* m_pWorkingAttacker = NULL;
	bool m_bWorkingInWater = false;
	TraceResult m_TraceResult;
};
