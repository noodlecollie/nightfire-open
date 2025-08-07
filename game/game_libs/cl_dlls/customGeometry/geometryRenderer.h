#pragma once

#include "customGeometry/geometryItem.h"
#include "EnginePublicAPI/cdll_int.h"

namespace CustomGeometry
{
	class CGeometryRenderer
	{
	public:
		CGeometryRenderer();

		void VidInit();
		void Render(const CGeometryItem& item);

	private:
		void Prepare(int renderMode, uint32_t colour);
		void RenderGeometry(const CGeometryItem& item);
		void RenderText(const CGeometryItem& item);
		void DrawLines(const CGeometryItem& item);
		void DrawTriangleFan(const CGeometryItem& item);
		void DrawTriangleStrip(const CGeometryItem& item);
		void DrawTriangles(const CGeometryItem& item);
		void DrawTriangleChainGeneric(const CGeometryItem& item, int glDrawMode);

		HSPRITE m_ColourSprite = 0;
		float m_Scale = 1.0f;
	};
}  // namespace CustomGeometry
