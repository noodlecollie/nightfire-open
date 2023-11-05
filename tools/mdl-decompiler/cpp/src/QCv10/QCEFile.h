#pragma once

#include <ostream>
#include "QCv10/QCECommands.h"

namespace QCv10
{
	class QCEFile
	{
	public:
		void Write(std::ostream& stream) const;

		const QCEVersion& GetVersion() const;
		void SetVersion(const QCEVersion& version);

	private:
		QCEVersion m_Version;
	};
}
