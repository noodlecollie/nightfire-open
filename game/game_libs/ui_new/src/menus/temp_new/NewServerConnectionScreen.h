#pragma once

#include "framework/BaseMenu.h"
#include "framework/IServerConnectionMenu.h"
#include "models/CvarModel.h"

namespace Rml
{
	class ElementProgress;
}

class NewServerConnectionScreen : public BaseMenu, public IServerConnectionMenu
{
public:
	static const char* const NAME;

	NewServerConnectionScreen();

	void Update(float currentTime) override;

	// IServerConnectionMenu
	void Connect(const char* server, bool isBackground) override;
	void ParseServerInfo(const char* server) override;
	void Precache(const char* mapFileName) override;
	void Download(const char* fileName, const char* serverName, int current, int total, const char* comment) override;
	void DownloadEnd() override;
	void Connected() override;
	void Disconnect() override;
	void ChangeLevel(bool isBackground) override;

protected:
	void OnDocumentLoaded() override;
	void OnDocumentUnloaded() override;

private:
	void ClearContentArea();
	void AppendContentLine(const Rml::String& text);

	CvarModel m_CvarModel;
	CvarDataVar<float>* m_CvarLoading = nullptr;
	CvarDataVar<float>* m_CvarDownload = nullptr;
	Rml::Element* m_ContentElement = nullptr;
	Rml::ElementProgress* m_ProgressElement = nullptr;
	bool m_Connecting = false;
};
