#pragma once

#include <vector>
#include "MDLv14/Components.h"
#include "MDLv14/SkinDataHolder.h"
#include "BufferedFile.h"

namespace MDLv14
{
	class MDLFile
	{
	public:
		explicit MDLFile(const BufferedFile& file);

	private:
		Header m_Header {};
		std::vector<Bone> m_Bones;
		std::vector<BoneController> m_BoneControllers;
		std::vector<HitBox> m_HitBoxes;
		std::vector<Sequence> m_Sequences;
		std::vector<SequenceGroup> m_SequenceGroups;
		std::vector<LevelOfDetail> m_LevelsOfDetail;
		std::vector<Texture> m_Textures;
		SkinDataHolder m_SkinData;
	};
}
