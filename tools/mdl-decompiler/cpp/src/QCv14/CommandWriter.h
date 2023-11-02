#pragma once

#include <ostream>
#include <type_traits>
#include "QCv14/QCCommands.h"
#include "QCv14/QCECommands.h"

namespace QCv14
{
	class CommandWriter
	{
	public:
		template<typename T>
		void Write(std::ostream& stream, const T& command)
		{
			WriteLine(stream, command);
		}

		template<typename T>
		typename std::enable_if<std::is_base_of<QCBaseCommand, T>::value, void>::type WriteQCCommand(
			std::ostream& stream,
			const T& command)
		{
			if ( command.IsValid() )
			{
				WriteLine(stream, command);
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

		void WriteInternal(std::ostream& stream, const float value);
		void WriteInternal(std::ostream& stream, const Vec3D& position);
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
		void WriteInternal(std::ostream& stream, const QCGamma& command);
		void WriteInternal(std::ostream& stream, const QCScale& command);
		void WriteInternal(std::ostream& stream, const QCTypeFlags& command);

		size_t m_Indent = 0;
	};
}  // namespace QCv14
