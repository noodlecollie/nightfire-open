#pragma once

#include <vector>
#include "Common/Container.h"
#include "MDLv14/Components.h"
#include "MDLv14/SkinDataHolder.h"
#include "BufferedFile.h"

namespace MDLv14
{
	class MDLFile
	{
	public:
		explicit MDLFile(const BufferedFile& file);

		void Validate() const;

		// Without the .mdl extension
		std::string ModelName() const;

		const Header& GetHeader() const;
		const Container<Attachment>& GetAttachments() const;
		const Container<Bone>& GetBones() const;
		const Container<BodyGroup>& GetBodyGroups() const;

		const Model* FindModelByOffset(int32_t offset) const;
		const Model* FindModelByName(const std::string& name) const;

	private:
		void ValidateBeforeRead(BufferedFileReader::Ref ref) const;

		Header m_Header {};
		Container<Bone> m_Bones;
		Container<BoneController> m_BoneControllers;
		Container<HitBox> m_HitBoxes;
		Container<Sequence> m_Sequences;
		Container<SequenceGroup> m_SequenceGroups;
		Container<LevelOfDetail> m_LevelsOfDetail;
		Container<Texture> m_Textures;
		SkinDataHolder m_SkinData;
		Container<BodyGroup> m_BodyGroups;
		Container<Attachment> m_Attachments;
		Container<SoundGroup> m_SoundGroups;
		Container<TriangleMap> m_TriangleMaps;
		Container<Vertex> m_Vertices;
		Container<Normal> m_Normals;
		Container<TextureCoOrdinate> m_TextureCoOrdinates;
		Container<BlendingScales> m_BlendingScales;
		Container<Blending> m_Blends;
		Container<BoneFixUp> m_BoneFixUps;
		Container<Model> m_Models;
	};
}
