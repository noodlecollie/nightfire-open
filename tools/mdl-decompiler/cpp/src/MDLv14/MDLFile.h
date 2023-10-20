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

		const Header& GetHeader() const;

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
		std::vector<BodyGroup> m_BodyGroups;
		std::vector<Attachment> m_Attachments;
		std::vector<SoundGroup> m_SoundGroups;
		std::vector<TriangleMap> m_TriangleMaps;
		std::vector<Vertex> m_Vertices;
		std::vector<Normal> m_Normals;
		std::vector<TextureCoOrdinate> m_TextureCoOrdinates;
		std::vector<BlendingScales> m_BlendingScales;
	};
}
