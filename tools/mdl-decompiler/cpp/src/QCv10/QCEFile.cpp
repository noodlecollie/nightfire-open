#include "QCv10/QCEFile.h"
#include "QCv10/CommandWriter.h"

namespace QCv10
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

		writer.Write(stream, m_Version);
	}
}  // namespace QCv10
