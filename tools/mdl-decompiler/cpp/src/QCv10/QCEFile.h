#pragma once

#include <ostream>
#include "QCv10/QCECommands.h"
#include "Common/Container.h"

namespace QCv10
{
	class QCEFile
	{
	public:
		void Write(std::ostream& stream) const;

		const QCEVersion& GetVersion() const;
		void SetVersion(const QCEVersion& version);

		const Container<QCEReplaceActivity>& GetReplaceActivities() const;
		void ClearReplaceActivities();
		void AddReplaceActivity(const QCEReplaceActivity& replaceActivity);

	private:
		QCEVersion m_Version;
		Container<QCEReplaceActivity> m_ReplaceActivities;
	};
}
