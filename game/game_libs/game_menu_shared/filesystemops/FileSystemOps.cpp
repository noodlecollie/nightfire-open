#include "filesystemops/FileSystemOps.h"
#include "projectInterface/IProjectInterface.h"

DirectoryEntry::DirectoryEntry()
{
}

DirectoryEntry::DirectoryEntry(const CUtlString& fullPath, const CUtlString& entryName) :
	m_FullPath(fullPath),
	m_EntryName(entryName)
{
}

CUtlString DirectoryEntry::FullPath() const
{
	return m_FullPath;
}

CUtlString DirectoryEntry::EntryName() const
{
	return m_EntryName;
}

void FileSystemOps::ListDirectory(
	const CUtlString& directoryPath,
	CUtlVector<DirectoryEntry>& outList,
	bool gameDirOnly)
{
	outList.Purge();

	IProjectInterface* ifc = IProjectInterface::ProjectInterfaceImpl();

	size_t numFiles = 0;
	char** fileList = ifc->FileLoader().ListDirectory(directoryPath, numFiles, gameDirOnly);

	size_t origPathLength = directoryPath.Length();

	for ( uint32_t index = 0; index < numFiles; ++index )
	{
		CUtlString fullPath(fileList[index]);
		CUtlString entryName(fileList[index] + origPathLength + 1);

		outList.AddToTail(DirectoryEntry(fullPath, entryName));
	}
}
