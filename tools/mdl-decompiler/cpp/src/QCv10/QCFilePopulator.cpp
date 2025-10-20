#include <stdexcept>
#include <limits>
#include <iostream>
#include "MDLv14/ComponentReflection.h"
#include "QCv10/QCFilePopulator.h"
#include "SMDv10/SMDReferencePopulator.h"
#include "Conversions/Activity.h"
#include "Conversions/MotionFlags.h"
#include "Exceptions.h"
#include "Utils.h"

namespace QCv10
{
	QCFilePopulator::QCFilePopulator(
		const std::shared_ptr<MDLv14::MDLFile>& m_MDLFile,
		const std::shared_ptr<QCv10::QCFile>& m_QCFile,
		const std::shared_ptr<QCv10::QCEFile>& m_QCEFile,
		const std::string& outputDir) :
		m_MDLFile(m_MDLFile),
		m_QCFile(m_QCFile),
		m_QCEFile(m_QCEFile),
		m_OutputDir(outputDir)
	{
		if ( !m_MDLFile )
		{
			throw std::invalid_argument("Expected a valid MDL file");
		}

		if ( !m_QCFile )
		{
			throw std::invalid_argument("Expected a valid QC file");
		}

		if ( !m_QCEFile )
		{
			throw std::invalid_argument("Expected a valid QCE file");
		}
	}

	void QCFilePopulator::Populate()
	{
		m_ReferenceSMDs.clear();
		m_AnimationSMDs.clear();

		BaseSetup();
		PopulateBones();
		PopulateAttachments();
		PopulateBodyGroups();
		PopulateBoneControllers();
		PopulateHitBoxes();
		PopulateSequences();
		PopulateSkins();
	}

	const std::vector<SMDv10::SMDName>& QCFilePopulator::GetReferenceSMDNames() const
	{
		return m_ReferenceSMDs;
	}

	const std::vector<SMDv10::SMDName>& QCFilePopulator::GetAnimationSMDNames() const
	{
		return m_AnimationSMDs;
	}

	void QCFilePopulator::BaseSetup()
	{
		m_QCEFile->SetVersion(QCv10::QCEVersion(QCv10::QCEGame::HalfLife, 10, 1, 0));

		m_QCFile->SetDirectory(m_OutputDir);
		m_QCFile->SetModelName({m_MDLFile->GetHeader().name});
		m_QCFile->SetBBox(
			QCv10::QCBBox(m_MDLFile->GetHeader().boundingBox.min, m_MDLFile->GetHeader().boundingBox.max));
		m_QCFile->SetCBox(
			QCv10::QCCBox(m_MDLFile->GetHeader().clippingBox.min, m_MDLFile->GetHeader().clippingBox.max));
		m_QCFile->SetEyePosition(QCv10::QCEyePosition(m_MDLFile->GetHeader().eyePosition.pos));
		m_QCFile->SetTypeFlags(QCv10::QCTypeFlags(m_MDLFile->GetHeader().typeFlags));
		m_QCFile->SetOrigin(QCv10::QCOrigin(Vec3D()));
	}

	void QCFilePopulator::PopulateBones()
	{
		for ( const MDLv14::Bone& bone : m_MDLFile->GetBones() )
		{
			if ( bone.parent == -1 )
			{
				m_QCFile->SetRoot(QCv10::QCRoot(bone.name));
				break;
			}
		}
	}

	void QCFilePopulator::PopulateAttachments()
	{
		for ( const MDLv14::Attachment& attachment : m_MDLFile->GetAttachments() )
		{
			m_QCFile->AddAttachment(QCv10::QCAttachment(
				attachment.name,
				m_MDLFile->GetBones().GetElementChecked(attachment.bone).name,
				attachment.position));
		}
	}

	void QCFilePopulator::PopulateBodyGroups()
	{
		for ( const MDLv14::BodyGroup& mdlBodyGroup : m_MDLFile->GetBodyGroups() )
		{
			QCv10::QCBodyGroup qcBodyGroup;
			qcBodyGroup.name = mdlBodyGroup.name;

			for ( size_t index = 0; index < IntToSizeT(mdlBodyGroup.modelCount); ++index )
			{
				const MDLv14::Model* model = m_MDLFile->FindModelByOffset(
					mdlBodyGroup.modelOffset + static_cast<int32_t>((index * Reflection::ReadSize<MDLv14::Model>())));

				if ( !model )
				{
					throw ValidationException(
						"MDLFile",
						"Body group " + mdlBodyGroup.name + " model " + std::to_string(index) + " (offset " +
							std::to_string(mdlBodyGroup.modelOffset) +
							" bytes) could not be mapped to a model component.");
				}

				if ( model->name == "blank" )
				{
					qcBodyGroup.bodies.emplace_back(QCv10::QCBodyGroupItem("blank"));
				}
				else
				{
					const std::string smdFileName = GoodFileName(model->name) + "_ref";
					m_ReferenceSMDs.emplace_back(SMDv10::SMDName(model->name, smdFileName));

					qcBodyGroup.bodies.emplace_back(QCv10::QCBodyGroupItem("studio", smdFileName));
				}
			}

			m_QCFile->AddBodyGroup(qcBodyGroup);
		}
	}

	void QCFilePopulator::PopulateBoneControllers()
	{
		for ( const MDLv14::BoneController& controller : m_MDLFile->GetBoneControllers() )
		{
			QCv10::QCBoneController qcController {};

			qcController.index = controller.index;
			qcController.bone = m_MDLFile->GetBones().GetElementChecked(controller.bone).name;
			qcController.motionType = controller.motionType;
			qcController.start = controller.start;
			qcController.end = controller.end;

			m_QCFile->AddController(qcController);
		}
	}

	void QCFilePopulator::PopulateHitBoxes()
	{
		for ( const MDLv14::HitBox& hitBox : m_MDLFile->GetHitBoxes() )
		{
			m_QCFile->AddHitBox(QCv10::QCHitBox(
				hitBox.group,
				m_MDLFile->GetBones().GetElementChecked(hitBox.bone).name,
				hitBox.min,
				hitBox.max));
		}
	}

	void QCFilePopulator::PopulateSequences()
	{
		if ( !m_MDLFile->GetBones().empty() )
		{
			for ( size_t seqIndex = 0; seqIndex < m_MDLFile->GetSequences().size(); ++seqIndex )
			{
				const MDLv14::Sequence& sequence = m_MDLFile->GetSequences().GetElementChecked(seqIndex);
				QCv10::QCSequence qcSeq;

				qcSeq.name = sequence.name;
				qcSeq.controlFlags = sequence.motionType;
				qcSeq.framesPerSecond = sequence.framesPerSecond;
				qcSeq.loop = (sequence.flags & MDLv14::SequenceFlag_Looping) != 0;

				if ( sequence.blendType0 != CommonTypes::MotionFlag_None )
				{
					qcSeq.blends.emplace_back(
						QCv10::QCOptionBlend(sequence.blendType0, sequence.blendStart0, sequence.blendEnd0));
				}

				if ( sequence.blendType1 != CommonTypes::MotionFlag_None )
				{
					qcSeq.blends.emplace_back(
						QCv10::QCOptionBlend(sequence.blendType1, sequence.blendStart1, sequence.blendEnd1));
				}

				if ( sequence.nodeEntry > 0 )
				{
					if ( sequence.nodeEntry <= std::numeric_limits<int8_t>::max() )
					{
						qcSeq.nodeEntryBone = static_cast<int8_t>(sequence.nodeEntry);
					}
					else
					{
						throw std::invalid_argument(
							"MDL sequence " + std::to_string(seqIndex) +
							" entry node bone was out of acceptable range");
					}
				}

				if ( sequence.nodeExit > 0 )
				{
					qcSeq.transition = QCv10::QCOptionTransition(
						sequence.nodeEntry,
						sequence.nodeExit,
						sequence.nodeFlags & MDLv14::NodeFlag_Reverse);
				}

				qcSeq.events.reserve(sequence.eventCollection.size());

				for ( const MDLv14::Event& event : sequence.eventCollection )
				{
					qcSeq.events.emplace_back(QCv10::QCOptionEvent(event.event, event.frame, event.options));
				}

				qcSeq.pivots.reserve(sequence.pivotCollection.size());

				for ( size_t index = 0; index < sequence.pivotCollection.size(); ++index )
				{
					qcSeq.pivots.emplace_back(QCv10::QCOptionPivot(
						static_cast<int32_t>(index),
						static_cast<float>(sequence.pivotCollection[index].start),
						static_cast<float>(sequence.pivotCollection[index].end)));
				}

				ReadActivity(qcSeq, seqIndex);

				qcSeq.files.emplace_back(RecordAnimationName(sequence.name));

				for ( int32_t blendIndex = 1; blendIndex < sequence.blendCount; ++blendIndex )
				{
					qcSeq.files.emplace_back(RecordAnimationName(sequence.name, blendIndex));
				}

				m_QCFile->AddSequence(std::move(qcSeq));
			}
		}
	}

	void QCFilePopulator::PopulateSkins()
	{
		const MDLv14::SkinDataHolder& skins = m_MDLFile->GetSkinData();

		if ( skins.SkinFamilyCount() > 1 )
		{
			const Container<MDLv14::Texture>& textures = m_MDLFile->GetTextures();
			QCv10::QCTextureGroup texGroup;

			texGroup.name = "Skins";
			texGroup.skins.reserve(skins.SkinFamilyCount());

			for ( size_t familyIndex = 0; familyIndex < skins.SkinFamilyCount(); ++familyIndex )
			{
				Container<std::string> textureNames;
				textureNames.reserve(skins.SkinReferenceCount());

				for ( size_t refIndex = 0; refIndex < skins.SkinReferenceCount(); ++refIndex )
				{
					const int16_t& textureIndex = skins.GetEntry(familyIndex, refIndex);
					const std::string& textureName =
						textures.GetElementChecked(static_cast<size_t>(textureIndex)).textureName;

					textureNames.emplace_back(textureName);
				}

				texGroup.skins.emplace_back(std::move(textureNames));
			}

			m_QCFile->SetTextureGroup(std::move(texGroup));
		}
	}

	void QCFilePopulator::ReadActivity(QCv10::QCSequence& qcSequence, size_t seqIndex)
	{
		const MDLv14::Sequence& sequence = m_MDLFile->GetSequences().GetElementChecked(seqIndex);

		if ( sequence.activity == 0 )
		{
			return;
		}

		MDLv14::Activity activity = MDLv14::ACT_INVALID;

		try
		{
			activity = Conversion::ConvertActivity_IntToV14(sequence.activity);
		}
		catch ( const std::invalid_argument& ex )
		{
			std::cerr << m_MDLFile->ModelName() << ": sequence " << seqIndex << " activity could not be read. "
					  << ex.what() << std::endl;

			return;
		}

		QCv10::Activity qcActivity = QCv10::ACT_INVALID;

		try
		{
			qcActivity = Conversion::ConvertActivity_V14ToV10(activity);
		}
		catch ( const std::invalid_argument& ex )
		{
			std::cerr << m_MDLFile->ModelName() << ": sequence " << seqIndex << " activity could not be converted. "
					  << ex.what() << std::endl;

			return;
		}

		qcSequence.activity.activity = qcActivity;
		qcSequence.activity.weight = static_cast<float>(sequence.activityWeight);

		if ( !StartsWith(Conversion::ActivityName(qcActivity), "ACT_") )
		{
			m_QCEFile->AddReplaceActivity(
				QCv10::QCEReplaceActivity(qcSequence.name, Conversion::ActivityName(activity)));
			qcSequence.activity.activity = QCv10::ACT_INVALID;
		}
	}

	std::string QCFilePopulator::RecordAnimationName(const std::string& baseName)
	{
		m_AnimationSMDs.emplace_back(SMDv10::SMDName(baseName, GoodFileName(baseName) + "_ani"));
		return m_AnimationSMDs.back().outputNameOnDisk;
	}

	std::string QCFilePopulator::RecordAnimationName(const std::string& baseName, int32_t blendIndex)
	{
		if ( blendIndex < 1 )
		{
			throw std::invalid_argument("Expected blend index to be greater than zero");
		}

		return RecordAnimationName(baseName + "_blend" + std::to_string(blendIndex));
	}
}  // namespace QCv10
