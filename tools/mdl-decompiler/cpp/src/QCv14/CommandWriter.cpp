#include <iomanip>
#include <cmath>
#include "QCv14/CommandWriter.h"

namespace QCv14
{
	void CommandWriter::IncreaseIndent()
	{
		++m_Indent;
	}

	void CommandWriter::DecreaseIndent()
	{
		if ( m_Indent > 0 )
		{
			--m_Indent;
		}
	}

	std::string CommandWriter::IndentString() const
	{
		return std::string(m_Indent, '\t');
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCEVersion& command)
	{
		stream << IndentString() << "$qceversion " << static_cast<int32_t>(command.game) << "." << command.version
			   << "." << command.versionMajor << "." << command.versionMinor;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCEReplaceActivity& command)
	{
		stream << IndentString() << "$replaceactivity \"" << command.sequenceName << "\" " << command.activity;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const Vec3D& position)
	{
		const auto defaultPrecision = stream.precision();
		stream << IndentString();

		float val = std::isnan(position.x) ? 0.0f : position.x;
		stream << std::setprecision(6) << val;

		val = std::isnan(position.y) ? 0.0f : position.y;
		stream << " " << std::setprecision(6) << val;

		val = std::isnan(position.z) ? 0.0f : position.z;
		stream << " " << std::setprecision(6) << val;

		stream << std::setprecision(defaultPrecision);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCModelName& command)
	{
		stream << IndentString() << "$modelname \"" << command.name << "\"";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCAttachment& command)
	{
		stream << IndentString() << "$attachment \"" << command.name << "\" \"" << command.bone << "\" ";
		WriteInternal(stream, command.position);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCBBox& command)
	{
		stream << IndentString() << "$bbox ";
		WriteInternal(stream, command.min);
		stream << " ";
		WriteInternal(stream, command.max);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCCBox& command)
	{
		stream << IndentString() << "$cbox ";
		WriteInternal(stream, command.min);
		stream << " ";
		WriteInternal(stream, command.max);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCBodyGroupItem& command)
	{
		stream << IndentString();

		if ( command.name == "studio" )
		{
			stream << "studio \"" << command.file << "\"";
		}
		else if ( command.name == "blank" )
		{
			stream << "blank";
		}
		else
		{
			stream << "\"" << command.name << "\" \"" << command.file << "\"";
		}
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCBodyGroup& command)
	{
		stream << IndentString() << "$bodygroup \"" << command.name << "\"" << std::endl;
		stream << IndentString() << "{" << std::endl;

		IncreaseIndent();

		for ( const QCBodyGroupItem& body : command.bodies )
		{
			WriteInternal(stream, body);
			stream << std::endl;
		}

		DecreaseIndent();

		stream << IndentString() << "}";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCCD& command)
	{
		stream << IndentString() << "$cd \"" << command.path << "\"";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCCDTexture& command)
	{
		stream << IndentString() << "$cdtexture \"" << command.path << "\"";
	}
}  // namespace QCv14
