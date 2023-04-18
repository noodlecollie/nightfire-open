#include "gameresources/GameResources.h"
#include "filesystemops/FileSystemOps.h"

CGameResources& CGameResources::StaticInstance()
{
	static CGameResources* resources = nullptr;

	if ( !resources )
	{
		resources = new CGameResources();
	}

	return *resources;
}

CUtlString CGameResources::MultiplayerModelFullPath(const CUtlString& modelName)
{
	CUtlString path("models/player/");
	path.AppendFormat("%s/%s.mdl", modelName.String(), modelName.String());
	return path;
}

const CUtlVector<CUtlString>& CGameResources::MultiplayerModelList() const
{
	return m_MultiplayerModelList;
}

void CGameResources::Initialise()
{
	InitMultiplayerModelList();
}

void CGameResources::InitMultiplayerModelList()
{
	m_MultiplayerModelList.Purge();

	CUtlVector<DirectoryEntry> entries;
	FileSystemOps::ListDirectory("models/player", entries, false);

	FOR_EACH_VEC(entries, index)
	{
		const DirectoryEntry& entry = entries[index];

		// entry.FullPath() is the path to the subfolder.
		// We need to add on the name of the model.
		// TODO: Validate presence of file?
		CUtlString fullPath(entry.FullPath());
		fullPath.AppendFormat("/%s.mdl", entry.EntryName().String());

		m_MultiplayerModelList.AddToTail(fullPath);
	}
}
