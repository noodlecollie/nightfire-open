#include "QCv14/CommandWriter.h"

namespace QCv14
{
	void CommandWriter::WriteInternal(std::ostream& stream, const ModelName& command)
	{
		stream << "$modelname \"" << command.name << "\"\n";
	}
}
