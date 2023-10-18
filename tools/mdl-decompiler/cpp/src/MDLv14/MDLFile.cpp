#include "MDLv14/MDLFile.h"
#include "BufferedFileReader.h"
#include "MDLv14/ComponentReader.h"
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
	}
}  // namespace MDLv14
