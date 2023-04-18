#include "messageWriter.h"

namespace CustomGeometry
{
	CMessageWriter::CMessageWriter(Category category) :
		CBaseMessageWriter(MESSAGE_NAME, StaticUserMessageId()),
		m_Category(Category::None),
		m_pTargetClient(nullptr)
	{
		SetCategory(category);
	}

	Category CMessageWriter::GetCategory() const
	{
		return m_Category;
	}

	bool CMessageWriter::SetCategory(Category category)
	{
		if ( !IsValidSpecificCategory(category) )
		{
			return false;
		}

		m_Category = category;
		return true;
	}

	// Message format:
	// - Header
	// - Colour (uint32)
	// - Point count (uint16)
	// - Points (float) [3 * point count]
	// - Index count (uint16)
	// - Indices (uint8) [index count]
	bool CMessageWriter::WriteMessage(const CGeometryItem& geometry)
	{
		if ( !CanWriteMessage() || !IsValidSpecificCategory(m_Category) )
		{
			return false;
		}

		const CUtlVector<Vector>& points = geometry.GetPoints();
		const CUtlVector<uint8_t>& indices = geometry.GetIndices();

		if ( points.Count() < 1 )
		{
			return false;
		}

		if ( points.Count() > MAX_POINTS_PER_MSG )
		{
			ALERT(at_warning, "Attempted to send custom geometry message with %d points when max point count is %u.\n",
				  points.Count(),
				  MAX_POINTS_PER_MSG);
			return false;
		}

		BeginMessage();
		WriteMessageHeader(m_Category, geometry.GetDrawType());

		WRITE_LONG(geometry.GetColour());
		WRITE_SHORT(static_cast<uint16_t>(points.Count()));

		FOR_EACH_VEC(points, index)
		{
			WRITE_VEC_PRECISE(points[index]);
		}

		WRITE_SHORT(static_cast<uint16_t>(indices.Count()));

		FOR_EACH_VEC(indices, index)
		{
			WRITE_CHAR(indices[index]);
		}

		EndMessage();
		return true;
	}

	bool CMessageWriter::WriteClearMessage()
	{
		if ( !CanWriteMessage() || !IsValidSpecificCategory(m_Category) )
		{
			return false;
		}

		WriteClearMessageInternal(m_Category);
		return true;
	}

	// Message format:
	// - Header only; draw type is None.
	void CMessageWriter::WriteClearMessageInternal(Category geomCategory)
	{
		BeginMessage();
		WriteMessageHeader(geomCategory, DrawType::None);
		EndMessage();
	}

	// Header format:
	// - Category (uint8)
	// - Draw type (uint8)
	void CMessageWriter::WriteMessageHeader(Category category, DrawType drawType)
	{
		WRITE_BYTE(static_cast<uint8_t>(category));
		WRITE_BYTE(static_cast<uint8_t>(drawType));
	}

	bool CMessageWriter::IsValidSpecificCategory(Category geomCategory)
	{
		return geomCategory > Category::None && geomCategory < Category::CategoryCount;
	}
}
