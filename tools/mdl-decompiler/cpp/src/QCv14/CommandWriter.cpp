#include "QCv14/CommandWriter.h"

namespace QCv14
{
	void CommandWriter::WriteInternal(std::ostream& stream, const QCEVersion& command)
	{
		stream << "$qceversion " << static_cast<int32_t>(command.game) << "." << command.version << "."
			   << command.versionMajor << "." << command.versionMinor;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCModelName& command)
	{
		stream << "$modelname \"" << command.name << "\"";
	}
}  // namespace QCv14
