#pragma once

#include "standard_includes.h"

namespace HitscanWeaponDebugging
{
	void Clear();
	void BeginBatch();
	void AddTraceToBatch(const Vector& begin, const Vector& end);
	void EndBatch();
};  // namespace HitscanWeaponDebugging
