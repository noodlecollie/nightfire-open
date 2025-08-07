#pragma once

namespace WeaponAtts
{
	struct AccuracyParameters;
}

typedef struct cvar_s cvar_t;

class CBasePlayer;

class CWeaponInaccuracyCalculator
{
public:
	using Accuracy = WeaponAtts::AccuracyParameters;

	CWeaponInaccuracyCalculator();

	void Clear();
	void CalculateInaccuracy();
	void AddInaccuracyPenaltyFromFiring();

	float InstantaneousInaccuracy() const;
	float SmoothedInaccuracy() const;
	void SetLastSmoothedInaccuracy(float inaccuracy);

	const Accuracy* AccuracyParams() const;
	void SetAccuracyParams(const Accuracy* params);

	CBasePlayer* Player() const;
	void SetPlayer(CBasePlayer* player);

	void SetFiredThisFrame(bool fired);
	void SetLastFireTime(float time);
	void SetLastFireTimeIsDecremented(bool decremented);

private:
	static void InitCvars();

	const WeaponAtts::AccuracyParameters* GetInternalParameters() const;
	bool IsValid() const;
	void CalculateInstantaneousInaccuracy(const WeaponAtts::AccuracyParameters* params);
	void CalculateSmoothedInaccuracy(const WeaponAtts::AccuracyParameters* params);
	bool LastFireTimeIsInHoldRegion(float holdTime) const;
	float DecayModulatorAfterFireImpulse(const WeaponAtts::AccuracyParameters& params) const;
	float TimeSinceLastFire() const;

	static bool m_CvarsLoaded;
	static cvar_t* m_CvarMaxSpeed;
	static cvar_t* m_CvarMaxFallSpeed;
	static WeaponAtts::AccuracyParameters m_StaticDebugParams;

	// These variables don't have defaults set here, because they
	// are set to their defaults in Clear() instead.
	float m_InstantaneousInaccuracy;
	float m_SmoothedInaccuracy;
	float m_LastSmoothedInaccuracy;
	bool m_FiredThisFrame;
	float m_LastFireTime;
	bool m_LastFireTimeIsDecremented;
	const WeaponAtts::AccuracyParameters* m_AccuracyParams;
	CBasePlayer* m_Player;
};
