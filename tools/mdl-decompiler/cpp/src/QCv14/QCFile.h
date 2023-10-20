#pragma once

#include <ostream>
#include "QCv14/Commands.h"

namespace QCv14
{
	class QCFile
	{
	public:
		void SetModelName(const ModelName& modelName);

		void Write(std::ostream& stream) const;

	private:
		ModelName m_ModelName;
	};
}
