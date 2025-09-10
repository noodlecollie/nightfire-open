#include "customGeometry/primitiveMessageReader.h"
#include "standard_includes.h"
#include "customGeometry/sharedDefs.h"
#include "customGeometry/primitiveDefs.h"
#include "customGeometry/constructors/wireBallConstructor.h"
#include "customGeometry/constructors/aabboxConstructor.h"
#include "customGeometry/constructors/waypointMarkerConstructor.h"
#include "parsemsg.h"
#include "cl_dll.h"

namespace CustomGeometry
{
	bool CPrimitiveMessageReader::WasClearMessage() const
	{
		return m_WasClearMessage;
	}

	const CUtlVector<GeometryItemPtr_t>& CPrimitiveMessageReader::GetGeometryItems() const
	{
		return m_GeometryItems;
	}

	Category CPrimitiveMessageReader::GetGeometryCategory() const
	{
		return m_GeomCategory;
	}

	bool CPrimitiveMessageReader::ReadMessageInternal(void* buffer, int size)
	{
		m_GeometryItems.Purge();

		BEGIN_READ(buffer, size);

		if ( !ReadGeometryCategory() )
		{
			return false;
		}

		const uint8_t primitiveType = static_cast<uint8_t>(READ_BYTE());
		const uint8_t count = static_cast<uint8_t>(READ_BYTE());

		if ( primitiveType == static_cast<uint8_t>(PrimitiveType::None) )
		{
			m_WasClearMessage = true;

			if ( count > 0 )
			{
				// Weird, but OK...
				gEngfuncs.Con_Printf(
					"Unexpected count value of %u when reading geometry primitive clear message, ignoring\n",
					static_cast<uint32_t>(count)
				);
			}

			// Nothing else to read.
			return true;
		}

		const uint32_t colour = static_cast<uint32_t>(READ_LONG());
		const uint16_t lifetimeTenths = static_cast<uint16_t>(READ_SHORT());

		for ( size_t index = 0; index < static_cast<size_t>(count); ++index )
		{
			GeometryItemPtr_t item;

			switch ( static_cast<PrimitiveType>(primitiveType) )
			{
				case PrimitiveType::WireBall:
				{
					item = CreateWireBall();
					break;
				}

				case PrimitiveType::AABBox:
				{
					item = CreateAABBox();
					break;
				}

				case PrimitiveType::WaypointMarker:
				{
					item = CreateWaypointMarker();
					break;
				}

				default:
				{
					SetErrorString("Unrecognised primitive type.");
					ASSERT(false);
					return false;
				}
			}

			if ( !item )
			{
				if ( !ErrorString().IsEmpty() )
				{
					// Make sure we know which item it was that failed.
					CUtlString errorStr;
					errorStr.AppendFormat("[%u] ", static_cast<uint32_t>(index));
					errorStr.Append(ErrorString());
					SetErrorString(errorStr);
				}
				else
				{
					SetErrorString("Failed to construct geometry for primitive.");
				}

				m_GeometryItems.Purge();
				return false;
			}

			item->SetColour(colour);

			if ( lifetimeTenths > 0.0f )
			{
				const float dieTime = gEngfuncs.GetClientTime() + (static_cast<float>(lifetimeTenths) * 0.1f);
				item->SetLifetimeSecs(dieTime);
			}

			m_GeometryItems.AddToTail(item);
		}

		return true;
	}

	bool CPrimitiveMessageReader::ReadGeometryCategory()
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

	GeometryItemPtr_t CPrimitiveMessageReader::CreateWireBall()
	{
		WireBallPrimitive wireBall {};
		READ_VEC_PRECISE(wireBall.origin);
		wireBall.radius = static_cast<uint16_t>(READ_SHORT());
		wireBall.numDivisions = static_cast<uint8_t>(READ_BYTE());

		CWireBallConstructor constructor;
		constructor.SetPrimitive(wireBall);

		if ( !constructor.IsValid() )
		{
			SetErrorString("Wire ball primitive was not valid.");
			return nullptr;
		}

		return constructor.Construct();
	}

	GeometryItemPtr_t CPrimitiveMessageReader::CreateAABBox()
	{
		AABBoxPrimitive bbox {};
		READ_VEC_PRECISE(bbox.mins);
		READ_VEC_PRECISE(bbox.maxs);

		CAABBoxConstructor constructor;
		constructor.SetBounds(bbox.mins, bbox.maxs);

		if ( !constructor.IsValid() )
		{
			SetErrorString("AABBox primitive was not valid.");
			return nullptr;
		}

		return constructor.Construct();
	}

	GeometryItemPtr_t CPrimitiveMessageReader::CreateWaypointMarker()
	{
		WaypointMarkerPrimitive waypoint {};
		READ_VEC_PRECISE(waypoint.location);

		CWaypointMarkerConstructor constructor;
		constructor.SetWaypoint(waypoint);

		if ( !constructor.IsValid() )
		{
			SetErrorString("Waypoint marker primitive was not valid.");
			return nullptr;
		}

		return constructor.Construct();
	}

	void CPrimitiveMessageReader::ResetInternalData()
	{
		Messages::CBaseMessageReader::ResetInternalData();

		m_GeometryItems.Purge();
		m_GeomCategory = Category::None;
		m_WasClearMessage = false;
	}
}  // namespace CustomGeometry
