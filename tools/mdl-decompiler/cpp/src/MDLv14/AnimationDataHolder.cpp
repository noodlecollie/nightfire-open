#include <stdexcept>
#include "MDLv14/AnimationDataHolder.h"

namespace MDLv14
{
	AnimationDataHolder::AnimationDataHolder()
	{
	}

	AnimationDataHolder::AnimationDataHolder(size_t blendCount, size_t boneCount) :
		m_BlendCount(blendCount),
		m_BoneCount(boneCount)
	{
		if ( m_BlendCount < 1 || m_BoneCount < 1 )
		{
			throw std::invalid_argument("AnimationDataHolder: Number of blends and bones must be greater than zero.");
		}

		Clear();
	}

	size_t AnimationDataHolder::BlendCount() const
	{
		return m_BlendCount;
	}

	size_t AnimationDataHolder::BoneCount() const
	{
		return m_BoneCount;
	}

	bool AnimationDataHolder::IsValid() const
	{
		return m_BlendCount > 0 && m_BoneCount > 0;
	}

	void AnimationDataHolder::Clear()
	{
		m_DataArrays.clear();

		if ( IsValid() )
		{
			m_DataArrays.resize(m_BlendCount * m_BoneCount * AXIS_COUNT);
		}
	}

	std::vector<int16_t>& AnimationDataHolder::GetDataArray(size_t blendIndex, size_t boneIndex, size_t axisIndex)
	{
		return m_DataArrays[LinearIndex(blendIndex, boneIndex, axisIndex)];
	}

	const std::vector<int16_t>&
	AnimationDataHolder::GetDataArray(size_t blendIndex, size_t boneIndex, size_t axisIndex) const
	{
		return m_DataArrays[LinearIndex(blendIndex, boneIndex, axisIndex)];
	}

	size_t AnimationDataHolder::LinearIndex(size_t blendIndex, size_t boneIndex, size_t axisIndex) const
	{
		if ( blendIndex >= m_BlendCount )
		{
			throw std::invalid_argument("AnimationDataHolder: Blend index was out of range.");
		}

		if ( boneIndex >= m_BoneCount )
		{
			throw std::invalid_argument("AnimationDataHolder: Bone index was out of range.");
		}

		if ( axisIndex >= AXIS_COUNT )
		{
			throw std::invalid_argument("AnimationDataHolder: Axis index was out of range.");
		}

		// There are this many indices per block when indexing by bone.
		const size_t boneBlockSize = AXIS_COUNT;

		// There are this many indices per block when indexing by blend.
		const size_t blendBlockSize = m_BoneCount * boneBlockSize;

		return (blendIndex * blendBlockSize) + (boneIndex * boneBlockSize) + axisIndex;
	}
}  // namespace MDLv14
