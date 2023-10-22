#include "QCv14/QCFile.h"
#include "QCv14/CommandWriter.h"
#include "Exceptions.h"

namespace QCv14
{
	void QCFile::SetModelName(const QCModelName& modelName)
	{
		m_ModelName = modelName;
	}

	const std::vector<QCAttachment> QCFile::GetAttachments() const
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

	void QCFile::SetBBox(const QCBBox& bbox)
	{
		m_BBox = bbox;
	}

	void QCFile::Write(std::ostream& stream) const
	{
		CommandWriter writer;

		if ( m_ModelName.name.empty() )
		{
			throw ValidationException("QCFile", "No model name was set.");
		}

		writer.WriteBanner(
			stream,
			[this](std::ostream& stream)
			{
				stream << "# Model: " << m_ModelName.name;
			});

		stream << std::endl;

		// cd
		// cdtexture
		// clip to textures
		// External textures

		writer.WriteCommand(stream, m_ModelName);
		stream << std::endl;

		// root
		// pivot
		// mirror bone
		// rename bone

		// gamma
		// scale
		// flags
		// origin
		// eye position
		if ( m_BBox.IsValid() )
		{
			writer.WriteCommand(stream, m_BBox);
		}
		// cbox

		// texture group

		// bodies

		// body groups

		if ( !m_Attachments.empty() )
		{
			writer.WriteBanner(
				stream,
				[this](std::ostream& stream)
				{
					stream << "# " << m_Attachments.size() << " attachments";
				});

			for ( const QCAttachment& attachment : m_Attachments )
			{
				writer.WriteCommand(stream, attachment);
			}

			stream << std::endl;
		}

		// controllers

		// hboxes

		// Sequences
	}
}  // namespace QCv14
