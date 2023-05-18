#pragma once

#include "BaseResourceCollection.h"

enum class SurfaceDecalId
{
	None = 0,
	Generic,
	Carpet,
	Dirt,
	Glass,
	BulletproofGlass,
	Grass,
	Metal,
	Gold,
	Plaster,
	Sand,
	Scorch,
	Snow,
	Stone,
	Wood,
	Count
};

class CSurfaceDecalResources :
	public CBaseResourceCollection<SurfaceDecalId, static_cast<size_t>(SurfaceDecalId::Count)>
{
public:
	struct Attributes
	{
		float scale = 1.0f;  // Higher = smaller
	};

	CSurfaceDecalResources();

	inline const Attributes& GetAttributes(SurfaceDecalId id) const
	{
		const uint32_t index = static_cast<uint32_t>(id);
		return m_ExtraAttributes[index < BaseClass::NUM_ENTRIES ? index : 0];
	}

private:
	using BaseClass = CBaseResourceCollection<SurfaceDecalId, static_cast<size_t>(SurfaceDecalId::Count)>;

	inline Attributes& Atts(SurfaceDecalId id)
	{
		const uint32_t index = static_cast<uint32_t>(id);
		return m_ExtraAttributes[index < BaseClass::NUM_ENTRIES ? index : 0];
	}

	Attributes m_ExtraAttributes[BaseClass::NUM_ENTRIES];
};
