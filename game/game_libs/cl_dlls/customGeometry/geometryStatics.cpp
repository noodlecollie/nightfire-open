#include <memory>
#include "geometryStatics.h"
#include "cl_dll.h"
#include "customGeometry/geometryCollectionManager.h"
#include "customGeometry/geometryRenderer.h"
#include "customGeometry/sharedDefs.h"
#include "customGeometry/messageReader.h"
#include "customGeometry/primitiveMessageReader.h"
#include "customGeometry/logger_client.h"
#include "projectInterface/IProjectInterface.h"

namespace CustomGeometry
{
	static std::unique_ptr<CGeometryRenderer> AdHocRenderer;

	static void HandleSuccessfullyReceivedMessage(
		bool wasClearMessage,
		Category category,
		const CUtlVector<GeometryItemPtr_t>& items
	)
	{
		if ( wasClearMessage && category == Category::None )
		{
			CL_LOG().LogF(ILogInterface::Level::Debug, "Received custom geometry ClearAll message.\n");
			ClearAllGeometry();
			return;
		}

		CGeometryCollectionManager& manager = CGeometryCollectionManager::StaticInstance();
		CBaseGeometryCollection* collection = manager.CollectionForCategory(category);

		if ( !collection )
		{
			return;
		}

		if ( wasClearMessage )
		{
			CL_LOG().LogF(
				ILogInterface::Level::Debug,
				"Received custom geometry clear message for category %s\n",
				CustomGeometry::CategoryName(category)
			);

			collection->Clear();
			return;
		}

		FOR_EACH_VEC(items, index)
		{
			collection->AddItem(items[index]);
		}
	}

	static int HandleCustomGeometryMessage(const char*, int size, void* buffer)
	{
		CMessageReader reader;

		if ( reader.ReadMessage(buffer, size) )
		{
			CUtlVector<GeometryItemPtr_t> items;
			items.AddToTail(reader.GetGeometryItem());
			HandleSuccessfullyReceivedMessage(reader.WasClearMessage(), reader.GetGeometryCategory(), items);
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

	static int HandleCustomGeometryPrimitiveMessage(const char*, int size, void* buffer)
	{
		CPrimitiveMessageReader reader;

		if ( reader.ReadMessage(buffer, size) )
		{
			HandleSuccessfullyReceivedMessage(
				reader.WasClearMessage(),
				reader.GetGeometryCategory(),
				reader.GetGeometryItems()
			);
		}
		else
		{
			ILogInterface& log = IProjectInterface::ProjectInterfaceImpl()->LogInterface();
			log.LogF(
				ILogInterface::Level::Error,
				"Failed to parse custom geometry primitive message. Error: %s\n",
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
		gEngfuncs.pfnHookUserMsg(GEOMETRY_PRIMITIVE_MESSAGE_NAME, &HandleCustomGeometryPrimitiveMessage);
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
