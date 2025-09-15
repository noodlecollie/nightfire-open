#include "ui/crosshair/QuadLineCrosshairRenderer.h"
#include "ui/crosshair/crosshairParameters.h"
#include "customGeometry/sharedDefs.h"
#include "customGeometry/geometryItem.h"
#include "customGeometry/geometryStatics.h"

// Crosshair bars are added in this order.
// The first two points of each bar are inner, and the second two are outer.
enum CrosshairBar
{
	TopBar = 0,
	BottomBar,
	LeftBar,
	RightBar
};

static inline constexpr uint8_t PointOffset(CrosshairBar bar)
{
	return static_cast<uint8_t>(4 * bar);
}

CQuadLineCrosshairRenderer::CQuadLineCrosshairRenderer() :
	CBaseCrosshairRenderer(WeaponAtts::CrosshairStyle::QuadLine)
{
	m_Geometry->ClearGeometry();

	m_Geometry->SetColour(CCrosshairParameters::DEFAULT_COLOUR);
	m_Geometry->SetDrawType(CustomGeometry::DrawType::Triangles);

	// Compute crosshair spokes. The positions will be manually modified later
	// according to the parameters we have.
	// The first two points of each bar are inner, and the second two are outer.

	// Top
	m_Geometry->AddTriangleQuad(Vector(-1, -1.0f, 0), Vector(1, -1.0f, 0), Vector(1, -2.0f, 0), Vector(-1, -2.0f, 0));

	// Bottom
	m_Geometry->AddTriangleQuad(Vector(1, 1.0f, 0), Vector(-1, 1.0f, 0), Vector(-1, 2.0f, 0), Vector(1, 2.0f, 0));

	// Left
	m_Geometry->AddTriangleQuad(Vector(-1.0f, 1, 0), Vector(-1.0f, -1, 0), Vector(-2.0f, -1, 0), Vector(-2.0f, 1, 0));

	// Right
	m_Geometry->AddTriangleQuad(Vector(1.0f, -1, 0), Vector(1.0f, 1, 0), Vector(2.0f, 1, 0), Vector(2.0f, -1, 0));
}

void CQuadLineCrosshairRenderer::Initialise(const CCrosshairParameters& params)
{
	const UIVec2 screenCentre = params.HalfScreenDimensions();
	const float halfThickness = params.Thickness() / 2.0f;

	// Centre the relevant co-ordinates based on the screen dimensions.
	for ( uint8_t bar = 0; bar < 4; ++bar )
	{
		const uint8_t base = PointOffset(static_cast<CrosshairBar>(bar));

		switch ( bar )
		{
			case TopBar:
			{
				m_Geometry->GetPoint(base + 0).x = static_cast<float>(screenCentre.x - halfThickness);
				m_Geometry->GetPoint(base + 1).x = static_cast<float>(screenCentre.x + halfThickness);
				m_Geometry->GetPoint(base + 2).x = static_cast<float>(screenCentre.x + halfThickness);
				m_Geometry->GetPoint(base + 3).x = static_cast<float>(screenCentre.x - halfThickness);
				break;
			}

			case BottomBar:
			{
				m_Geometry->GetPoint(base + 0).x = static_cast<float>(screenCentre.x + halfThickness);
				m_Geometry->GetPoint(base + 1).x = static_cast<float>(screenCentre.x - halfThickness);
				m_Geometry->GetPoint(base + 2).x = static_cast<float>(screenCentre.x - halfThickness);
				m_Geometry->GetPoint(base + 3).x = static_cast<float>(screenCentre.x + halfThickness);
				break;
			}

			case LeftBar:
			{
				m_Geometry->GetPoint(base + 0).y = static_cast<float>(screenCentre.y + halfThickness);
				m_Geometry->GetPoint(base + 1).y = static_cast<float>(screenCentre.y - halfThickness);
				m_Geometry->GetPoint(base + 2).y = static_cast<float>(screenCentre.y - halfThickness);
				m_Geometry->GetPoint(base + 3).y = static_cast<float>(screenCentre.y + halfThickness);
				break;
			}

			case RightBar:
			{
				m_Geometry->GetPoint(base + 0).y = static_cast<float>(screenCentre.y - halfThickness);
				m_Geometry->GetPoint(base + 1).y = static_cast<float>(screenCentre.y + halfThickness);
				m_Geometry->GetPoint(base + 2).y = static_cast<float>(screenCentre.y + halfThickness);
				m_Geometry->GetPoint(base + 3).y = static_cast<float>(screenCentre.y - halfThickness);
				break;
			}
		}
	}
}

void CQuadLineCrosshairRenderer::Update(const CCrosshairParameters& params)
{
	if ( m_Geometry->GetPointCount() != 4 * 4 )
	{
		return;
	}

	const UIVec2 screenCentre = params.HalfScreenDimensions();
	const int innerDisp = params.DisplacementFromScreenCentre(params.Radius());
	const float barLength = params.BarLength();
	const int outerDisp = params.DisplacementFromScreenCentre(params.Radius() + barLength);

	// Move the relevant co-ordinates based on the displacement.
	for ( uint8_t bar = 0; bar < 4; ++bar )
	{
		const uint8_t base = PointOffset(static_cast<CrosshairBar>(bar));

		switch ( bar )
		{
			case TopBar:
			{
				m_Geometry->GetPoint(base + 0).y = static_cast<float>(screenCentre.y - innerDisp);
				m_Geometry->GetPoint(base + 1).y = static_cast<float>(screenCentre.y - innerDisp);
				m_Geometry->GetPoint(base + 2).y = static_cast<float>(screenCentre.y - outerDisp);
				m_Geometry->GetPoint(base + 3).y = static_cast<float>(screenCentre.y - outerDisp);
				break;
			}

			case BottomBar:
			{
				m_Geometry->GetPoint(base + 0).y = static_cast<float>(screenCentre.y + innerDisp);
				m_Geometry->GetPoint(base + 1).y = static_cast<float>(screenCentre.y + innerDisp);
				m_Geometry->GetPoint(base + 2).y = static_cast<float>(screenCentre.y + outerDisp);
				m_Geometry->GetPoint(base + 3).y = static_cast<float>(screenCentre.y + outerDisp);
				break;
			}

			case LeftBar:
			{
				m_Geometry->GetPoint(base + 0).x = static_cast<float>(screenCentre.x - innerDisp);
				m_Geometry->GetPoint(base + 1).x = static_cast<float>(screenCentre.x - innerDisp);
				m_Geometry->GetPoint(base + 2).x = static_cast<float>(screenCentre.x - outerDisp);
				m_Geometry->GetPoint(base + 3).x = static_cast<float>(screenCentre.x - outerDisp);
				break;
			}

			case RightBar:
			{
				m_Geometry->GetPoint(base + 0).x = static_cast<float>(screenCentre.x + innerDisp);
				m_Geometry->GetPoint(base + 1).x = static_cast<float>(screenCentre.x + innerDisp);
				m_Geometry->GetPoint(base + 2).x = static_cast<float>(screenCentre.x + outerDisp);
				m_Geometry->GetPoint(base + 3).x = static_cast<float>(screenCentre.x + outerDisp);
				break;
			}
		}
	}
}

void CQuadLineCrosshairRenderer::Draw()
{
	CustomGeometry::RenderAdHocGeometry(m_Geometry);
}
