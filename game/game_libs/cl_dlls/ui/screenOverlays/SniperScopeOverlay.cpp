#include "SniperScopeOverlay.h"
#include "cl_dll.h"
#include "hud.h"
#include "cl_util.h"
#include "EnginePublicAPI/triangleapi.h"
#include "customGeometry/geometryStatics.h"
#include "coreutil/coreutil.h"
#include "standard_includes.h"

namespace
{
	// How much of the short edge of the screen should the base scope image take up?
	// This must be in the interval (0 1], otherwise you're in for bad times.
	static constexpr float SCREEN_SHORTEDGE_FACTOR = 0.95f;

	// How many line segments should be used to create the scope ring circle.
	static constexpr size_t SCOPE_RING_DIVISIONS = 40;

	// What proportion of the scope image the ring diameter should take up.
	static constexpr float SCOPE_RING_FACTOR = 0.6f;

	// How many pixels thick the scope ring should be.
	static constexpr float SCOPE_RING_WIDTH = 3;

	// How many pixels thick the crosshair lines should be.
	static constexpr size_t CROSSHAIR_WIDTH = 1;

	// Height of the extra bars on the horizontal crosshair line.
	static constexpr size_t EXTRABAR_HEIGHT = 75;

	// Width of these bars.
	static constexpr size_t EXTRABAR_WIDTH = 2;

	// How far the bars are away from the centre of the crosshair.
	// 0 = at the centre, 1 = at the edge of the crosshair image.
	static constexpr float EXTRABAR_DISPLACEMENT_FACTOR = 0.5f;

	// Width of the spikes that are placed around the scope circle.
	// Should be an odd number, so that the triangle tip lines up with the crosshair line.
	static constexpr size_t SPIKE_WIDTH = 33;

	// Height of the spike tip.
	static constexpr size_t SPIKE_TIP_HEIGHT = 20;

	// Height of the whole spike.
	static constexpr size_t SPIKE_TOTAL_HEIGHT = 150;

	static inline void GetCirclePointCoOrds(uint32_t point, size_t segmentCount, float radius, float& x, float& y)
	{
		static constexpr float TWO_PI = 2.0f * static_cast<float>(M_PI);

		const float theta = (TWO_PI * static_cast<float>(point)) / static_cast<float>(segmentCount);
		x = radius * cosf(theta);
		y = radius * sinf(theta);
	}

	static inline Vector P2V(int x, int y)
	{
		return Vector(static_cast<float>(x), static_cast<float>(y), 0);
	}

	static inline Vector P2V(const UIVec2& p)
	{
		return P2V(p.x, p.y);
	}

	static inline Vector S2V(size_t x, size_t y)
	{
		return Vector(static_cast<float>(x), static_cast<float>(y), 0);
	}

	static inline void
	AddPointsToGeometryItem(const CustomGeometry::GeometryItemPtr_t& item, const UIVec2* array, size_t length)
	{
		for ( uint32_t index = 0; index < length; ++index )
		{
			item->AddPoint(P2V(array[index]));
		}
	}
}  // namespace

namespace ScreenOverlays
{
	CSniperScopeOverlay::~CSniperScopeOverlay()
	{
	}

	ScreenOverlays::OverlayId CSniperScopeOverlay::Id() const
	{
		return OVERLAY_ID;
	}

	void CSniperScopeOverlay::VidInit()
	{
		m_hScopeImage = SPR_Load("sprites/sniper_scope.spr");

		SCREENINFO screenInfo;
		memset(&screenInfo, 0, sizeof(screenInfo));

		// This API is really weird, dude. We should write something that just return the screen dimensions as
		// integers...
		screenInfo.iSize = sizeof(screenInfo);

		gEngfuncs.pfnGetScreenInfo(&screenInfo);

		m_ScreenDim.x = screenInfo.iWidth;
		m_ScreenDim.y = screenInfo.iHeight;

		// We take away 1 so that the screen centre matches what the engine crosshair sprites actually use.
		m_ScreenHalfDim = (m_ScreenDim / 2) - UIVec2(1, 1);

		CalculateScopeSpriteParameters();
		ConstructGeometry();
	}

	void CSniperScopeOverlay::ConstructGeometry()
	{
		ConstructSurroundingBlocks();
		ConstructScopeRing();
		ConstructCrosshairItems();
	}

	void CSniperScopeOverlay::Draw(float)
	{
		DrawScopeBackgroundSprite();
		CustomGeometry::RenderAdHocGeometry(m_SurroundingBlocks);
		CustomGeometry::RenderAdHocGeometry(m_ScopeRing);
		CustomGeometry::RenderAdHocGeometry(m_CrosshairItems);
	}

	void CSniperScopeOverlay::DrawScopeBackgroundSprite()
	{
		SPR_Set(m_hScopeImage, 255, 255, 255);

		sprite_draw_args_t args;
		memset(&args, 0, sizeof(args));

		args.x = m_ScopeImagePos.x;
		args.y = m_ScopeImagePos.y;
		args.width = m_ScopeImageDim.x;
		args.height = m_ScopeImageDim.y;
		args.renderMode = kRenderTransTexture;

		SPR_DrawCustom(&args);
	}

	void CSniperScopeOverlay::ConstructSurroundingBlocks()
	{
		m_SurroundingBlocks = CreateNewGeometryItem(CustomGeometry::DrawType::TriangleStrip);

		UIVec2 points[8];

		// Points are added in the correct order for a triangle strip that surrounds the scope sprite.
		// The strip begins at the top left and alternates between outer and inner points, proceeding anticlockwise.
		points[0] = UIVec2(0, 0);
		points[2] = UIVec2(0, m_ScreenDim.y);
		points[4] = m_ScreenDim;
		points[6] = UIVec2(m_ScreenDim.x, 0);

		points[1] = m_ScopeImagePos;
		points[3] = m_ScopeImagePos + UIVec2(0, m_ScopeImageDim.y);
		points[5] = m_ScopeImagePos + m_ScopeImageDim;
		points[7] = m_ScopeImagePos + UIVec2(m_ScopeImageDim.x, 0);

		// Add points and indices to the geometry.
		for ( uint8_t index = 0; index < CoreUtil::ArraySize(points); ++index )
		{
			m_SurroundingBlocks->AddPoint(P2V(points[index].x, points[index].y));
			m_SurroundingBlocks->AddIndex(index);
		}

		// We need two extra indices to form the last two triangles.
		m_SurroundingBlocks->AddIndex(0);
		m_SurroundingBlocks->AddIndex(1);
	}

	void CSniperScopeOverlay::ConstructScopeRing()
	{
		m_ScopeRing = CreateNewGeometryItem(CustomGeometry::DrawType::TriangleStrip);

		const UIVec2 scopeRingCentre(m_ScopeImagePos + (m_ScopeImageDim / 2));
		const float scopeRingOuterRadius = (static_cast<float>(m_ScopeImageDim.x) * SCOPE_RING_FACTOR) / 2.0f;

		if ( scopeRingOuterRadius < 1.0f )
		{
			return;
		}

		const float scopeRingInnerRadius = scopeRingOuterRadius - SCOPE_RING_WIDTH;

		if ( scopeRingInnerRadius < 1.0f )
		{
			return;
		}

		for ( uint32_t index = 0; index < SCOPE_RING_DIVISIONS; ++index )
		{
			float outerX = 0.0f;
			float outerY = 0.0f;
			float innerX = 0.0f;
			float innerY = 0.0f;

			GetCirclePointCoOrds(index, SCOPE_RING_DIVISIONS, scopeRingOuterRadius, outerX, outerY);
			GetCirclePointCoOrds(index, SCOPE_RING_DIVISIONS, scopeRingInnerRadius, innerX, innerY);

			m_ScopeRing->AddPointAndIndex(Vector(scopeRingCentre.x + outerX, scopeRingCentre.y + outerY, 0));
			m_ScopeRing->AddPointAndIndex(Vector(scopeRingCentre.x + innerX, scopeRingCentre.y + innerY, 0));
		}

		// Add the first two points again to complete the triangle strip.
		m_ScopeRing->AddIndex(0);
		m_ScopeRing->AddIndex(1);
	}

	void CSniperScopeOverlay::ConstructCrosshairItems()
	{
		m_CrosshairItems = CreateNewGeometryItem(CustomGeometry::DrawType::Triangles);

		ConstructCrosshair();
		ConstructExtraBars();
		ConstructSpikes();
	}

	void CSniperScopeOverlay::ConstructCrosshair()
	{
		// Vertical line
		m_CrosshairItems->AddTriangleQuad(
			S2V(m_ScreenHalfDim.x, 0),
			S2V(m_ScreenHalfDim.x, m_ScreenDim.y),
			S2V(m_ScreenHalfDim.x + CROSSHAIR_WIDTH, m_ScreenDim.y),
			S2V(m_ScreenHalfDim.x + CROSSHAIR_WIDTH, 0));

		// Horizontal line
		m_CrosshairItems->AddTriangleQuad(
			S2V(0, m_ScreenHalfDim.y),
			S2V(0, m_ScreenHalfDim.y + CROSSHAIR_WIDTH),
			S2V(m_ScreenDim.x, m_ScreenHalfDim.y + CROSSHAIR_WIDTH),
			S2V(m_ScreenDim.x, m_ScreenHalfDim.y));
	}

	void CSniperScopeOverlay::ConstructExtraBars()
	{
		const size_t displacementFromCentre =
			static_cast<size_t>(EXTRABAR_DISPLACEMENT_FACTOR * (static_cast<float>(m_ScopeImageDim.x) / 2.0f));

		// Left bar
		m_CrosshairItems->AddTriangleQuad(
			S2V(m_ScreenHalfDim.x - displacementFromCentre - EXTRABAR_WIDTH, m_ScreenHalfDim.y - (EXTRABAR_HEIGHT / 2)),
			S2V(m_ScreenHalfDim.x - displacementFromCentre - EXTRABAR_WIDTH, m_ScreenHalfDim.y + (EXTRABAR_HEIGHT / 2)),
			S2V(m_ScreenHalfDim.x - displacementFromCentre, m_ScreenHalfDim.y + (EXTRABAR_HEIGHT / 2)),
			S2V(m_ScreenHalfDim.x - displacementFromCentre, m_ScreenHalfDim.y - (EXTRABAR_HEIGHT / 2)));

		// Right bar
		m_CrosshairItems->AddTriangleQuad(
			S2V(m_ScreenHalfDim.x + displacementFromCentre, m_ScreenHalfDim.y - (EXTRABAR_HEIGHT / 2)),
			S2V(m_ScreenHalfDim.x + displacementFromCentre, m_ScreenHalfDim.y + (EXTRABAR_HEIGHT / 2)),
			S2V(m_ScreenHalfDim.x + displacementFromCentre + EXTRABAR_WIDTH, m_ScreenHalfDim.y + (EXTRABAR_HEIGHT / 2)),
			S2V(m_ScreenHalfDim.x + displacementFromCentre + EXTRABAR_WIDTH,
				m_ScreenHalfDim.y - (EXTRABAR_HEIGHT / 2)));
	}

	void CSniperScopeOverlay::ConstructSpikes()
	{
		// Iteration 0 is top and bottom, iteration 1 is left and right.
		for ( uint32_t iteration = 0; iteration < 2; ++iteration )
		{
			static constexpr size_t HALF_SPIKE_WIDTH = SPIKE_WIDTH / 2;
			static constexpr size_t NUM_POINTS_IN_SPIKE = 5;

			UIVec2 minPoints[NUM_POINTS_IN_SPIKE];
			UIVec2 maxPoints[NUM_POINTS_IN_SPIKE];

			const size_t xIndex = iteration == 0 ? 0 : 1;
			const size_t yIndex = iteration == 0 ? 1 : 0;

			// Spike at lower extent of the axis:
			minPoints[0][xIndex] = m_ScreenHalfDim[xIndex] - HALF_SPIKE_WIDTH;
			minPoints[0][yIndex] = m_ScopeImagePos[yIndex];

			minPoints[1][xIndex] = minPoints[0][xIndex];
			minPoints[1][yIndex] = minPoints[0][yIndex] + SPIKE_TOTAL_HEIGHT - SPIKE_TIP_HEIGHT;

			minPoints[2][xIndex] = minPoints[1][xIndex] + SPIKE_WIDTH;
			minPoints[2][yIndex] = minPoints[1][yIndex];

			minPoints[3][xIndex] = minPoints[2][xIndex];
			minPoints[3][yIndex] = minPoints[0][yIndex];

			minPoints[4][xIndex] = m_ScreenHalfDim[xIndex];
			minPoints[4][yIndex] = minPoints[0][yIndex] + SPIKE_TOTAL_HEIGHT;

			// Spike at upper extent of axis:
			// Note that HALF_SPIKE_WIDTH is not quite half if the width is odd (which it should be).
			// To make sure these points line up with those computed above, we take away half the width
			// for X, and then add the full width.
			maxPoints[0][xIndex] = m_ScreenHalfDim[xIndex] - HALF_SPIKE_WIDTH + SPIKE_WIDTH;
			maxPoints[0][yIndex] = m_ScopeImagePos[yIndex] + m_ScopeImageDim[yIndex];

			maxPoints[1][xIndex] = maxPoints[0][xIndex];
			maxPoints[1][yIndex] = maxPoints[0][yIndex] - (SPIKE_TOTAL_HEIGHT - SPIKE_TIP_HEIGHT);

			maxPoints[2][xIndex] = maxPoints[1][xIndex] - SPIKE_WIDTH;
			maxPoints[2][yIndex] = maxPoints[1][yIndex];

			maxPoints[3][xIndex] = maxPoints[2][xIndex];
			maxPoints[3][yIndex] = maxPoints[0][yIndex];

			maxPoints[4][xIndex] = m_ScreenHalfDim[xIndex];
			maxPoints[4][yIndex] = maxPoints[0][yIndex] - SPIKE_TOTAL_HEIGHT;

			const size_t indexBase = m_CrosshairItems->GetPointCount();

			AddPointsToGeometryItem(m_CrosshairItems, minPoints, NUM_POINTS_IN_SPIKE);
			AddPointsToGeometryItem(m_CrosshairItems, maxPoints, NUM_POINTS_IN_SPIKE);

			for ( uint32_t subIteration = 0; subIteration < 2; ++subIteration )
			{
				const size_t offset = indexBase + (subIteration * NUM_POINTS_IN_SPIKE);

				// Triangle 1 in quad:
				m_CrosshairItems->AddIndex(offset + 0);
				m_CrosshairItems->AddIndex(offset + 1);
				m_CrosshairItems->AddIndex(offset + 2);

				// Triangle 2 in quad:
				m_CrosshairItems->AddIndex(offset + 0);
				m_CrosshairItems->AddIndex(offset + 2);
				m_CrosshairItems->AddIndex(offset + 3);

				// Triangle for spike tip:
				m_CrosshairItems->AddIndex(offset + 1);
				m_CrosshairItems->AddIndex(offset + 4);
				m_CrosshairItems->AddIndex(offset + 2);
			}
		}
	}

	void CSniperScopeOverlay::CalculateScopeSpriteParameters()
	{
		const size_t shortEdge = m_ScreenDim.y < m_ScreenDim.x ? m_ScreenDim.y : m_ScreenDim.x;
		const size_t scopeImageDim = static_cast<size_t>(static_cast<float>(shortEdge) * SCREEN_SHORTEDGE_FACTOR);

		m_ScopeImageDim = UIVec2(static_cast<int>(scopeImageDim), static_cast<int>(scopeImageDim));

		// We do dim + 1 to make sure that the image position is always closer to the top left if the dimension is odd.
		const size_t scopeImageHalfDim = (scopeImageDim + 1) / 2;
		m_ScopeImagePos =
			m_ScreenHalfDim - UIVec2(static_cast<int>(scopeImageHalfDim), static_cast<int>(scopeImageHalfDim));
	}

	CustomGeometry::GeometryItemPtr_t CSniperScopeOverlay::CreateNewGeometryItem(CustomGeometry::DrawType drawType)
	{
		CustomGeometry::GeometryItemPtr_t item(new CustomGeometry::CGeometryItem());
		item->SetColour(0x000000FF);
		item->SetDrawType(drawType);

		return item;
	}
}  // namespace ScreenOverlays
