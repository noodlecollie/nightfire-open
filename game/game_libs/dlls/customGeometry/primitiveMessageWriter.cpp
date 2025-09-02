#include <cmath>
#include "customGeometry/primitiveMessageWriter.h"

namespace CustomGeometry
{
	static float ClampLifetimeSecs(float lifetimeSecs)
	{
		static constexpr float MAX_LIFETIME_SECS = static_cast<float>(std::numeric_limits<uint16_t>::max()) * 0.1f;

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

	CPrimitiveMessageWriter::CPrimitiveMessageWriter(Category category) :
		CBaseMessageWriter(MESSAGE_NAME, StaticUserMessageId())
	{
		SetCategory(category);
	}

	Category CPrimitiveMessageWriter::GetCategory() const
	{
		return m_Category;
	}

	bool CPrimitiveMessageWriter::SetCategory(Category category)
	{
		if ( !IsValidSpecificCategory(category) )
		{
			return false;
		}

		m_Category = category;
		return true;
	}

	bool CPrimitiveMessageWriter::WriteClearMessage()
	{
		if ( !CanWriteMessage() || !IsValidSpecificCategory(m_Category) )
		{
			return false;
		}

		WriteClearMessageInternal(m_Category);
		return true;
	}

	// Message format:
	// - Header
	// - Colour (uint32)
	// - Lifetime (10ths of second) (uint16)
	// Then exactly `count` of:
	// - Origin (vec3_t)
	// - Radius (uint16)
	// - Num divisions (uint8_t)
	bool CPrimitiveMessageWriter::WriteMessage(
		uint32_t colour,
		float lifetimeSecs,
		const WireBallPrimitive* primitives,
		uint8_t count
	)
	{
		if ( !CanWriteMessage() || !IsValidSpecificCategory(m_Category) )
		{
			ASSERT(false);
			return false;
		}

		if ( !primitives || count < 1 )
		{
			ASSERT(false);
			return false;
		}

		for ( size_t index = 0; index < static_cast<size_t>(count); ++index )
		{
			if ( !primitives[index].IsValid() )
			{
				ASSERT(false);
				return false;
			}
		}

		BeginMessage();
		WriteMessageHeader(m_Category, WireBallPrimitive::TYPE, count);
		WRITE_LONG(colour);
		WRITE_SHORT(static_cast<uint16_t>(ClampLifetimeSecs(lifetimeSecs) * 10.0f));

		for ( size_t index = 0; index < static_cast<size_t>(count); ++index )
		{
			const WireBallPrimitive& primitive = primitives[index];
			WRITE_VEC_PRECISE(primitive.origin);
			WRITE_SHORT(primitive.radius);
			WRITE_BYTE(primitive.numDivisions);
		}

		EndMessage();
		return true;
	}

	// Message format:
	// - Header only; draw type is None.
	void CPrimitiveMessageWriter::WriteClearMessageInternal(Category geomCategory)
	{
		BeginMessage();
		WriteMessageHeader(geomCategory, PrimitiveType::None, 0);
		EndMessage();
	}

	// Header format:
	// - Category (uint8)
	// - Primitive type (uint8)
	// - Number of primitives in message (uint8)
	void CPrimitiveMessageWriter::WriteMessageHeader(Category category, PrimitiveType primitiveType, uint8_t count)
	{
		WRITE_BYTE(static_cast<uint8_t>(category));
		WRITE_BYTE(static_cast<uint8_t>(primitiveType));
		WRITE_BYTE(count);
	}

	bool CPrimitiveMessageWriter::IsValidSpecificCategory(Category geomCategory)
	{
		return geomCategory > Category::None && geomCategory < Category::CategoryCount;
	}
}  // namespace CustomGeometry
