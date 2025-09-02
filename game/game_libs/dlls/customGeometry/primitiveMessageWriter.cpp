#include <cmath>
#include "customGeometry/primitiveMessageWriter.h"

namespace CustomGeometry
{
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
	// - Header only; draw type is None.
	void CPrimitiveMessageWriter::WriteClearMessageInternal(Category geomCategory)
	{
		BeginMessage();
		WriteMessageHeader(geomCategory, PrimitiveType::None, 0);
		EndMessage();
	}

	// Payload format:
	// - Origin (vec3_t)
	// - Radius (uint16)
	// - Num divisions (uint8_t)
	void CPrimitiveMessageWriter::WritePrimitive(const WireBallPrimitive& primitive)
	{
		WRITE_VEC_PRECISE(primitive.origin);
		WRITE_SHORT(primitive.radius);
		WRITE_BYTE(primitive.numDivisions);
	}

	// Payload format:
	// - Mins (vec3_t)
	// - Maxs (vec3_t)
	void CPrimitiveMessageWriter::WritePrimitive(const AABBoxPrimitive& primitive)
	{
		WRITE_VEC_PRECISE(primitive.mins);
		WRITE_VEC_PRECISE(primitive.maxs);
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

	float CPrimitiveMessageWriter::ClampLifetimeSecs(float lifetimeSecs)
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
}  // namespace CustomGeometry
