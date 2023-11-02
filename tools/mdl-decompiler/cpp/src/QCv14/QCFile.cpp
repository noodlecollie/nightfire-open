#include "QCv14/QCFile.h"
#include "QCv14/CommandWriter.h"
#include "Exceptions.h"

namespace QCv14
{
	void QCFile::SetDirectory(const QCCD& dir)
	{
		m_CD = dir;
	}

	void QCFile::SetTextureDirectory(const QCCDTexture& textureDir)
	{
		m_CDTexture = textureDir;
	}

	void QCFile::SetModelName(const QCModelName& modelName)
	{
		m_ModelName = modelName;
	}

	const Container<QCAttachment>& QCFile::GetAttachments() const
	{
		return m_Attachments;
	}

	void QCFile::ClearAttachments()
	{
		m_Attachments.clear();
	}

	void QCFile::AddAttachment(const QCAttachment& attachment)
	{
		m_Attachments.emplace_back(attachment);
	}

	const Container<QCBodyGroup>& QCFile::GetBodyGroups() const
	{
		return m_BodyGroups;
	}

	void QCFile::ClearBodyGroups()
	{
		m_BodyGroups.clear();
	}

	void QCFile::AddBodyGroup(const QCBodyGroup& bodyGroup)
	{
		m_BodyGroups.emplace_back(bodyGroup);
	}

	const Container<QCBoneController>& QCFile::GetControllers() const
	{
		return m_BoneControllers;
	}

	void QCFile::ClearControllers()
	{
		m_BoneControllers.clear();
	}

	void QCFile::AddController(const QCBoneController& controller)
	{
		m_BoneControllers.emplace_back(controller);
	}

	const Container<QCHitBox>& QCFile::GetHitBoxes() const
	{
		return m_HitBoxes;
	}

	void QCFile::ClearHitBoxes()
	{
		m_HitBoxes.clear();
	}

	void QCFile::AddHitBox(const QCHitBox& hitbox)
	{
		m_HitBoxes.emplace_back(hitbox);
	}

	void QCFile::SetBBox(const QCBBox& bbox)
	{
		m_BBox = bbox;
	}

	void QCFile::SetCBox(const QCCBox& cbox)
	{
		m_CBox = cbox;
	}

	void QCFile::SetClipToTextures(const QCClipToTextures& clipToTextures)
	{
		m_ClipToTextures = clipToTextures;
	}

	void QCFile::SetExternalTextures(const QCExternalTextures& externalTextures)
	{
		m_ExternalTextures = externalTextures;
	}

	void QCFile::SetRoot(const QCRoot root)
	{
		m_Root = root;
	}

	void QCFile::SetEyePosition(const QCEyePosition& eyePosition)
	{
		m_EyePosition = eyePosition;
	}

	void QCFile::SetPivot(const QCPivot& pivot)
	{
		m_Pivot = pivot;
	}

	void QCFile::SetMirrorBone(const QCMirrorBone& mirrorBone)
	{
		m_MirrorBone = mirrorBone;
	}

	void QCFile::SetRenameBone(const QCRenameBone& renameBone)
	{
		m_RenameBone = renameBone;
	}

	void QCFile::SetGamma(const QCGamma& gamma)
	{
		m_Gamma = gamma;
	}

	void QCFile::SetScale(const QCScale& scale)
	{
		m_Scale = scale;
	}

	void QCFile::SetTypeFlags(const QCTypeFlags& typeFlags)
	{
		m_TypeFlags = typeFlags;
	}

	void QCFile::SetOrigin(const QCOrigin& origin)
	{
		m_Origin = origin;
	}

	void QCFile::SetTextureGroup(QCTextureGroup&& textureGroup)
	{
		m_TextureGroup = std::move(textureGroup);
	}

	void QCFile::Write(std::ostream& stream) const
	{
		CommandWriter writer;

		if ( !m_ModelName.IsValid() )
		{
			throw ValidationException("QCFile", "No model name was set.");
		}

		stream << "########################################" << std::endl;
		stream << "# QC for: " << m_ModelName.name << std::endl;
		stream << "# Decompiled with " EXECUTABLE_NAME << std::endl;
		stream << "#" << std::endl;
		stream << "# " << m_BodyGroups.size() << " body groups" << std::endl;
		stream << "# " << m_Attachments.size() << " attachments" << std::endl;
		stream << "# " << m_BoneControllers.size() << " bone controllers" << std::endl;
		stream << "# " << m_HitBoxes.size() << " hit boxes" << std::endl;
		stream << "########################################" << std::endl;
		stream << std::endl;

		//////////////////////////////////////////////////////////////

		writer.WriteQCCommand(stream, m_CD);
		writer.WriteQCCommand(stream, m_CDTexture);
		writer.WriteQCCommand(stream, m_ClipToTextures);
		writer.WriteQCCommand(stream, m_ExternalTextures);
		writer.WriteQCCommand(stream, m_ModelName);

		stream << std::endl;

		//////////////////////////////////////////////////////////////

		writer.WriteQCCommand(stream, m_Root);
		writer.WriteQCCommand(stream, m_Pivot);
		writer.WriteQCCommand(stream, m_MirrorBone);
		writer.WriteQCCommand(stream, m_RenameBone);

		stream << std::endl;

		//////////////////////////////////////////////////////////////

		writer.WriteQCCommand(stream, m_Gamma);
		writer.WriteQCCommand(stream, m_Scale);
		writer.WriteQCCommand(stream, m_TypeFlags);
		writer.WriteQCCommand(stream, m_Origin);
		writer.WriteQCCommand(stream, m_EyePosition);
		writer.WriteQCCommand(stream, m_BBox);
		writer.WriteQCCommand(stream, m_CBox);

		stream << std::endl;

		//////////////////////////////////////////////////////////////

		writer.WriteQCCommand(stream, m_TextureGroup);

		stream << std::endl;

		//////////////////////////////////////////////////////////////

		// bodies

		stream << std::endl;

		//////////////////////////////////////////////////////////////

		for ( const QCBodyGroup& bodyGroup : m_BodyGroups )
		{
			writer.WriteQCCommand(stream, bodyGroup);
			stream << std::endl;
		}

		stream << std::endl;

		//////////////////////////////////////////////////////////////

		for ( const QCAttachment& attachment : m_Attachments )
		{
			writer.WriteQCCommand(stream, attachment);
		}

		stream << std::endl;

		//////////////////////////////////////////////////////////////

		for ( const QCBoneController& controller : m_BoneControllers )
		{
			writer.WriteQCCommand(stream, controller);
		}

		stream << std::endl;

		//////////////////////////////////////////////////////////////

		for ( const QCHitBox& hitBox : m_HitBoxes )
		{
			writer.WriteQCCommand(stream, hitBox);
		}

		stream << std::endl;

		//////////////////////////////////////////////////////////////

		// Sequences

		stream << std::endl;
	}
}  // namespace QCv14
