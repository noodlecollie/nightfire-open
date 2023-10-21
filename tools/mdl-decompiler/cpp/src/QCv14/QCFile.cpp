#include "QCv14/QCFile.h"
#include "QCv14/CommandWriter.h"

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

	void QCFile::Write(std::ostream& stream) const
	{
		CommandWriter writer;

		writer.WriteBanner(stream, [this](std::ostream& stream)
		{
			stream << "# Model: " << m_ModelName.name;
		});

		stream << std::endl;

		writer.WriteCommand(stream, m_ModelName);
		stream << std::endl;

		writer.WriteBanner(stream, [this](std::ostream& stream)
		{
			stream << "# " << m_Attachments.size() << " attachments";
		});

		for ( const QCAttachment& attachment : m_Attachments )
		{
			writer.WriteCommand(stream, attachment);
		}

		stream << std::endl;
	}
}  // namespace QCv14
