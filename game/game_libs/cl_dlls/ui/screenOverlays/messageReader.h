#pragma once

#include "messages/baseMessageReader.h"
#include "screenOverlays/screenOverlayIds.h"

namespace ScreenOverlays
{
	class CMessageReader : public Messages::CBaseMessageReader
	{
	public:
		OverlayId Id() const;

	protected:
		virtual bool ReadMessageInternal(void* buffer, int size) override;
		virtual void ResetInternalData() override;

	private:
		OverlayId m_OverlayId = ScreenOverlays::OverlayId::Overlay_None;
	};
}  // namespace ScreenOverlays
