#pragma once

#include <ostream>
#include "QCv14/QCCommands.h"
#include "QCv14/QCECommands.h"

namespace QCv14
{
	class CommandWriter
	{
	public:
		template<typename T>
		void WriteCommand(std::ostream& stream, const T& command)
		{
			WriteInternal(stream, command);
			stream << "\n";
		}

	private:
		void WriteInternal(std::ostream& stream, const QCEVersion& command);

		void WriteInternal(std::ostream& stream, const QCModelName& command);
	};
}
