#pragma once

#include "framework/BaseMenu.h"
#include "models/CvarModel.h"

namespace Rml
{
	class ElementProgress;
}

class ServerConnectionScreen : public BaseMenu
{
public:
	static const char* const NAME;

	ServerConnectionScreen();

	void Update(float currentTime) override;

	void ReceiveConnectionProgress_Connect(const Rml::String& server);
	void ReceiveConnectionProgress_ParseServerInfo(const Rml::String& server);
	void ReceiveConnectionProgress_Precache();
	void ReceiveConnectionProgress_Download(
		const Rml::String& fileName,
		const Rml::String& serverName,
		int current,
		int total,
		const Rml::String& comment
	);
	void ReceiveConnectionProgress_DownloadEnd();
	void ReceiveConnectionProgress_Connected();
	void ReceiveConnectionProgress_Disconnect();
	void ReceiveConnectionProgress_ChangeLevel();

protected:
	bool OnSetUpDataModelBindings(Rml::DataModelConstructor& constructor) override;

	void OnEndDocumentLoaded() override;
	void OnBeginDocumentUnloaded() override;

private:
	void ClearContentArea();
	void AppendContentLine(const Rml::String& text);

	CvarModel m_CvarModel;
	CvarDataVar<float>* m_CvarLoading = nullptr;
	CvarDataVar<float>* m_CvarDownload = nullptr;
	Rml::DataModelHandle m_ModelHandle;
	Rml::Element* m_ContentElement = nullptr;
	Rml::ElementProgress* m_ProgressElement = nullptr;
};
