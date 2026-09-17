#pragma once

#include "standard_includes.h"

class CObserverPoint : public CPointEntity
{
public:
	static constexpr const char* const CLASSNAME = "info_observer_point";
	static TYPEDESCRIPTION m_SaveData[];

	int Save(CSave& save) override;
	int Restore(CRestore& restore) override;
	void Precache() override;
	void KeyValue(KeyValueData* pkvd) override;
	void Spawn() override;

	int Index() const;

private:
	int m_index = 0;
};
