#pragma once

#include "standard_includes.h"
#include "customGeometry/geometryItem.h"
#include "messages/baseMessageWriter.h"
#include "messages/messageRegistrationHelper.h"

namespace CustomGeometry
{
	class CMessageWriter :
		public Messages::CBaseMessageWriter,
		public Messages::CMessageRegistrationHelper<CMessageWriter>
	{
	public:
		static constexpr const char* MESSAGE_NAME = CustomGeometry::MESSAGE_NAME;
		static constexpr int MESSAGE_SIZE = -1;

		// Category must be greater than None and less than CategoryCount.
		explicit CMessageWriter(Category category);

		Category GetCategory() const;
		bool SetCategory(Category category);

		// Writes message given currently set category.
		// If category is None, these functions will fail and return false.
		bool WriteMessage(const CGeometryItem& geometry);
		bool WriteClearMessage();

	private:
		static bool IsValidSpecificCategory(Category geomCategory);

		void WriteMessageHeader(Category category, DrawType drawType);
		void WriteClearMessageInternal(Category geomCategory);

		Category m_Category;
		CBasePlayer* m_pTargetClient;
	};
}  // namespace CustomGeometry
