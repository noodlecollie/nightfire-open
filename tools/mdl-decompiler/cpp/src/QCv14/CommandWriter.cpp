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
		stream << "$qceversion " << static_cast<int32_t>(command.game) << "." << command.version
			   << "." << command.versionMajor << "." << command.versionMinor;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCEReplaceActivity& command)
	{
		stream << "$replaceactivity \"" << command.sequenceName << "\" " << command.activity;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const Vec3D& position)
	{
		const auto defaultPrecision = stream.precision();

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

	void CommandWriter::WriteInternal(std::ostream& stream, const QCCBox& command)
	{
		stream << "$cbox ";
		WriteInternal(stream, command.min);
		stream << " ";
		WriteInternal(stream, command.max);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCBodyGroupItem& command)
	{
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
		stream << "$bodygroup \"" << command.name << "\"" << std::endl;
		stream << IndentString() << "{" << std::endl;

		IncreaseIndent();

		for ( const QCBodyGroupItem& body : command.bodies )
		{
			WriteQCCommand(stream, body);
		}

		DecreaseIndent();

		stream << IndentString() << "}";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCCD& command)
	{
		stream << "$cd \"" << command.path << "\"";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCCDTexture& command)
	{
		stream << "$cdtexture \"" << command.path << "\"";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCClipToTextures& command)
	{
		if ( command.enabled )
		{
			stream << "$cliptotextures";
		}
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCExternalTextures& command)
	{
		if ( command.enabled )
		{
			stream << "$externaltextures";
		}
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCRoot& command)
	{
		stream << "$root \"" << command.bone << "\"";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCBoneController& command)
	{
		stream << "$controller " << command.index << " \"" << command.bone << "\" "
			   << command.motionFlags << " " << command.start << " " << command.end;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCHitBox& command)
	{
		stream << "$hbox " << command.group << " \"" << command.bone << "\" ";
		WriteInternal(stream, command.min);
		stream << " ";
		WriteInternal(stream, command.max);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCEyePosition& command)
	{
		stream << "$eyeposition ";
		WriteInternal(stream, Vec3D(command.position.y, command.position.x, command.position.z));
	}
}  // namespace QCv14
