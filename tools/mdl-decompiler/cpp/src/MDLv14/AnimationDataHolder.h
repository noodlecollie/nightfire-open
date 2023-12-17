#pragma once

#include <cstdint>
#include <vector>

namespace MDLv14
{
	class AnimationDataHolder
	{
	public:
		static constexpr size_t AXIS_COUNT = 6;

		AnimationDataHolder();
		AnimationDataHolder(size_t blendCount, size_t boneCount);

		size_t BlendCount() const;
		size_t BoneCount() const;
		bool IsValid() const;
		void Clear();

		std::vector<int16_t>& GetDataArray(size_t blendIndex, size_t boneIndex, size_t axisIndex);
		const std::vector<int16_t>& GetDataArray(size_t blendIndex, size_t boneIndex, size_t axisIndex) const;

	private:
		size_t LinearIndex(size_t blendIndex, size_t boneIndex, size_t axisIndex) const;

		size_t m_BlendCount = 0;
		size_t m_BoneCount = 0;
		std::vector<std::vector<int16_t>> m_DataArrays;
	};
}
