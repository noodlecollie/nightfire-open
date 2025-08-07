#pragma once

#include "messages/baseMessageWriter.h"
#include "messages/messageRegistrationHelper.h"
#include "screenOverlays/screenOverlayIds.h"

namespace ScreenOverlays
{
	class CMessageWriter :
		public Messages::CBaseMessageWriter,
		public Messages::CMessageRegistrationHelper<CMessageWriter>
	{
	public:
		static constexpr const char* MESSAGE_NAME = ScreenOverlays::MESSAGE_NAME;
		static constexpr int MESSAGE_SIZE = 1;

		CMessageWriter();

		ScreenOverlays::OverlayId Id() const;
		void SetId(ScreenOverlays::OverlayId id);

		void WriteMessage();

	private:
		ScreenOverlays::OverlayId m_Id = ScreenOverlays::Overlay_None;
	};
}  // namespace ScreenOverlays
