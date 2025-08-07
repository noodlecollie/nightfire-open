#pragma once

#include "standard_includes.h"
#include "utlvector.h"
#include "utlstring.h"
#include "customGeometry/sharedDefs.h"
#include "customGeometry/geometryItemPtr.h"

namespace CustomGeometry
{
	class CGeometryItem
	{
	public:
		void Reset();
		void ClearGeometry();
		bool IsEmpty() const;

		DrawType GetDrawType() const;
		void SetDrawType(DrawType type);

		uint32_t GetColour() const;
		void SetColour(uint32_t col);

		float GetScale() const;
		void SetScale(float scale);

		// These add points only.
		bool AddPoint(const Vector& p0);

		// These add points and relevant indices.
		bool AddLine(const Vector& p0, const Vector& p1);
		bool AddPointAndIndex(const Vector& p0);

		// Expects a quad with vertices specified in consecutive order around the perimeter.
		// The direction that the perimeter is traversed defines the winding of the quad.
		bool AddTriangleQuad(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3);

		// Expects a triangle with vertices specified in consecutive order around the perimeter.
		// The direction that the perimeter is traversed defines the winding of the triangle.
		bool AddTriangle(const Vector& p0, const Vector& p1, const Vector& p2);

		// These only add indices, and assume that the indices are for valid points
		// (they are not checked).
		bool AddIndex(size_t index);
		bool AddLineIndices(size_t i0, size_t i1);

		bool SetText(const Vector& pos, const CUtlString& text, int lineOffset = 0);

		size_t GetPointCount() const;

		const CUtlVector<Vector>& GetPoints() const;
		const CUtlVector<size_t>& GetIndices() const;
		const CUtlString& GetText() const;
		int GetTextLineOffset() const;

		Vector& GetPoint(size_t index);
		const Vector& GetPoint(size_t index) const;

		size_t& GetIndex(size_t i);
		const size_t& GetIndex(size_t i) const;

		float GetLifetimeSecs() const;
		void SetLifetimeSecs(float lifetime);
		bool HasLifetime() const;

	private:
		size_t CurrentBaseIndex() const;

		DrawType m_DrawType = DrawType::None;
		uint32_t m_Colour = 0xFFFFFFFF;
		CUtlVector<Vector> m_Points;
		CUtlVector<size_t> m_Indices;
		CUtlString m_Text;
		int m_TextLineOffset = 0;
		float m_Scale = 1.0f;
		float m_Lifetime = NAN;
	};
}  // namespace CustomGeometry
