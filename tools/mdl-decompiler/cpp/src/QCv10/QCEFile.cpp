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

	const Container<QCEReplaceActivity>& QCEFile::GetReplaceActivities() const
	{
		return m_ReplaceActivities;
	}

	void QCEFile::ClearReplaceActivities()
	{
		m_ReplaceActivities.clear();
	}

	void QCEFile::AddReplaceActivity(const QCEReplaceActivity& replaceActivity)
	{
		m_ReplaceActivities.emplace_back(replaceActivity);
	}

	void QCEFile::Write(std::ostream& stream) const
	{
		CommandWriter writer;

		writer.Write(stream, m_Version);

		for ( const QCEReplaceActivity& replace : m_ReplaceActivities )
		{
			writer.Write(stream, replace);
		}
	}
}  // namespace QCv10
