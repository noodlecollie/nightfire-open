#include "geometryRenderer.h"
#include "cl_dll.h"
#include "triangleapi.h"
#include "debug_assert.h"
#include "hud.h"
#include "cl_util.h"

namespace
{
	static inline bool FloatIsZero(float f)
	{
		return fabs(f) < 0.0001f;
	}

	static inline void DefaultTextureCoOrdinate()
	{
		gEngfuncs.pTriAPI->TexCoord2f(0.0f, 0.0f);
	}
}

namespace CustomGeometry
{
	CGeometryRenderer::CGeometryRenderer()
	{
	}

	void CGeometryRenderer::VidInit()
	{
		m_ColourSprite = SPR_Load("sprites/white.spr");
	}

	void CGeometryRenderer::Render(const CGeometryItem& item)
	{
		if ( m_ColourSprite == 0 )
		{
			gEngfuncs.Con_Printf("CGeometryRenderer: Colour sprite not initialised!\n");
			return;
		}

		m_Scale = item.GetScale();

		if ( FloatIsZero(m_Scale) )
		{
			return;
		}

		const CUtlVector<Vector>& points = item.GetPoints();

		if ( points.Count() < 1 )
		{
			return;
		}

		switch ( item.GetDrawType() )
		{
			case DrawType::Lines:
			{
				DrawLines(item);
				break;
			}

			case DrawType::TriangleFan:
			{
				DrawTriangleFan(item);
				break;
			}

			case DrawType::TriangleStrip:
			{
				DrawTriangleStrip(item);
				break;
			}

			case DrawType::Triangles:
			{
				DrawTriangles(item);
				break;
			}

			default:
			{
				break;
			}
		}
	}

	void CGeometryRenderer::Prepare(int renderMode, uint32_t colour)
	{
		// This sprite is used as a base texture which we then set the colour of.
		// For some reason lines don't draw correctly unless a texture is bound...
		const model_s* colourSprite = gEngfuncs.GetSpritePointer(m_ColourSprite);

		if ( !colourSprite )
		{
			return;
		}

		// UGH. Awful HL const-incorrectness again.
		gEngfuncs.pTriAPI->SpriteTexture(const_cast<model_s*>(colourSprite), 0);
		gEngfuncs.pTriAPI->RenderMode(renderMode);
		gEngfuncs.pTriAPI->Color4ub((colour & 0xFF000000) >> 24,
									(colour & 0x00FF0000) >> 16,
									(colour & 0x0000FF00) >> 8,
									(colour & 0x000000FF) >> 0);
	}

	void CGeometryRenderer::DrawLines(const CGeometryItem& item)
	{
		const CUtlVector<Vector>& points = item.GetPoints();
		const CUtlVector<uint8_t>& indices = item.GetIndices();
		const size_t halfCount = indices.Count() / 2;
		const uint32_t colour = item.GetColour();
		const bool scaled = !FloatIsZero(m_Scale - 1.0f);

		if ( halfCount < 1 )
		{
			return;
		}

		Prepare(kRenderNormal, colour);

		gEngfuncs.pTriAPI->Begin(TRI_LINES);

		for ( uint32_t index = 0; index < halfCount; ++index )
		{
			uint8_t pointIndex0 = indices[2 * index];
			uint8_t pointIndex1 = indices[(2 * index) + 1];

			ASSERTSZ(pointIndex0 < static_cast<size_t>(points.Count()), "Index was out of range.");
			ASSERTSZ(pointIndex1 < static_cast<size_t>(points.Count()), "Index was out of range.");

			if ( scaled )
			{
				DefaultTextureCoOrdinate();
				gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex0] * m_Scale);

				DefaultTextureCoOrdinate();
				gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex1] * m_Scale);
			}
			else
			{
				DefaultTextureCoOrdinate();
				gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex0]);

				DefaultTextureCoOrdinate();
				gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex1]);
			}
		}

		gEngfuncs.pTriAPI->End();
	}

	void CGeometryRenderer::DrawTriangleFan(const CGeometryItem& item)
	{
		DrawTriangleChainGeneric(item, TRI_TRIANGLE_FAN);
	}

	void CGeometryRenderer::DrawTriangleStrip(const CGeometryItem& item)
	{
		DrawTriangleChainGeneric(item, TRI_TRIANGLE_STRIP);
	}

	// TODO: All of these drawing functions could be made into a single function
	// where the halfCount/thirdCount turns into a divisor parameter.
	void CGeometryRenderer::DrawTriangles(const CGeometryItem& item)
	{
		const CUtlVector<Vector>& points = item.GetPoints();
		const CUtlVector<uint8_t>& indices = item.GetIndices();
		const size_t thirdCount = indices.Count() / 3;
		const uint32_t colour = item.GetColour();
		const bool scaled = !FloatIsZero(m_Scale - 1.0f);

		if ( thirdCount < 1 )
		{
			return;
		}

		Prepare(kRenderNormal, colour);

		gEngfuncs.pTriAPI->Begin(TRI_TRIANGLES);

		for ( uint32_t index = 0; index < thirdCount; ++index )
		{
			uint8_t pointIndex0 = indices[3 * index];
			uint8_t pointIndex1 = indices[(3 * index) + 1];
			uint8_t pointIndex2 = indices[(3 * index) + 2];

			ASSERTSZ(pointIndex0 < static_cast<size_t>(points.Count()), "Index was out of range.");
			ASSERTSZ(pointIndex1 < static_cast<size_t>(points.Count()), "Index was out of range.");
			ASSERTSZ(pointIndex2 < static_cast<size_t>(points.Count()), "Index was out of range.");

			if ( scaled )
			{
				DefaultTextureCoOrdinate();
				gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex0] * m_Scale);

				DefaultTextureCoOrdinate();
				gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex1] * m_Scale);

				DefaultTextureCoOrdinate();
				gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex2] * m_Scale);
			}
			else
			{
				DefaultTextureCoOrdinate();
				gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex0]);

				DefaultTextureCoOrdinate();
				gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex1]);

				DefaultTextureCoOrdinate();
				gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex2]);
			}
		}

		gEngfuncs.pTriAPI->End();
	}

	void CGeometryRenderer::DrawTriangleChainGeneric(const CGeometryItem& item, int glDrawMode)
	{
		const CUtlVector<Vector>& points = item.GetPoints();
		const CUtlVector<uint8_t>& indices = item.GetIndices();
		const size_t count = indices.Count();
		const uint32_t colour = item.GetColour();
		const bool scaled = !FloatIsZero(m_Scale - 1.0f);

		if ( count < 3 )
		{
			return;
		}

		Prepare(kRenderNormal, colour);

		gEngfuncs.pTriAPI->Begin(glDrawMode);

		for ( uint32_t index = 0; index < count; ++index )
		{
			uint8_t pointIndex = indices[index];

			ASSERTSZ(pointIndex < static_cast<size_t>(points.Count()), "Index was out of range.");

			if ( scaled )
			{
				DefaultTextureCoOrdinate();
				gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex] * m_Scale);
			}
			else
			{
				DefaultTextureCoOrdinate();
				gEngfuncs.pTriAPI->Vertex3fv(points[pointIndex]);
			}
		}

		gEngfuncs.pTriAPI->End();
	}
}
