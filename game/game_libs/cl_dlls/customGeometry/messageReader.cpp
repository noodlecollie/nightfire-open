#include "messageReader.h"
#include "standard_includes.h"
#include "customGeometry/sharedDefs.h"
#include "parsemsg.h"
#include "cl_dll.h"

namespace CustomGeometry
{
	bool CMessageReader::WasClearMessage() const
	{
		return m_WasClearMessage;
	}

	GeometryItemPtr_t CMessageReader::GetGeometryItem() const
	{
		return m_GeometryItem;
	}

	Category CMessageReader::GetGeometryCategory() const
	{
		return m_GeomCategory;
	}

	bool CMessageReader::ReadMessageInternal(void* buffer, int size)
	{
		m_GeometryItem.reset();

		BEGIN_READ(buffer, size);

		if ( !ReadGeometryCategory() )
		{
			return false;
		}

		const uint8_t drawType = static_cast<uint8_t>(READ_BYTE());

		if ( drawType == static_cast<uint8_t>(DrawType::None) )
		{
			m_WasClearMessage = true;

			// Nothing else to read.
			return true;
		}

		m_GeometryItem.reset(new CGeometryItem());

		switch ( static_cast<DrawType>(drawType) )
		{
			case DrawType::Lines:
			case DrawType::TriangleFan:
			case DrawType::TriangleStrip:
			case DrawType::Triangles:
			case DrawType::Text:
			{
				m_GeometryItem->SetDrawType(static_cast<DrawType>(drawType));
				break;
			}

			default:
			{
				SetErrorString("Unrecognised draw type.");
				ASSERT(false);
				return false;
			}
		}

		m_GeometryItem->SetColour(READ_LONG());

		const uint16_t lifetimeTenths = static_cast<uint16_t>(READ_SHORT());

		if ( lifetimeTenths > 0.0f )
		{
			const float dieTime = gEngfuncs.GetClientTime() + (static_cast<float>(lifetimeTenths) * 0.1f);
			m_GeometryItem->SetLifetimeSecs(dieTime);
		}

		if ( m_GeometryItem->GetDrawType() == DrawType::Text )
		{
			Vector location;
			READ_VEC_PRECISE(location);
			int lineOffset = static_cast<int>(READ_CHAR());
			m_GeometryItem->SetText(location, CUtlString(READ_STRING()), lineOffset);
		}
		else
		{
			const uint8_t pointCount = static_cast<uint8_t>(READ_BYTE());

			for ( uint32_t index = 0; index < pointCount; ++index )
			{
				Vector vec;
				READ_VEC_PRECISE(vec);
				m_GeometryItem->AddPoint(vec);
			}

			const uint8_t indexCount = static_cast<uint8_t>(READ_BYTE());

			for ( uint32_t index = 0; index < indexCount; ++index )
			{
				m_GeometryItem->AddIndex(static_cast<uint8_t>(READ_BYTE()));
			}
		}

		return true;
	}

	bool CMessageReader::ReadGeometryCategory()
	{
		const uint8_t categoryInt = static_cast<uint8_t>(READ_BYTE());

		if ( categoryInt >= CATEGORY_COUNT )
		{
			SetErrorString("Unrecognised geometry category.");
			return false;
		}

		m_GeomCategory = static_cast<Category>(categoryInt);
		return true;
	}

	void CMessageReader::ResetInternalData()
	{
		Messages::CBaseMessageReader::ResetInternalData();

		m_GeometryItem.reset();
		m_GeomCategory = Category::None;
		m_WasClearMessage = false;
	}
}  // namespace CustomGeometry
