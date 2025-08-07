#pragma once

#include "filesystemops/FileSystemOps.h"
#include "utlvector.h"
#include "utlstring.h"
#include <functional>

class CGameResources
{
public:
	struct MultiplayerModel
	{
		CUtlString modelName;
		CUtlString modelFullPath;
		int modelIndex;
	};

	static CGameResources& StaticInstance();

	// Model name should be provided without the .mdl extension.
	static CUtlString MultiplayerModelFullPath(const CUtlString& modelName);

	void Initialise();
	void Precache(const std::function<int(const char*)>& precacheCB);

	const CUtlVector<MultiplayerModel>& MultiplayerModelList() const;
	int ModelIndex(const CUtlString& modelName) const;

private:
	void InitMultiplayerModelList();

	CUtlVector<MultiplayerModel> m_MultiplayerModelList;
};
