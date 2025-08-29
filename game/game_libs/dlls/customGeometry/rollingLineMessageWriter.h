#pragma once

#include <cstdint>
#include <cmath>
#include "standard_includes.h"
#include "customGeometry/sharedDefs.h"
#include "customGeometry/geometryItemPtr.h"

namespace CustomGeometry
{
	class CRollingLineMessageWriter
	{
	public:
		explicit CRollingLineMessageWriter(Category category);
		~CRollingLineMessageWriter();

		void BeginGeometry(uint32_t colour = 0xFFFFFFFF, float scale = 1.0f, float lifetime = NAN);
		void Finalise();

		bool AddLine(const Vector& p0, const Vector& p1);

	private:
		// Max message size is actually 2048, but leave some headroom.
		static constexpr size_t MAX_MSG_LENGTH = 2000;

		void WriteClearMessage();
		void WriteGeometryMessage();
		void CreateGeometryItem(uint32_t colour, float scale, float lifetime);
		void EnsureCanAdd(size_t pointsToAdd, size_t indicesToAdd);
		bool CanAdd(size_t pointsToAdd, size_t indicesToAdd) const;
		bool ExtendsCurrentLine(const Vector& p0, const Vector& p1) const;

		Category m_Category;
		GeometryItemPtr_t m_CurrentGeometry;
	};
}  // namespace CustomGeometry
