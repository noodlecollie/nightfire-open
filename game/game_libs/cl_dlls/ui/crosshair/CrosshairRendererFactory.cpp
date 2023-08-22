#include "ui/crosshair/CrosshairRendererFactory.h"
#include "ui/crosshair/QuadLineCrosshairRenderer.h"
#include "ui/crosshair/CircleCrosshairRenderer.h"

// Renderers returned here are static, so that their geometry doesn't need to be
// re-initialised each time we switch to a weapon with a different crosshair style.
std::shared_ptr<CBaseCrosshairRenderer> GetCrosshairRenderer(WeaponAtts::CrosshairStyle style)
{
	switch ( style )
	{
		case WeaponAtts::CrosshairStyle::QuadLine:
		{
			static std::shared_ptr<CQuadLineCrosshairRenderer> quadLineRenderer =
				std::make_shared<CQuadLineCrosshairRenderer>();

			return std::static_pointer_cast<CBaseCrosshairRenderer>(quadLineRenderer);
		}

		case WeaponAtts::CrosshairStyle::Circle:
		{
			static std::shared_ptr<CCircleCrosshairRenderer> circleRenderer =
				std::make_shared<CCircleCrosshairRenderer>();

			return std::static_pointer_cast<CBaseCrosshairRenderer>(circleRenderer);
		}

		default:
		{
			return std::shared_ptr<CBaseCrosshairRenderer>();
		}
	}
}
