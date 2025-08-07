#pragma once

#include <ostream>
#include <type_traits>
#include "QCv10/QCCommands.h"
#include "QCv10/QCECommands.h"
#include "QCv10/QCCommandReflection.h"
#include "QCv10/QCECommandReflection.h"

namespace QCv10
{
	class CommandWriter
	{
	public:
		enum class WarnIfInvalid
		{
			No = 0,
			Yes
		};

		template<typename T>
		void Write(std::ostream& stream, const T& command)
		{
			WriteLine(stream, command);
		}

		template<typename T>
		typename std::enable_if<std::is_base_of<QCBaseCommand, T>::value, void>::type
		WriteQCCommand(std::ostream& stream, const T& command, WarnIfInvalid warnIfInvalid = WarnIfInvalid::No)
		{
			if ( command.IsValid() )
			{
				WriteLine(stream, command);
			}
			else if ( warnIfInvalid == WarnIfInvalid::Yes )
			{
				stream << IndentString() << "// <WARNING: Encountered invalid " << Reflection::TypeName<T>()
					   << " data!>" << std::endl;
			}
		}

		void IncreaseIndent();
		void DecreaseIndent();

	private:
		std::string IndentString() const;

		template<typename T>
		void WriteLine(std::ostream& stream, const T& command)
		{
			stream << IndentString();
			WriteInternal(stream, command);
			stream << std::endl;
		}

		void WriteInternal(std::ostream& stream, const QCEVersion& command);
		void WriteInternal(std::ostream& stream, const QCEReplaceActivity& command);

		void WriteInternal(std::ostream& stream, const QCModelName& command);
		void WriteInternal(std::ostream& stream, const QCAttachment& command);
		void WriteInternal(std::ostream& stream, const QCBBox& command);
		void WriteInternal(std::ostream& stream, const QCCBox& command);
		void WriteInternal(std::ostream& stream, const QCBodyGroupItem& command);
		void WriteInternal(std::ostream& stream, const QCBodyGroup& command);
		void WriteInternal(std::ostream& stream, const QCCD& command);
		void WriteInternal(std::ostream& stream, const QCCDTexture& command);
		void WriteInternal(std::ostream& stream, const QCClipToTextures& command);
		void WriteInternal(std::ostream& stream, const QCExternalTextures& command);
		void WriteInternal(std::ostream& stream, const QCRoot& command);
		void WriteInternal(std::ostream& stream, const QCBoneController& command);
		void WriteInternal(std::ostream& stream, const QCHitBox& command);
		void WriteInternal(std::ostream& stream, const QCEyePosition& command);
		void WriteInternal(std::ostream& stream, const QCPivot& command);
		void WriteInternal(std::ostream& stream, const QCMirrorBone& command);
		void WriteInternal(std::ostream& stream, const QCRenameBone& command);
		void WriteInternal(std::ostream& stream, const QCScale& command);
		void WriteInternal(std::ostream& stream, const QCTypeFlags& command);
		void WriteInternal(std::ostream& stream, const QCOrigin& command);
		void WriteInternal(std::ostream& stream, const QCTextureGroup& command);
		void WriteInternal(std::ostream& stream, const QCBody& command);
		void WriteInternal(std::ostream& stream, const QCSequence& command);
		void WriteInternal(std::ostream& stream, const QCOptionActivity& command);
		void WriteInternal(std::ostream& stream, const QCOptionBlend& command);
		void WriteInternal(std::ostream& stream, const QCOptionEvent& command);
		void WriteInternal(std::ostream& stream, const QCOptionTransition& command);
		void WriteInternal(std::ostream& stream, const QCOptionPivot& command);

		void WriteInternal(std::ostream& stream, const CommonTypes::MotionFlag& flag);

		size_t m_Indent = 0;
	};
}  // namespace QCv10
