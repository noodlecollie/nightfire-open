#pragma once
#include "utlstring.h"
#include "utlvector.h"

class DirectoryEntry
{
public:
	DirectoryEntry();

	CUtlString FullPath() const;
	CUtlString EntryName() const;

private:
	friend class FileSystemOps;

	DirectoryEntry(const CUtlString& fullPath, const CUtlString& entryName);

	CUtlString m_FullPath;
	CUtlString m_EntryName;
};

class FileSystemOps
{
public:
	static void
	ListDirectory(const CUtlString& directoryPath, CUtlVector<DirectoryEntry>& outList, bool gameDirOnly = true);
};
