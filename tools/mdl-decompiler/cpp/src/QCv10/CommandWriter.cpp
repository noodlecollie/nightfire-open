#include <iomanip>
#include <cmath>
#include "QCv10/CommandWriter.h"
#include "Conversions/Activity.h"
#include "Conversions/MotionFlags.h"
#include "Common/StreamHelpers.h"

namespace QCv10
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
		stream << "$qceversion " << static_cast<int32_t>(command.game) << "." << command.version << "."
			   << command.versionMajor << "." << command.versionMinor;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCEReplaceActivity& command)
	{
		stream << "$replaceactivity \"" << command.sequenceName << "\" " << command.activity;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCModelName& command)
	{
		stream << "$modelname \"" << command.name << "\"";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCAttachment& command)
	{
		stream << "$attachment \"" << command.name << "\" \"" << command.bone << "\" ";
		WriteVec3D(stream, command.position);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCBBox& command)
	{
		stream << "$bbox ";
		WriteVec3D(stream, command.min);
		stream << " ";
		WriteVec3D(stream, command.max);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCCBox& command)
	{
		stream << "$cbox ";
		WriteVec3D(stream, command.min);
		stream << " ";
		WriteVec3D(stream, command.max);
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
			   << Conversion::MotionFlagShortName(command.motionType) << " " << command.start << " " << command.end;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCHitBox& command)
	{
		stream << "$hbox " << command.group << " \"" << command.bone << "\" ";
		WriteVec3D(stream, command.min);
		stream << " ";
		WriteVec3D(stream, command.max);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCEyePosition& command)
	{
		stream << "$eyeposition ";
		WriteVec3D(stream, Vec3D(command.position.y, command.position.x, command.position.z));
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

	void CommandWriter::WriteInternal(std::ostream& stream, const QCScale& command)
	{
		stream << "$scale " << command.value;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCTypeFlags& command)
	{
		stream << "$flags " << command.flags;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCOrigin& command)
	{
		stream << "$origin ";
		WriteVec3D(stream, command.pos);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCTextureGroup& command)
	{
		stream << "$texturegroup \"" << command.name << "\"" << std::endl;
		stream << IndentString() << "{" << std::endl;

		IncreaseIndent();

		for ( const Container<std::string>& list : command.skins )
		{
			stream << IndentString() << "{ ";

			for ( size_t index = 0; index < list.size(); ++index )
			{
				if ( index > 0 )
				{
					stream << " ";
				}

				stream << "\"" << list.GetElementChecked(index) << "\"";
			}

			stream << " }" << std::endl;
		}

		DecreaseIndent();

		stream << IndentString() << "}";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCBody& command)
	{
		stream << "$body \"" << command.name << "\" \"" << command.file << "\"";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCSequence& command)
	{
		stream << "$sequence \"" + command.name + "\"";

		for ( const std::string& file : command.files )
		{
			stream << " \"" << file << "\"";
		}

		if ( command.activity.IsValid() )
		{
			stream << " ";
			WriteInternal(stream, command.activity);
		}

		for ( const QCv10::QCOptionBlend& blend : command.blends )
		{
			stream << " ";
			WriteInternal(stream, blend);
		}

		if ( command.controlFlags != CommonTypes::MotionFlag_None )
		{
			stream << " ";
			WriteInternal(stream, command.controlFlags);
		}

		stream << " fps " << command.framesPerSecond;

		if ( command.loop )
		{
			stream << " loop";
		}

		if ( command.nodeEntryBone >= 0 )
		{
			stream << " node " << static_cast<int32_t>(command.nodeEntryBone);
		}

		if ( command.transition.IsValid() )
		{
			stream << " ";
			WriteInternal(stream, command.transition);
		}

		if ( !command.pivots.empty() )
		{
			for ( const QCv10::QCOptionPivot& pivot : command.pivots )
			{
				stream << " ";
				WriteInternal(stream, pivot);
			}
		}

		if ( !command.events.empty() )
		{
			stream << IndentString() << " {" << std::endl;
			IncreaseIndent();

			for ( const QCv10::QCOptionEvent& event : command.events )
			{
				stream << IndentString();
				WriteInternal(stream, event);
				stream << std::endl;
			}

			DecreaseIndent();
			stream << IndentString() << "}";
		}
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCOptionActivity& command)
	{
		stream << Conversion::ActivityName(command.activity) << " ";
		WriteFixedFloat(stream, command.weight);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCOptionBlend& command)
	{
		stream << "blend " << Conversion::MotionFlagName(command.motionFlags) << " ";
		WriteFixedFloat(stream, command.start);
		stream << " ";
		WriteFixedFloat(stream, command.end);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCOptionEvent& command)
	{
		stream << "event " << command.value << " " << command.frame << " \"" << command.options << "\"";
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCOptionTransition& command)
	{
		stream << (command.reverse ? "rtransition" : "transition") << " " << command.sourceBone << " "
			   << command.targetBone;
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const QCOptionPivot& command)
	{
		stream << "pivot " << command.index << " ";
		WriteFixedFloat(stream, command.start);
		stream << " ";
		WriteFixedFloat(stream, command.end);
	}

	void CommandWriter::WriteInternal(std::ostream& stream, const CommonTypes::MotionFlag& flag)
	{
		stream << Conversion::MotionFlagShortName(flag);
	}
}  // namespace QCv10
