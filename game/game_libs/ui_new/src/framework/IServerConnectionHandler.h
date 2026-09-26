#pragma once

class IServerConnectionHandler
{
public:
	virtual ~IServerConnectionHandler() = default;

	virtual void Connect(const char* server, bool isBackground) = 0;
	virtual void ParseServerInfo(const char* server) = 0;
	virtual void Precache(const char* mapFileName) = 0;
	virtual void
	Download(const char* fileName, const char* serverName, int current, int total, const char* comment) = 0;
	virtual void DownloadEnd() = 0;
	virtual void Connected() = 0;
	virtual void Disconnect() = 0;
	virtual void ChangeLevel(bool isBackground) = 0;
};
