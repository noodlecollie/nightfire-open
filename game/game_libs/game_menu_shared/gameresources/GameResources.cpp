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

const CUtlVector<CGameResources::MultiplayerModel>& CGameResources::MultiplayerModelList() const
{
	return m_MultiplayerModelList;
}

int CGameResources::ModelIndex(const CUtlString& modelName) const
{
	FOR_EACH_VEC(m_MultiplayerModelList, index)
	{
		if ( m_MultiplayerModelList[index].modelName == modelName )
		{
			return m_MultiplayerModelList[index].modelIndex;
		}
	}

	return -1;
}

void CGameResources::Initialise()
{
	InitMultiplayerModelList();
}

void CGameResources::Precache(const std::function<int(const char*)>& precacheCB)
{
	if ( !precacheCB )
	{
		return;
	}

	FOR_EACH_VEC(m_MultiplayerModelList, index)
	{
		m_MultiplayerModelList[index].modelIndex = precacheCB(m_MultiplayerModelList[index].modelFullPath.Get());
	}
}

void CGameResources::InitMultiplayerModelList()
{
	m_MultiplayerModelList.Purge();

	CUtlVector<DirectoryEntry> entries;
	FileSystemOps::ListDirectory("models/player", entries, false);

	FOR_EACH_VEC(entries, index)
	{
		const DirectoryEntry& entry = entries[index];

		m_MultiplayerModelList.AddToTail(
			MultiplayerModel {entry.EntryName(), MultiplayerModelFullPath(entry.EntryName()), -1});
	}
}
