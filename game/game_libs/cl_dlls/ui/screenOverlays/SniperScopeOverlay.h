#pragma once

#include "BaseScreenOverlay.h"
#include "standard_includes.h"
#include "EnginePublicAPI/cdll_int.h"
#include "customGeometry/geometryItem.h"
#include "ui/core/uiVec2.h"

namespace ScreenOverlays
{
	class CSniperScopeOverlay : public CBaseScreenOverlay
	{
	public:
		static constexpr ScreenOverlays::OverlayId OVERLAY_ID = ScreenOverlays::Overlay_SniperScope;

		virtual ~CSniperScopeOverlay();

		virtual ScreenOverlays::OverlayId Id() const override;
		virtual void VidInit() override;
		virtual void Draw(float time) override;

	private:
		CustomGeometry::GeometryItemPtr_t CreateNewGeometryItem(CustomGeometry::DrawType drawType);
		void CalculateScopeSpriteParameters();
		void ConstructGeometry();
		void ConstructSurroundingBlocks();
		void ConstructScopeRing();
		void ConstructCrosshairItems();
		void ConstructCrosshair();
		void ConstructExtraBars();
		void ConstructSpikes();

		void DrawScopeBackgroundSprite();

		UIVec2 m_ScreenDim;
		UIVec2 m_ScreenHalfDim;
		HSPRITE m_hScopeImage = 0;
		CustomGeometry::GeometryItemPtr_t m_SurroundingBlocks;
		CustomGeometry::GeometryItemPtr_t m_ScopeRing;
		CustomGeometry::GeometryItemPtr_t m_CrosshairItems;

		// Convenience values for each edge of the scope sprite.
		UIVec2 m_ScopeImagePos;
		UIVec2 m_ScopeImageDim;
	};
}  // namespace ScreenOverlays
