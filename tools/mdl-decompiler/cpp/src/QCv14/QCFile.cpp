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

	const Container<QCAttachment> QCFile::GetAttachments() const
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

	const Container<QCBodyGroup> QCFile::GetBodyGroups() const
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

	void QCFile::Write(std::ostream& stream) const
	{
		CommandWriter writer;

		if ( !m_ModelName.IsValid() )
		{
			throw ValidationException("QCFile", "No model name was set.");
		}

		writer.WriteBanner(
			stream,
			[this](std::ostream& stream)
			{
				stream << "# Model: " << m_ModelName.name << std::endl;
				stream << "# Decompiled with " EXECUTABLE_NAME;
			});

		stream << std::endl;

		//////////////////////////////////////////////////////////////

		if ( m_CD.IsValid() )
		{
			writer.WriteCommand(stream, m_CD);
		}

		if ( m_CDTexture.IsValid() )
		{
			writer.WriteCommand(stream, m_CDTexture);
		}

		if ( m_ClipToTextures.IsValid() )
		{
			writer.WriteCommand(stream, m_ClipToTextures);
		}

		if ( m_ExternalTextures.IsValid() )
		{
			writer.WriteCommand(stream, m_ExternalTextures);
		}

		writer.WriteCommand(stream, m_ModelName);

		stream << std::endl;

		//////////////////////////////////////////////////////////////

		if ( m_Root.IsValid() )
		{
			writer.WriteCommand(stream, m_Root);
		}

		// pivot
		// mirror bone
		// rename bone

		stream << std::endl;

		// gamma
		// scale
		// flags
		// origin
		// eye position
		if ( m_BBox.IsValid() )
		{
			writer.WriteCommand(stream, m_BBox);
		}

		if ( m_CBox.IsValid() )
		{
			writer.WriteCommand(stream, m_CBox);
		}

		stream << std::endl;

		// texture group

		stream << std::endl;

		// bodies

		stream << std::endl;

		if ( !m_BodyGroups.empty() )
		{
			writer.WriteBanner(
				stream,
				[this](std::ostream& stream)
				{
					stream << "# " << m_BodyGroups.size() << " body groups";
				});

			stream << std::endl;

			for ( const QCBodyGroup& bodyGroup : m_BodyGroups )
			{
				writer.WriteCommand(stream, bodyGroup);
				stream << std::endl;
			}
		}

		stream << std::endl;

		if ( !m_Attachments.empty() )
		{
			writer.WriteBanner(
				stream,
				[this](std::ostream& stream)
				{
					stream << "# " << m_Attachments.size() << " attachments";
				});

			stream << std::endl;

			for ( const QCAttachment& attachment : m_Attachments )
			{
				writer.WriteCommand(stream, attachment);
			}
		}

		stream << std::endl;

		// controllers

		stream << std::endl;

		// hboxes

		stream << std::endl;

		// Sequences

		stream << std::endl;
	}
}  // namespace QCv14
