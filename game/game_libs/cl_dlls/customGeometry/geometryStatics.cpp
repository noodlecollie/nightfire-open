#include <memory>
#include "geometryStatics.h"
#include "cl_dll.h"
#include "customGeometry/geometryCollectionManager.h"
#include "customGeometry/geometryRenderer.h"
#include "customGeometry/sharedDefs.h"
#include "customGeometry/messageReader.h"
#include "projectInterface/IProjectInterface.h"
#include "customGeometry/logger_client.h"

namespace CustomGeometry
{
	static std::unique_ptr<CGeometryRenderer> AdHocRenderer;

	static void HandleSuccessfullyReceivedMessage(const CMessageReader& reader)
	{
		if ( reader.WasClearMessage() && reader.GetGeometryCategory() == Category::None )
		{
			CL_LOG().LogF(ILogInterface::Level::Message, "Received custom geometry ClearAll message.\n");
			ClearAllGeometry();
			return;
		}

		CGeometryCollectionManager& manager = CGeometryCollectionManager::StaticInstance();
		CBaseGeometryCollection* collection = manager.CollectionForCategory(reader.GetGeometryCategory());

		if ( !collection )
		{
			return;
		}

		if ( reader.WasClearMessage() )
		{
			CL_LOG().LogF(
				ILogInterface::Level::Message,
				"Received custom geometry clear message for category %s\n",
				CustomGeometry::CategoryName(reader.GetGeometryCategory())
			);

			collection->Clear();
			return;
		}

		GeometryItemPtr_t item = reader.GetGeometryItem();

		CL_LOG().LogF(
			ILogInterface::Level::Message,
			"Received custom geometry for category %s (%d points, %d indices)\n",
			CustomGeometry::CategoryName(reader.GetGeometryCategory()),
			item->GetPoints().Count(),
			item->GetIndices().Count()
		);

		collection->AddItem(item);
	}

	static int HandleCustomGeometryMessage(const char*, int size, void* buffer)
	{
		CMessageReader reader;

		if ( reader.ReadMessage(buffer, size) )
		{
			HandleSuccessfullyReceivedMessage(reader);
		}
		else
		{
			ILogInterface& log = IProjectInterface::ProjectInterfaceImpl()->LogInterface();
			log.LogF(
				ILogInterface::Level::Error,
				"Failed to parse custom geometry message. Error: %s\n",
				reader.ErrorString().Get()
			);
		}

		return 1;
	}

	void Initialise()
	{
		CGeometryCollectionManager& manager = CGeometryCollectionManager::StaticInstance();

		// Specify any custom geometry collection factory functions before this call, via manager.SetFactoryFunction().
		// This is used for providing the manager with custom CBaseGeometryCollection subclasses.
		manager.Initialise();
		AdHocRenderer.reset(new CGeometryRenderer());

		gEngfuncs.pfnHookUserMsg(CUSTOM_GEOMETRY_MESSAGE_NAME, &HandleCustomGeometryMessage);
	}

	void VidInit()
	{
		CGeometryCollectionManager& manager = CGeometryCollectionManager::StaticInstance();
		manager.VidInit();

		if ( AdHocRenderer )
		{
			AdHocRenderer->VidInit();
		}
		else
		{
			gEngfuncs.Con_Printf("CustomGeometry::VidInit: Ad-hoc renderer was not initialised!\n");
		}
	}

	void RenderAllGeometry(RenderType renderType)
	{
		CGeometryCollectionManager& manager = CGeometryCollectionManager::StaticInstance();

		for ( uint32_t index = 1; index < CATEGORY_COUNT; ++index )
		{
			CBaseGeometryCollection* collection = manager.CollectionForCategory(static_cast<Category>(index));

			if ( !collection )
			{
				continue;
			}

			collection->Render(renderType);
		}
	}

	void RenderAdHocGeometry(const GeometryItemPtr_t& item)
	{
		if ( AdHocRenderer && item )
		{
			AdHocRenderer->Render(*item);
		}
	}

	void ClearAllGeometry()
	{
		CGeometryCollectionManager& manager = CGeometryCollectionManager::StaticInstance();

		for ( uint32_t index = 1; index < CATEGORY_COUNT; ++index )
		{
			CBaseGeometryCollection* collection = manager.CollectionForCategory(static_cast<Category>(index));

			if ( !collection )
			{
				continue;
			}

			collection->Clear();
		}
	}
}  // namespace CustomGeometry
