#pragma once

class IServerConnectionMenu
{
public:
	virtual ~IServerConnectionMenu() = default;

	virtual void Connect(const char* server) = 0;
	virtual void ParseServerInfo(const char* server) = 0;
	virtual void Precache() = 0;
	virtual void
	Download(const char* pszFileName, const char* pszServerName, int iCurrent, int iTotal, const char* comment) = 0;
	virtual void DownloadEnd() = 0;
	virtual void Connected() = 0;
	virtual void Disconnect() = 0;
	virtual void ChangeLevel() = 0;
};
