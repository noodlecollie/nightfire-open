#pragma once

#include "standard_includes.h"
#include "customGeometry/geometryItem.h"
#include "messages/baseMessageWriter.h"
#include "messages/messageRegistrationHelper.h"
#include "customGeometry/primitiveDefs.h"

namespace CustomGeometry
{
	class CPrimitiveMessageWriter :
		public Messages::CBaseMessageWriter,
		public Messages::CMessageRegistrationHelper<CPrimitiveMessageWriter>
	{
	public:
		static constexpr const char* MESSAGE_NAME = CustomGeometry::GEOMETRY_PRIMITIVE_MESSAGE_NAME;
		static constexpr int MESSAGE_SIZE = -1;

		// Category must be greater than None and less than CategoryCount.
		explicit CPrimitiveMessageWriter(Category category);

		Category GetCategory() const;
		bool SetCategory(Category category);

		// Writes message given currently set category.
		// If category is None, these functions will fail and return false.
		bool WriteClearMessage();

		template<typename T>
		bool WriteMessage(uint32_t colour, float lifetimeSecs, const T& primitive)
		{
			return WriteMessage(colour, lifetimeSecs, &primitive, 1);
		}

		// Message format:
		// - Header
		// - Colour (uint32)
		// - Lifetime (10ths of second) (uint16)
		// Then exactly `count` of the specified primitive's payload.
		template<typename T>
		bool WriteMessage(uint32_t colour, float lifetimeSecs, const T* primitives, uint8_t count)
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
			WriteMessageHeader(m_Category, T::TYPE, count);
			WRITE_LONG(colour);
			WRITE_SHORT(static_cast<uint16_t>(ClampLifetimeSecs(lifetimeSecs) * 10.0f));

			for ( size_t index = 0; index < static_cast<size_t>(count); ++index )
			{
				WritePrimitive(primitives[index]);
			}

			EndMessage();
			return true;
		}

	private:
		static bool IsValidSpecificCategory(Category geomCategory);
		static float ClampLifetimeSecs(float lifetimeSecs);

		void WriteMessageHeader(Category category, PrimitiveType primitiveType, uint8_t count);
		void WriteClearMessageInternal(Category geomCategory);

		void WritePrimitive(const WireBallPrimitive& primitive);
		void WritePrimitive(const AABBoxPrimitive& primitive);

		Category m_Category = Category::None;
		CBasePlayer* m_pTargetClient = nullptr;
	};
}  // namespace CustomGeometry
