#pragma once

#include <ostream>
#include "QCv14/QCCommands.h"

namespace QCv14
{
	class QCFile
	{
	public:
		void SetModelName(const QCModelName& modelName);

		void Write(std::ostream& stream) const;

	private:
		QCModelName m_ModelName;
	};
}
