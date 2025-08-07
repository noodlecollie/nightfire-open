#pragma once

namespace ScreenOverlays
{
	static constexpr const char* MESSAGE_NAME = "ActiveScreenOverlay";

	enum OverlayId
	{
		Overlay_None = 0,
		Overlay_SniperScope,

		Overlay__Count
	};

	static inline OverlayId ToOverlayId(int value)
	{
		return (value > Overlay_None && value < Overlay__Count) ? static_cast<OverlayId>(value) : Overlay_None;
	}
}  // namespace ScreenOverlays
