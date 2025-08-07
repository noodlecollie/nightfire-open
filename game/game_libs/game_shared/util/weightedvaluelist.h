#pragma once

#include <cstdint>
#include "utlvector.h"

template<typename T>
class WeightedValueList
{
public:
	uint32_t Add(const T& value, float weight = 1.0f, bool normalise = true)
	{
		m_Items.AddToTail({value, weight, 0.0f});

		if ( normalise )
		{
			NormaliseWeights();
		}

		return m_Items.Count();
	}

	// Convenience for adding items with equal chance to be chosen.
	WeightedValueList<T>& operator<<(const T& value)
	{
		Add(value, 1.0f, true);
		return *this;
	}

	const T& Value(uint32_t index) const
	{
		ASSERTSZ_Q((int)index >= 0 && (int)index < m_Items.Count(), "Index must be in range.");
		return m_Items[index].m_Value;
	}

	// Given a float between 0 and 1 (assumed to be randomly generated),
	// returns the index of a value taking into account all weights.
	// Assumes NormaliseWeight() has been called.
	uint32_t IndexByProbabilisticValue(float input) const
	{
		float accumulatedWeight = 0;

		for ( int index = 0; index < m_Items.Count(); ++index )
		{
			// accumulatedWeight is the lower bound in the [0 1] range,
			// and this + the item's normalised weight is the upper bound.
			// If the input is within these bounds, this it the item we choose.
			if ( accumulatedWeight + m_Items[index].m_NormalisedWeight >= input )
			{
				return index;
			}

			accumulatedWeight += m_Items[index].m_NormalisedWeight;
		}

		// Unlikely to happen - probably the result of rounding errors.
		return m_Items.Count() - 1;
	}

	// As above, but returns the item pointed to by the index.
	const T& ItemByProbabilisticValue(float input) const
	{
		ASSERTSZ(m_Items.Count() > 0, "Container must not be empty.");
		return m_Items[IndexByProbabilisticValue(input)].m_Value;
	}

	float OriginalWeight(uint32_t index) const
	{
		ASSERTSZ(index < m_Items.Count(), "Index must be in range.");
		return m_Items[index].m_Weight;
	}

	float NormalisedWeight(uint32_t index) const
	{
		ASSERTSZ(index < m_Items.Count(), "Index must be in range.");
		return m_Items[index].m_NormalisedWeight;
	}

	size_t Count() const
	{
		return m_Items.Count();
	}

	void Clear()
	{
		m_Items.Purge();
	}

	void NormaliseWeights()
	{
		// We calculate this each time, because if we kept track of it
		// on addition/deletion floating point errors could accumulate.
		float totalWeight = 0.0f;

		for ( int index = 0; index < m_Items.Count(); ++index )
		{
			totalWeight += m_Items[index].m_Weight;
		}

		for ( int index = 0; index < m_Items.Count(); ++index )
		{
			m_Items[index].m_NormalisedWeight = m_Items[index].m_Weight / totalWeight;
		}
	}

private:
	struct WeightedValue
	{
		T m_Value;
		float m_Weight;
		float m_NormalisedWeight;
	};

	CUtlVector<WeightedValue> m_Items;
};
