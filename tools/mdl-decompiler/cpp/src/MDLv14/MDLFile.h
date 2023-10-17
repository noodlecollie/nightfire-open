#pragma once

#include <vector>
#include "MDLv14/Components.h"
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
	};
}
