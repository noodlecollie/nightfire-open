#include "messageReader.h"
#include "parsemsg.h"

namespace ScreenOverlays
{
	OverlayId CMessageReader::Id() const
	{
		return m_OverlayId;
	}

	bool CMessageReader::ReadMessageInternal(void* buffer, int size)
	{
		BEGIN_READ(buffer, size);

		const uint8_t overlayIdInt = static_cast<uint8_t>(READ_BYTE());

		if ( overlayIdInt >= ScreenOverlays::OverlayId::Overlay__Count )
		{
			SetErrorString("Unrecognised overlay ID.");
			return false;
		}

		m_OverlayId = static_cast<OverlayId>(overlayIdInt);
		return true;
	}

	void CMessageReader::ResetInternalData()
	{
		Messages::CBaseMessageReader::ResetInternalData();

		m_OverlayId = OverlayId::Overlay_None;
	}
}  // namespace ScreenOverlays
