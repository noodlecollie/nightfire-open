#pragma once

#include <memory>
#include "screenOverlays/screenOverlayIds.h"
#include "BaseScreenOverlay.h"

namespace ScreenOverlays
{
	class CScreenOverlayContainer
	{
	public:
		static CScreenOverlayContainer& StaticInstance();

		CScreenOverlayContainer();

		void Initialise();

		// Called whenever the video settings change, eg. monitor resolution.
		// All existing overlays are informed.
		void VidInit();

		// Called when the currently active overlay should be drawn.
		void DrawCurrentOverlay(float time);

		void SetCurrentOverlay(ScreenOverlays::OverlayId id);
		void ResetCurrentOverlay();

	private:
		using FactoryFunc = CBaseScreenOverlay* (*)(void);
		using ScreenOverlayPtr = std::unique_ptr<CBaseScreenOverlay>;

		template<typename T>
		inline void MapIdToClass()
		{
			volatile ScreenOverlays::OverlayId id = T::OVERLAY_ID;
			if ( id > ScreenOverlays::Overlay_None && id < ScreenOverlays::Overlay__Count )
			{
				m_FactoryFunctions[id] = []()
				{
					return static_cast<CBaseScreenOverlay*>(new T());
				};
			}
		}

		template<typename FUNC>
		inline void ForEachValidOverlay(FUNC callback)
		{
			using namespace ScreenOverlays;

			for ( OverlayId id = static_cast<OverlayId>(Overlay_None + 1); id < Overlay__Count;
				  id = static_cast<OverlayId>(id + 1) )
			{
				CBaseScreenOverlay* overlay = GetOverlay(id);

				if ( overlay )
				{
					callback(id, overlay);
				}
			}
		}

		// This is called once on client initialisation.
		// Any overlays supported in the game should be registered within this function.
		void RegisterOverlays();

		void CreateAllOverlays();
		CBaseScreenOverlay* GetOverlay(ScreenOverlays::OverlayId id);

		ScreenOverlayPtr m_Overlays[ScreenOverlays::Overlay__Count];
		FactoryFunc m_FactoryFunctions[ScreenOverlays::Overlay__Count];
		ScreenOverlays::OverlayId m_CurrentOverlay = ScreenOverlays::Overlay_None;
	};
}  // namespace ScreenOverlays
