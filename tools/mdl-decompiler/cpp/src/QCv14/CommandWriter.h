#pragma once

#include <ostream>
#include "QCv14/Commands.h"

namespace QCv14
{
	class CommandWriter
	{
	public:
		template<typename T>
		void WriteCommand(std::ostream& stream, const T& command)
		{
			WriteInternal(stream, command);
		}

	private:
		void WriteInternal(std::ostream& stream, const ModelName& command);
	};
}
