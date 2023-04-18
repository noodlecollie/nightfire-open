#include "messageReader.h"
#include "standard_includes.h"
#include "customGeometry/sharedDefs.h"
#include "parsemsg.h"

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

		const uint16_t pointCount = static_cast<uint16_t>(READ_SHORT());

		if ( pointCount > MAX_POINTS_PER_MSG )
		{
			SetErrorString("Point count exceeded max allowed points.");
			return false;
		}

		for ( uint32_t index = 0; index < pointCount; ++index )
		{
			Vector vec;
			READ_VEC_PRECISE(vec);
			m_GeometryItem->AddPoint(vec);
		}

		const uint16_t indexCount = static_cast<uint16_t>(READ_SHORT());

		if ( indexCount > MAX_INDICES_PER_MSG )
		{
			SetErrorString("Index count exceeded max allowed indices.");
			return false;
		}

		for ( uint32_t index = 0; index < indexCount; ++index )
		{
			m_GeometryItem->AddIndex(static_cast<uint8_t>(READ_CHAR()));
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
}
