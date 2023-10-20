#include "MDLv14/MDLFile.h"
#include "BufferedFileReader.h"
#include "MDLv14/ComponentReader.h"
#include "MDLv14/ComponentUtils.h"
#include "Utils.h"

namespace MDLv14
{
	MDLFile::MDLFile(const BufferedFile& file)
	{
		BufferedFileReader fileReader = file.CreateReader();
		MDLv14::ComponentReader componentReader;

		m_Header = componentReader.ReadComponent<MDLv14::Header>(fileReader);

		m_Bones = componentReader.ReadComponentArray<MDLv14::Bone>(
			fileReader,
			IntToSizeT(m_Header.bones.count),
			IntToSizeT(m_Header.bones.offset));

		m_BoneControllers = componentReader.ReadComponentArray<MDLv14::BoneController>(
			fileReader,
			IntToSizeT(m_Header.boneControllers.count),
			IntToSizeT(m_Header.boneControllers.offset));

		m_HitBoxes = componentReader.ReadComponentArray<MDLv14::HitBox>(
			fileReader,
			IntToSizeT(m_Header.hitBoxes.count),
			IntToSizeT(m_Header.hitBoxes.offset));

		m_Sequences = componentReader.ReadComponentArray<MDLv14::Sequence>(
			fileReader,
			IntToSizeT(m_Header.sequences.count),
			IntToSizeT(m_Header.sequences.offset));

		for ( Sequence& sequence : m_Sequences )
		{
			sequence.eventCollection = componentReader.ReadComponentArray<MDLv14::Event>(
				fileReader,
				IntToSizeT(sequence.events.count),
				IntToSizeT(sequence.events.offset));
		}

		for ( Sequence& sequence : m_Sequences )
		{
			sequence.pivotCollection = componentReader.ReadComponentArray<MDLv14::Pivot>(
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

		m_SequenceGroups = componentReader.ReadComponentArray<MDLv14::SequenceGroup>(
			fileReader,
			IntToSizeT(m_Header.sequenceGroups.count),
			IntToSizeT(m_Header.sequenceGroups.offset));

		if ( m_Header.levelOfDetailFlags != LevelOfDetailFlag_None )
		{
			fileReader.SeekForward(DistToNextMultiple(fileReader.CurrentPosition(), 16));

			m_LevelsOfDetail = componentReader.ReadComponentArray<MDLv14::LevelOfDetail>(
				fileReader,
				MDLv14::LODFlagsToLODLevels(m_Header.levelOfDetailFlags));
		}

		m_Textures = componentReader.ReadComponentArray<MDLv14::Texture>(
			fileReader,
			IntToSizeT(m_Header.textures.count),
			IntToSizeT(m_Header.textures.offset));

		fileReader.SeekFromBeginning(m_Header.skinsOffset);
		m_SkinData = componentReader.ReadSkinData(fileReader, m_Header.skinFamilies, m_Header.skinReferences);

		m_BodyGroups = componentReader.ReadComponentArray<MDLv14::BodyGroup>(
			fileReader,
			IntToSizeT(m_Header.bodyGroups.count),
			IntToSizeT(m_Header.bodyGroups.offset));

		m_Attachments = componentReader.ReadComponentArray<MDLv14::Attachment>(
			fileReader,
			IntToSizeT(m_Header.attachments.count),
			IntToSizeT(m_Header.attachments.offset));

		m_SoundGroups = componentReader.ReadComponentArray<MDLv14::SoundGroup>(
			fileReader,
			IntToSizeT(m_Header.soundGroups.count),
			IntToSizeT(m_Header.soundGroups.offset));

		const size_t endOfSoundGroupLump = fileReader.CurrentPosition();

		for ( SoundGroup& soundGroup : m_SoundGroups )
		{
			fileReader.SeekFromBeginning(endOfSoundGroupLump + IntToSizeT(soundGroup.offset));
			soundGroup.sounds = componentReader.ReadComponent<MDLv14::Sounds>(fileReader);
		}

		m_TriangleMaps = componentReader.ReadComponentArray<MDLv14::TriangleMap>(
			fileReader,
			IntToSizeT(m_Header.triangleCount),
			IntToSizeT(m_Header.triangleMapOffset));

		m_Vertices = componentReader.ReadComponentArray<MDLv14::Vertex>(
			fileReader,
			IntToSizeT(m_Header.vertexCount),
			IntToSizeT(m_Header.verticesOffset));

		m_Normals = componentReader.ReadComponentArray<MDLv14::Normal>(
			fileReader,
			IntToSizeT(m_Header.vertexCount),
			IntToSizeT(m_Header.normalsOffset));

		m_TextureCoOrdinates = componentReader.ReadComponentArray<MDLv14::TextureCoOrdinate>(
			fileReader,
			IntToSizeT(m_Header.vertexCount),
			IntToSizeT(m_Header.textureCoOrdsOffset));

		m_BlendingScales = componentReader.ReadComponentArray<MDLv14::BlendingScales>(
			fileReader,
			IntToSizeT(m_Header.vertexCount),
			IntToSizeT(m_Header.blendingScalesOffset));

		m_Blends = componentReader.ReadComponentArray<MDLv14::Blending>(
			fileReader,
			IntToSizeT(m_Header.vertexCount),
			IntToSizeT(m_Header.blendingOffset));
	}

	const Header& MDLFile::GetHeader() const
	{
		return m_Header;
	}
}  // namespace MDLv14
