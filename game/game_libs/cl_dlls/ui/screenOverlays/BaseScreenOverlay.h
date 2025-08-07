#pragma once

#include "screenOverlays/screenOverlayIds.h"

namespace ScreenOverlays
{
	class CBaseScreenOverlay
	{
	public:
		virtual ~CBaseScreenOverlay();

		// Unique ID for this overlay.
		virtual ScreenOverlays::OverlayId Id() const = 0;

		// Called when video settings change, eg. screen resolution.
		// Loading of sprites and construction of relevant screen geometry should happen here.
		virtual void VidInit();

		// Called when the overlay should be drawn.
		// Intended for drawing contents that have been constructed in VidInit.
		virtual void Draw(float time);
	};
}  // namespace ScreenOverlays
