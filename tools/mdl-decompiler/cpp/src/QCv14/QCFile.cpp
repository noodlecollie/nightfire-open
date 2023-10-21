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

		writer.WriteCommand(stream, m_ModelName);

		// TODO: Write attachments
	}
}  // namespace QCv14
