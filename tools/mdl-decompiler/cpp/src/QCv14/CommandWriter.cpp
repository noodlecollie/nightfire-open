#include <iomanip>
#include <cmath>
#include "QCv14/CommandWriter.h"

namespace QCv14
{
	void CommandWriter::WriteInternal(std::ostream& stream, const QCEVersion& command)
	{
		stream << "$qceversion " << static_cast<int32_t>(command.game) << "." << command.version << "."
			   << command.versionMajor << "." << command.versionMinor;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCEReplaceActivity& command)
	{
		stream << "$replaceactivity \"" << command.sequenceName << "\" " << command.activity;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const Vec3D& position)
	{
		const auto defaultPrecision = stream.precision();
		stream << std::setprecision(6);

		float val = std::isnan(position.x) ? 0.0f : position.x;
		stream << val;

		val = std::isnan(position.y) ? 0.0f : position.y;
		stream << " " << val;

		val = std::isnan(position.z) ? 0.0f : position.z;
		stream << " " << val;

		stream << std::setprecision(defaultPrecision);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCModelName& command)
	{
		stream << "$modelname \"" << command.name << "\"";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCAttachment& command)
	{
		stream << "$attachment \"" << command.name << "\" \"" << command.bone << "\" ";
		WriteInternal(stream, command.position);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCBBox& command)
	{
		stream << "$bbox ";
		WriteInternal(stream, command.min);
		stream << " ";
		WriteInternal(stream, command.max);
	}
}  // namespace QCv14
