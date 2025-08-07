#include <cmath>
#include "messageWriter.h"

namespace CustomGeometry
{
	static float GetLifetimeSecs(const CGeometryItem& geometry)
	{
		static constexpr float MAX_LIFETIME_SECS = static_cast<float>(std::numeric_limits<uint16_t>::max()) * 0.1f;

		float lifetimeSecs = 0.0f;

		if ( geometry.HasLifetime() )
		{
			lifetimeSecs = geometry.GetLifetimeSecs();
		}

		if ( std::isnan(lifetimeSecs) || lifetimeSecs < 0.0f )
		{
			lifetimeSecs = 0.0f;
		}
		else if ( lifetimeSecs > MAX_LIFETIME_SECS )
		{
			lifetimeSecs = MAX_LIFETIME_SECS;
		}

		return lifetimeSecs;
	}

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
	// - Lifetime (10ths of second) (uint16)
	// Then, if draw type is text:
	// - 3D position float [3]
	// - Text line offset
	// - Text string
	// Otherwise:
	// - Point count (uint16)
	// - Points (float) [3 * point count]
	// - Index count (uint16)
	// - Indices (uint8) [index count]
	bool CMessageWriter::WriteMessage(const CGeometryItem& geometry)
	{
		if ( !CanWriteMessage() || !IsValidSpecificCategory(m_Category) )
		{
			ASSERT(false);
			return false;
		}

		const ValidationResult validationResult = ValidateGeometryBeforeSend(geometry);
		ASSERTSZ(validationResult == ValidationResult::Valid, "Geometry item was not valid - check logs for info");

		if ( validationResult == ValidationResult::Invalid )
		{
			return false;
		}

		BeginMessage();
		WriteMessageHeader(m_Category, geometry.GetDrawType());

		WRITE_LONG(geometry.GetColour());
		WRITE_SHORT(static_cast<uint16_t>(GetLifetimeSecs(geometry) * 10.0f));

		if ( geometry.GetDrawType() == DrawType::Text )
		{
			const CUtlVector<Vector>& points = geometry.GetPoints();
			const CUtlString& text = geometry.GetText();
			const int lineOffset =
				std::max<int>(MIN_TEXT_LINE_OFFSET, std::min<int>(geometry.GetTextLineOffset(), MAX_TEXT_LINE_OFFSET));

			WRITE_VEC_PRECISE(points[0]);
			WRITE_CHAR(static_cast<char>(lineOffset));

			if ( text.Length() <= MAX_TEXT_CHARS_PER_MSG )
			{
				WRITE_STRING(text.Get());
			}
			else
			{
				CUtlString truncated = text;
				truncated.Truncate(MAX_TEXT_CHARS_PER_MSG);
				WRITE_STRING(truncated.Get());
			}
		}
		else
		{
			const CUtlVector<Vector>& points = geometry.GetPoints();
			const CUtlVector<size_t>& indices = geometry.GetIndices();

			const uint8_t indexCount =
				static_cast<uint8_t>(std::min<size_t>(static_cast<size_t>(indices.Count()), MAX_INDICES_PER_MSG));

			WRITE_BYTE(static_cast<uint8_t>(points.Count()));

			FOR_EACH_VEC(points, index)
			{
				WRITE_VEC_PRECISE(points[index]);
			}

			WRITE_BYTE(indexCount);

			for ( int index = 0; index < indexCount; ++index )
			{
				WRITE_BYTE(static_cast<uint8_t>(indices[index]));
			}
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

	CMessageWriter::ValidationResult CMessageWriter::ValidateGeometryBeforeSend(const CGeometryItem& geometry)
	{
		const DrawType drawType = geometry.GetDrawType();
		ValidationResult result = ValidationResult::Valid;

		if ( drawType != DrawType::Text )
		{
			const CUtlVector<Vector>& points = geometry.GetPoints();
			const CUtlVector<size_t>& indices = geometry.GetIndices();

			if ( points.IsEmpty() )
			{
				ALERT(at_error, "Discarding custom geometry item with no points.\n");
				return ValidationResult::Invalid;
			}

			// We can't truncate the points, since this could make indices become invalid.
			if ( static_cast<size_t>(points.Count()) > MAX_POINTS_PER_MSG )
			{
				ALERT(
					at_error,
					"Discarding custom geometry item with %d points (max points per message is %zu).\n",
					points.Count(),
					MAX_POINTS_PER_MSG);

				return ValidationResult::Invalid;
			}

			if ( indices.IsEmpty() )
			{
				ALERT(at_error, "Discarding custom geometry item with no indices.\n");
				return ValidationResult::Invalid;
			}

			if ( static_cast<size_t>(indices.Count()) > MAX_INDICES_PER_MSG )
			{
				ALERT(
					at_warning,
					"Custom geometry message contained %u indices. This will be truncated to %zu indices.\n",
					indices.Count(),
					MAX_INDICES_PER_MSG);

				result = ValidationResult::Truncated;
			}
		}
		else
		{
			const CUtlVector<Vector>& points = geometry.GetPoints();
			const CUtlString& text = geometry.GetText();
			const int lineOffset = geometry.GetTextLineOffset();

			if ( points.IsEmpty() )
			{
				ALERT(at_error, "Discarding custom geometry item specifying text, but with no location set.\n");
				return ValidationResult::Invalid;
			}

			if ( text.IsEmpty() )
			{
				ALERT(at_error, "Discarding custom geometry item specifying text, but with no characters.\n");
				return ValidationResult::Invalid;
			}

			if ( points.Count() != 1 )
			{
				ALERT(
					at_warning,
					"Custom geometry message contained text and more than one 3D point. The first point will be used "
					"as a location.\n");

				result = ValidationResult::Truncated;
			}

			if ( static_cast<size_t>(text.Length()) > MAX_TEXT_CHARS_PER_MSG )
			{
				ALERT(
					at_warning,
					"Custom geometry message contained %u text characters. This will be truncated to %zu characters.\n",
					text.Length(),
					MAX_TEXT_CHARS_PER_MSG);

				result = ValidationResult::Truncated;
			}

			if ( lineOffset < MIN_TEXT_LINE_OFFSET || lineOffset > MAX_TEXT_LINE_OFFSET )
			{
				ALERT(
					at_warning,
					"Custom geometry text line offset of %d was outside the range [%d %d], it will be clamped.\n",
					lineOffset,
					MIN_TEXT_LINE_OFFSET,
					MAX_TEXT_LINE_OFFSET);

				result = ValidationResult::Truncated;
			}
		}

		return result;
	}
}  // namespace CustomGeometry
