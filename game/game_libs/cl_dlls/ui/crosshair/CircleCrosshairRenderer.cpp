#include "ui/crosshair/CircleCrosshairRenderer.h"
#include "ui/crosshair/crosshairParameters.h"
#include "customGeometry/sharedDefs.h"
#include "customGeometry/geometryItem.h"
#include "customGeometry/geometryStatics.h"
#include "customGeometry/constructors/circle2DConstructor.h"

CCircleCrosshairRenderer::CCircleCrosshairRenderer() :
	CBaseCrosshairRenderer(WeaponAtts::CrosshairStyle::Circle)
{
	CustomGeometry::CCircle2DConstructor constructor;

	// As a slight hack, we set these default values for a unit circle.
	// We can then adjust the points later.
	constructor.SetCentre(Vector2D(0, 0));
	constructor.SetRadius(1.0f);
	constructor.SetWidth(0.0f);
	constructor.SetNumDivisions(NUM_DIVISIONS);

	CustomGeometry::GeometryItemPtr_t circleGeom = constructor.Construct();

	if ( !circleGeom )
	{
		// Something went wrong - this should not happen.
		ASSERTSZ(false, "Failed to construct circle crosshair geometry!");
		return;
	}

	m_Geometry = circleGeom;
	m_Geometry->SetColour(CCrosshairParameters::DEFAULT_COLOUR);

	// Cache the unit circle point values for later use.
	m_CirclePoints.EnsureCapacity(static_cast<int>(m_Geometry->GetPointCount()));

	for ( size_t index = 0; index < m_Geometry->GetPointCount(); ++index )
	{
		m_CirclePoints.AddToTail(m_Geometry->GetPoint(index));
	}
}

void CCircleCrosshairRenderer::Initialise(const CCrosshairParameters&)
{
	// Nothing left do here - all points will be updated each time Update() is called.
}
void CCircleCrosshairRenderer::Update(const CCrosshairParameters& params)
{
	const UIVec2 screenCentre = params.HalfScreenDimensions();
	const Vector screenCentrePoint(static_cast<float>(screenCentre.x), static_cast<float>(screenCentre.y), 0.0f);
	const float innerDisp = static_cast<float>(params.DisplacementFromScreenCentre(params.Radius()));
	const float thickness = params.Thickness();
	const float outerDisp = innerDisp + thickness;
	const size_t pointCount = m_CirclePoints.Count() / 2;

	for ( size_t index = 0; index < pointCount; ++index )
	{
		// We encounter an outer point followed by an inner point each time.
		m_Geometry->GetPoint(2 * index) =
			screenCentrePoint + (m_CirclePoints.Element(static_cast<int>(2 * index)) * outerDisp);

		m_Geometry->GetPoint((2 * index) + 1) =
			screenCentrePoint + (m_CirclePoints.Element(static_cast<int>((2 * index) + 1)) * innerDisp);
	}
}

void CCircleCrosshairRenderer::Draw()
{
	CustomGeometry::RenderAdHocGeometry(m_Geometry);
}
