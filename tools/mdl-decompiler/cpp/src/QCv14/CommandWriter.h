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
			stream << std::endl;
		}

		template<typename CB>
		void WriteBanner(std::ostream& stream, const CB& callback)
		{
			static constexpr const char* const BANNER = "########################################";

			stream << BANNER << std::endl;

			callback(stream);
			stream << std::endl;

			stream << BANNER << std::endl;
		}

		void IncreaseIndent();
		void DecreaseIndent();

	private:
		std::string IndentString() const;

		void WriteInternal(std::ostream& stream, const QCEVersion& command);
		void WriteInternal(std::ostream& stream, const QCEReplaceActivity& command);

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

		size_t m_Indent = 0;
	};
}
