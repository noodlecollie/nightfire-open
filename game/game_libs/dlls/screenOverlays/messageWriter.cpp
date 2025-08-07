#include "messageWriter.h"
#include "standard_includes.h"

namespace ScreenOverlays
{
	CMessageWriter::CMessageWriter() :
		CBaseMessageWriter(MESSAGE_NAME, StaticUserMessageId())
	{
	}

	ScreenOverlays::OverlayId CMessageWriter::Id() const
	{
		return m_Id;
	}

	void CMessageWriter::SetId(ScreenOverlays::OverlayId id)
	{
		m_Id = id;
	}

	void CMessageWriter::WriteMessage()
	{
		WriteMessageWrapper(
			[this]()
			{
				WRITE_BYTE(static_cast<uint8_t>(m_Id));
			});
	}
}  // namespace ScreenOverlays
