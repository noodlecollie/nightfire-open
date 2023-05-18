#pragma once

#include "messages/baseMessageReader.h"
#include "customGeometry/geometryItem.h"

namespace CustomGeometry
{
	class CMessageReader : public Messages::CBaseMessageReader
	{
	public:
		bool WasClearMessage() const;
		GeometryItemPtr_t GetGeometryItem() const;
		Category GetGeometryCategory() const;

	protected:
		virtual bool ReadMessageInternal(void* buffer, int size) override;
		virtual void ResetInternalData() override;

	private:
		bool ReadGeometryCategory();

		GeometryItemPtr_t m_GeometryItem;
		Category m_GeomCategory = Category::None;
		bool m_WasClearMessage = false;
	};
}  // namespace CustomGeometry
