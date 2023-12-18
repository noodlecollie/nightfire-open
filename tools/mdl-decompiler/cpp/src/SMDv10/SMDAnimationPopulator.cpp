#include <stdexcept>
#include "SMDv10/SMDAnimationPopulator.h"
#include "MDLv14/ComponentReflection.h"

namespace SMDv10
{
	SMDAnimationPopulator::SMDAnimationPopulator(
		const std::shared_ptr<MDLv14::MDLFile>& mdlFile,
		const std::string& sequenceName) :
		m_MDLFile(mdlFile),
		m_SequenceName(sequenceName)
	{
		if ( !m_MDLFile )
		{
			throw std::invalid_argument("Expected a valid MDL file");
		}
	}

	std::shared_ptr<SMDFile> SMDAnimationPopulator::Populate()
	{
		const Container<MDLv14::Bone>& bones = m_MDLFile->GetBones();

		std::shared_ptr<SMDv10::SMDFile> smdFile = std::make_shared<SMDv10::SMDFile>(SMDv10::SMDFile::Type::Animation);

		Container<SMDv10::NodeFrame> defaultBoneFrameValues;

		for ( size_t boneIndex = 0; boneIndex < bones.size(); ++boneIndex )
		{
			const MDLv14::Bone& bone = bones.GetElementChecked(boneIndex);

			smdFile->AddNode(SMDv10::Node(static_cast<int8_t>(boneIndex), bone.name, static_cast<int8_t>(bone.parent)));
			defaultBoneFrameValues.emplace_back(
				SMDv10::NodeFrame(static_cast<int8_t>(boneIndex), bone.position, bone.rotation));
		}

		uint8_t blend = 0;
		const MDLv14::Sequence* mdlSeq = m_MDLFile->FindSequenceByName(m_SequenceName, &blend);

		if ( !mdlSeq )
		{
			// Should never happen
			throw std::runtime_error("Lookup failed for sequence \"" + m_SequenceName + "\"");
		}

		for ( size_t frameIndex = 0; frameIndex < static_cast<size_t>(mdlSeq->frameCount); ++frameIndex )
		{
			Container<SMDv10::NodeFrame> boneFrameValues;

			for ( size_t boneIndex = 0; boneIndex < bones.size(); ++boneIndex )
			{
				const MDLv14::Bone& bone = bones.GetElementChecked(boneIndex);
				Vec3D bonePos = bone.position;
				Vec3D boneRot = bone.rotation;

				for ( size_t axis = 0; axis < 6; ++axis )
				{
					const std::vector<int16_t>& animData =
						mdlSeq->animationCollection.GetDataArray(static_cast<size_t>(blend), boneIndex, axis);

					if ( animData.empty() )
					{
						continue;
					}

					if ( frameIndex > animData.size() )
					{
						// Should never happen
						throw std::runtime_error(
							"Frame " + std::to_string(frameIndex) + " exceeded animation data for sequence \"" +
							m_SequenceName + "\"");
					}

					const int16_t animValue = animData[frameIndex];

					switch ( axis )
					{
						case 0:
						{
							bonePos.x += animValue * bone.scalePosition.x;
							break;
						}

						case 1:
						{
							bonePos.y += animValue * bone.scalePosition.y;
							break;
						}

						case 2:
						{
							bonePos.z += animValue * bone.scalePosition.z;
							break;
						}

						case 3:
						{
							boneRot.x += animValue * bone.scaleRotation.x;
							break;
						}

						case 4:
						{
							boneRot.y += animValue * bone.scaleRotation.y;
							break;
						}

						case 5:
						{
							boneRot.z += animValue * bone.scaleRotation.z;
							break;
						}
					}
				}

				if ( bone.parent < 0 && mdlSeq->frameCount > 1 )
				{
					// Force linear movement
					bonePos.x += frameIndex * mdlSeq->linearMovement.x / (mdlSeq->frameCount - 1);
					bonePos.y += frameIndex * mdlSeq->linearMovement.y / (mdlSeq->frameCount - 1);
					bonePos.z += frameIndex * mdlSeq->linearMovement.z / (mdlSeq->frameCount - 1);
				}

				boneFrameValues.emplace_back(SMDv10::NodeFrame(static_cast<int8_t>(boneIndex), bonePos, boneRot));
			}

			smdFile->AddFrame(SMDv10::Frame(static_cast<int32_t>(frameIndex), boneFrameValues));
		}

		if ( smdFile->GetFrames().empty() )
		{
			smdFile->AddFrame(SMDv10::Frame(0, defaultBoneFrameValues));
		}

		smdFile->RotateAroundZAxis(-90.0f);

		return smdFile;
	}
}  // namespace SMDv10
