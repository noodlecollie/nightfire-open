#include <cctype>
#include <algorithm>
#include "MDLv14/MDLFile.h"
#include "BufferedFileReader.h"
#include "MDLv14/ComponentReader.h"
#include "MDLv14/ComponentUtils.h"
#include "Exceptions.h"
#include "Utils.h"

namespace MDLv14
{
	MDLFile::MDLFile(const BufferedFile& file)
	{
		BufferedFileReader fileReader = file.CreateReader();
		ValidateBeforeRead(BufferedFileReader::Ref(fileReader));

		ComponentReader componentReader;

		m_Header = componentReader.ReadComponent<Header>(fileReader);

		m_Bones = componentReader.ReadComponentArray<Bone>(
			fileReader,
			IntToSizeT(m_Header.bones.count),
			IntToSizeT(m_Header.bones.offset));

		m_BoneControllers = componentReader.ReadComponentArray<BoneController>(
			fileReader,
			IntToSizeT(m_Header.boneControllers.count),
			IntToSizeT(m_Header.boneControllers.offset));

		m_HitBoxes = componentReader.ReadComponentArray<HitBox>(
			fileReader,
			IntToSizeT(m_Header.hitBoxes.count),
			IntToSizeT(m_Header.hitBoxes.offset));

		m_Sequences = componentReader.ReadComponentArray<Sequence>(
			fileReader,
			IntToSizeT(m_Header.sequences.count),
			IntToSizeT(m_Header.sequences.offset));

		for ( Sequence& sequence : m_Sequences )
		{
			sequence.eventCollection = componentReader.ReadComponentArray<Event>(
				fileReader,
				IntToSizeT(sequence.events.count),
				IntToSizeT(sequence.events.offset));
		}

		for ( Sequence& sequence : m_Sequences )
		{
			sequence.pivotCollection = componentReader.ReadComponentArray<Pivot>(
				fileReader,
				IntToSizeT(sequence.pivots.count),
				IntToSizeT(sequence.pivots.offset));
		}

		for ( Sequence& sequence : m_Sequences )
		{
			fileReader.SeekFromBeginning(IntToSizeT(sequence.animationOffset));

			sequence.animationCollection = componentReader.ReadAnimationData(
				BufferedFileReader::Ref(fileReader),
				IntToSizeT(sequence.blendCount),
				IntToSizeT(m_Header.bones.count),
				sequence.frameCount);
		}

		m_SequenceGroups = componentReader.ReadComponentArray<SequenceGroup>(
			fileReader,
			IntToSizeT(m_Header.sequenceGroups.count),
			IntToSizeT(m_Header.sequenceGroups.offset));

		if ( m_Header.levelOfDetailFlags != LevelOfDetailFlag_None )
		{
			fileReader.SeekForward(DistToNextMultiple(fileReader.CurrentPosition(), 16));

			m_LevelsOfDetail = componentReader.ReadComponentArray<LevelOfDetail>(
				fileReader,
				MDLv14::LODFlagsToLODLevels(m_Header.levelOfDetailFlags));
		}

		m_Textures = componentReader.ReadComponentArray<Texture>(
			fileReader,
			IntToSizeT(m_Header.textures.count),
			IntToSizeT(m_Header.textures.offset));

		fileReader.SeekFromBeginning(m_Header.skinsOffset);
		m_SkinData = componentReader.ReadSkinData(fileReader, m_Header.skinFamilies, m_Header.skinReferences);

		m_BodyGroups = componentReader.ReadComponentArray<BodyGroup>(
			fileReader,
			IntToSizeT(m_Header.bodyGroups.count),
			IntToSizeT(m_Header.bodyGroups.offset));

		m_Attachments = componentReader.ReadComponentArray<Attachment>(
			fileReader,
			IntToSizeT(m_Header.attachments.count),
			IntToSizeT(m_Header.attachments.offset));

		m_SoundGroups = componentReader.ReadComponentArray<SoundGroup>(
			fileReader,
			IntToSizeT(m_Header.soundGroups.count),
			IntToSizeT(m_Header.soundGroups.offset));

		const size_t endOfSoundGroupLump = fileReader.CurrentPosition();

		for ( SoundGroup& soundGroup : m_SoundGroups )
		{
			fileReader.SeekFromBeginning(endOfSoundGroupLump + IntToSizeT(soundGroup.offset));
			soundGroup.sounds = componentReader.ReadComponent<Sounds>(fileReader);
		}

		m_TriangleMaps = componentReader.ReadComponentArray<TriangleMap>(
			fileReader,
			IntToSizeT(m_Header.triangleCount),
			IntToSizeT(m_Header.triangleMapOffset));

		m_Vertices = componentReader.ReadComponentArray<Vertex>(
			fileReader,
			IntToSizeT(m_Header.vertexCount),
			IntToSizeT(m_Header.verticesOffset));

		m_Normals = componentReader.ReadComponentArray<Normal>(
			fileReader,
			IntToSizeT(m_Header.vertexCount),
			IntToSizeT(m_Header.normalsOffset));

		m_TextureCoOrdinates = componentReader.ReadComponentArray<TextureCoOrdinate>(
			fileReader,
			IntToSizeT(m_Header.vertexCount),
			IntToSizeT(m_Header.textureCoOrdsOffset));

		m_BlendingScales = componentReader.ReadComponentArray<BlendingScales>(
			fileReader,
			IntToSizeT(m_Header.vertexCount),
			IntToSizeT(m_Header.blendingScalesOffset));

		m_Blends = componentReader.ReadComponentArray<Blending>(
			fileReader,
			IntToSizeT(m_Header.vertexCount),
			IntToSizeT(m_Header.blendingOffset));

		m_BoneFixUps = componentReader.ReadComponentArray<BoneFixUp>(
			fileReader,
			IntToSizeT(m_Header.bones.count),
			IntToSizeT(m_Header.boneFixUpOffset));

		m_Models.clear();

		for ( size_t modelIndex = 0;
			  modelIndex < IntToSizeT(m_Header.modelCount) && modelIndex < m_Header.modelOffsets.size();
			  ++modelIndex )
		{
			fileReader.SeekFromBeginning(m_Header.modelOffsets[modelIndex]);
			m_Models.emplace_back(componentReader.ReadComponent<Model>(fileReader));
		}

		for ( Model& model : m_Models )
		{
			for ( int32_t modelInfoOffset : model.modelInfoOffsets )
			{
				if ( modelInfoOffset == 0 )
				{
					continue;
				}

				fileReader.SeekFromBeginning(modelInfoOffset);
				model.modelInfos.emplace_back(componentReader.ReadComponent<ModelInfo>(fileReader));
			}

			for ( ModelInfo& modelInfo : model.modelInfos )
			{
				modelInfo.meshList = componentReader.ReadComponentArray<Mesh>(
					fileReader,
					modelInfo.meshes.count,
					modelInfo.meshes.offset);
			}
		}
	}

	void MDLFile::Validate() const
	{
		if ( m_Header.name.empty() )
		{
			throw ValidationException("MDLFile", "No embedded MDL name was present.");
		}
	}

	std::string MDLFile::ModelName() const
	{
		std::string name = m_Header.name;

		if ( name.length() < 4 )
		{
			return name;
		}

		std::for_each(
			name.begin() + name.length() - 3,
			name.end(),
			[](std::string::value_type& ch)
			{
				ch = static_cast<std::string::value_type>(std::tolower(ch));
			});

		if ( name.substr(name.length() - 4) == ".mdl" )
		{
			return name.substr(0, name.length() - 4);
		}

		return name;
	}

	const Header& MDLFile::GetHeader() const
	{
		return m_Header;
	}

	const Container<Attachment>& MDLFile::GetAttachments() const
	{
		return m_Attachments;
	}

	const Container<Bone>& MDLFile::GetBones() const
	{
		return m_Bones;
	}

	const Container<BodyGroup>& MDLFile::GetBodyGroups() const
	{
		return m_BodyGroups;
	}

	const Container<BoneController>& MDLFile::GetBoneControllers() const
	{
		return m_BoneControllers;
	}

	const Container<HitBox>& MDLFile::GetHitBoxes() const
	{
		return m_HitBoxes;
	}

	const SkinDataHolder& MDLFile::GetSkinData() const
	{
		return m_SkinData;
	}

	const Container<Texture>& MDLFile::GetTextures() const
	{
		return m_Textures;
	}

	void MDLFile::ValidateBeforeRead(BufferedFileReader::Ref ref) const
	{
		static constexpr const char* const EXPECTED_IDENTIFIER = "MDLZ";
		static constexpr int32_t EXPECTED_VERSION = 14;

		BufferedFileReader reader = ref.CreateSubReader(8);
		std::string identifier = reader.ReadString(4);

		if ( identifier != EXPECTED_IDENTIFIER )
		{
			throw ValidationException(
				reader.FilePath(),
				"Expected MDL identifier \"" + std::string(EXPECTED_IDENTIFIER) + "\" but got \"" + identifier + "\"");
		}

		int32_t version = reader.ReadElement<int32_t>();

		if ( version != EXPECTED_VERSION )
		{
			throw ValidationException(
				reader.FilePath(),
				"Expected MDL version \"" + std::to_string(EXPECTED_VERSION) + "\" but got \"" +
					std::to_string(version) + "\"");
		}
	}

	const Model* MDLFile::FindModelByOffset(int32_t offset) const
	{
		for ( size_t index = 0; index < IntToSizeT(m_Header.modelCount); ++index )
		{
			if ( m_Header.modelOffsets[index] == offset )
			{
				return &m_Models.GetElementChecked(index);
			}
		}

		return nullptr;
	}

	const Model* MDLFile::FindModelByName(const std::string& name) const
	{
		for ( const Model& model : m_Models )
		{
			if ( model.name == name )
			{
				return &model;
			}
		}

		return nullptr;
	}
}  // namespace MDLv14
