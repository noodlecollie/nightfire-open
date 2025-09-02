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

		bool WriteMessage(uint32_t colour, float lifetimeSecs, const WireBallPrimitive* primitives, uint8_t count);

		template<typename T>
		bool WriteMessage(uint32_t colour, float lifetimeSecs, const T& primitive)
		{
			return WriteMessage(colour, lifetimeSecs, &primitive, 1);
		}

	private:
		static bool IsValidSpecificCategory(Category geomCategory);

		void WriteMessageHeader(Category category, PrimitiveType primitiveType, uint8_t count);
		void WriteClearMessageInternal(Category geomCategory);

		Category m_Category = Category::None;
		CBasePlayer* m_pTargetClient = nullptr;
	};
}  // namespace CustomGeometry
