#include "QCv14/QCEFile.h"
#include "QCv14/CommandWriter.h"

namespace QCv14
{
	const QCEVersion& QCEFile::GetVersion() const
	{
		return m_Version;
	}

	void QCEFile::SetVersion(const QCEVersion& version)
	{
		m_Version = version;
	}

	void QCEFile::Write(std::ostream& stream) const
	{
		CommandWriter writer;

		writer.WriteCommand(stream, m_Version);
	}
}  // namespace QCv14
