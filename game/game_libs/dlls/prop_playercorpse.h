#pragma once

#include "standard_includes.h"

class CPropPlayerCorpse : public CBaseAnimating
{
public:
	static void Create(entvars_t* other);

	virtual void Spawn() override;
	void Initialise(entvars_t* other);

private:
	void Think();
	void SetMultiplayerModel(entvars_t* other);

	float m_flInitTime = 0.0f;
};
