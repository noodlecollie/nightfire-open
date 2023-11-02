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

	void CommandWriter::WriteInternal(std::ostream& stream, const float value)
	{
		const std::streamsize defaultPrecision = stream.precision();
		stream << std::setprecision(6) << (std::isnan(value) ? 0.0f : value) << std::setprecision(defaultPrecision);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const Vec3D& position)
	{
		WriteInternal(stream, position.x);
		stream << " ";
		WriteInternal(stream, position.y);
		stream << " ";
		WriteInternal(stream, position.z);
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

	void CommandWriter::WriteInternal(std::ostream& stream, const QCPivot& command)
	{
		stream << "$pivot " << command.index << " \"" << command.bone << "\"";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCMirrorBone& command)
	{
		stream << "$mirrorbone \"" << command.bone << "\"";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCRenameBone& command)
	{
		stream << "$renamebone \"" << command.oldName << "\" \"" << command.newName << "\"";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCGamma& command)
	{
		stream << "$gamma ";
		WriteInternal(stream, command.value);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCScale& command)
	{
		stream << "$scale ";
		WriteInternal(stream, command.value);
	}
}  // namespace QCv14
