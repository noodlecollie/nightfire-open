#pragma once

#include "messages/baseMessageReader.h"
#include "customGeometry/geometryItem.h"
#include "utlvector.h"

namespace CustomGeometry
{
	class CPrimitiveMessageReader : public Messages::CBaseMessageReader
	{
	public:
		bool WasClearMessage() const;
		const CUtlVector<GeometryItemPtr_t>& GetGeometryItems() const;
		Category GetGeometryCategory() const;

	protected:
		virtual bool ReadMessageInternal(void* buffer, int size) override;
		virtual void ResetInternalData() override;

	private:
		bool ReadGeometryCategory();
		GeometryItemPtr_t CreateWireBall();
		GeometryItemPtr_t CreateAABBox();

		CUtlVector<GeometryItemPtr_t> m_GeometryItems;
		Category m_GeomCategory = Category::None;
		bool m_WasClearMessage = false;
	};
}  // namespace CustomGeometry
