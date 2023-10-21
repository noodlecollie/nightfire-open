#pragma once

#include <ostream>
#include "QCv14/QCECommands.h"

namespace QCv14
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
